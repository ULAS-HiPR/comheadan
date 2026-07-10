#include <SPI/SPI_STM.h>

#ifdef STM

#if defined(__has_include)
#if __has_include("cmsis_os.h")
#include "cmsis_os.h"
#define COMHEADAN_HAS_CMSIS_OS 1
#endif
#endif

#ifndef COMHEADAN_HAS_CMSIS_OS
#define COMHEADAN_HAS_CMSIS_OS 0
#endif

namespace {
constexpr uint32_t kSpiTransferTimeoutMs = 100U;

#if COMHEADAN_HAS_CMSIS_OS
constexpr std::size_t kMaxSpiBuses = 4U;

struct SpiBusMutex {
    SPI_HandleTypeDef* handle{nullptr};
    osMutexId_t mutex{nullptr};
};

SpiBusMutex spi_bus_mutexes[kMaxSpiBuses]{};

osMutexId_t register_bus(SPI_HandleTypeDef* handle) {
    for (SpiBusMutex& slot : spi_bus_mutexes) {
        if (slot.handle == handle) {
            return slot.mutex;
        }
    }
    for (SpiBusMutex& slot : spi_bus_mutexes) {
        if (slot.handle == nullptr) {
            slot.handle = handle;
            slot.mutex = osMutexNew(nullptr);
            return slot.mutex;
        }
    }
    return nullptr;
}

osMutexId_t mutex_for(SPI_HandleTypeDef* handle) {
    for (SpiBusMutex& slot : spi_bus_mutexes) {
        if (slot.handle == handle) {
            return slot.mutex;
        }
    }
    return nullptr;
}

bool acquire_bus(SPI_HandleTypeDef* handle, bool& acquired) {
    acquired = false;
    if (osKernelGetState() != osKernelRunning) {
        return true;
    }
    osMutexId_t mutex = mutex_for(handle);
    if (mutex == nullptr || osMutexAcquire(mutex, kSpiTransferTimeoutMs) != osOK) {
        return false;
    }
    acquired = true;
    return true;
}

void release_bus(SPI_HandleTypeDef* handle, bool acquired) {
    if (!acquired) {
        return;
    }
    osMutexId_t mutex = mutex_for(handle);
    if (mutex != nullptr) {
        (void)osMutexRelease(mutex);
    }
}
#else
void register_bus(SPI_HandleTypeDef*) {}

bool acquire_bus(SPI_HandleTypeDef*, bool& acquired) {
    acquired = false;
    return true;
}

void release_bus(SPI_HandleTypeDef*, bool) {}
#endif
}

SPI_STM::SPI_STM(SPI_HandleTypeDef* hspi, GPIO_TypeDef* cs_port, uint16_t cs_pin) : _hspi(hspi), _cs_port(cs_port), _cs_pin(cs_pin)
    , _last_status(HAL_OK), _last_error(0), _bus_mutex_acquired(false), _bus_lock_failed(false)
{
    register_bus(_hspi);
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_SET);
}

bool SPI_STM::update_status(HAL_StatusTypeDef status)
{
    _last_status = static_cast<uint32_t>(status);
    _last_error = HAL_SPI_GetError(_hspi);
    return status == HAL_OK;
}

void SPI_STM::cs_low()
{
    _bus_lock_failed = !acquire_bus(_hspi, _bus_mutex_acquired);
    if (_bus_lock_failed) {
        _last_status = HAL_TIMEOUT;
        _last_error = HAL_SPI_ERROR_FLAG;
        return;
    }
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_RESET);
}

void SPI_STM::cs_high()
{
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_SET);
    release_bus(_hspi, _bus_mutex_acquired);
    _bus_mutex_acquired = false;
    _bus_lock_failed = false;
}

void SPI_STM::cs_select(int)
{
    cs_low();
}

void SPI_STM::cs_deselect(int)
{
    cs_high();
}

bool SPI_STM::write(int cs, uint8_t reg, uint8_t *buf, uint16_t len)
{
    cs_select(cs);
    bool ok = transmit(&reg, 1) && transmit(buf, len);
    cs_deselect(cs);
    return ok;
}


bool SPI_STM::read(int cs, uint8_t reg, uint8_t* buf, uint16_t len)
{
    cs_select(cs);
    bool ok = read_no_cs(reg, buf, len);
    cs_deselect(cs);
    return ok;
}

bool SPI_STM::read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t addr = reg | 0x80;
    return transmit(&addr, 1) && receive(buf, len);
}


bool SPI_STM::write_no_cs(uint8_t reg, const uint8_t *buf, uint16_t len)
{
    return transmit(&reg, 1) && transmit(buf, len);
}

bool SPI_STM::transmit(const uint8_t *data, std::size_t len)
{
    if (_bus_lock_failed) {
        return false;
    }
    if ((data == nullptr) && (len > 0U)) {
        _last_status = HAL_ERROR;
        _last_error = HAL_SPI_ERROR_FLAG;
        return false;
    }

    while (len > 0) {
        uint16_t chunk = len > UINT16_MAX ? UINT16_MAX : static_cast<uint16_t>(len);
        if (!update_status(HAL_SPI_Transmit(
                _hspi, const_cast<uint8_t*>(data), chunk, kSpiTransferTimeoutMs))) {
            return false;
        }
        data += chunk;
        len -= chunk;
    }
    return true;
}

bool SPI_STM::receive(uint8_t *buf, std::size_t len)
{
    if (_bus_lock_failed) {
        return false;
    }
    if ((buf == nullptr) && (len > 0U)) {
        _last_status = HAL_ERROR;
        _last_error = HAL_SPI_ERROR_FLAG;
        return false;
    }

    while (len > 0) {
        uint16_t chunk = len > UINT16_MAX ? UINT16_MAX : static_cast<uint16_t>(len);
        if (!update_status(HAL_SPI_Receive(_hspi, buf, chunk, kSpiTransferTimeoutMs))) {
            return false;
        }
        buf += chunk;
        len -= chunk;
    }
    return true;
}

bool SPI_STM::transfer(const uint8_t *tx, uint8_t *rx, std::size_t len)
{
    if (_bus_lock_failed) {
        return false;
    }
    if (((tx == nullptr) || (rx == nullptr)) && (len > 0U)) {
        _last_status = HAL_ERROR;
        _last_error = HAL_SPI_ERROR_FLAG;
        return false;
    }

    while (len > 0) {
        uint16_t chunk = len > UINT16_MAX ? UINT16_MAX : static_cast<uint16_t>(len);
        if (!update_status(HAL_SPI_TransmitReceive(
                _hspi, const_cast<uint8_t*>(tx), rx, chunk, kSpiTransferTimeoutMs))) {
            return false;
        }
        tx += chunk;
        rx += chunk;
        len -= chunk;
    }
    return true;
}


void SPI_STM::delay_ms(int ms)
{
    if (ms <= 0) {
        return;
    }

#if COMHEADAN_HAS_CMSIS_OS
    if (osKernelGetState() == osKernelRunning) {
        osDelay(static_cast<uint32_t>(ms));
        return;
    }
#endif

    for (int remaining = 0; remaining < ms; ++remaining) {
        volatile uint32_t cycles = SystemCoreClock / 8000U;
        while (cycles-- > 0U) {
            __NOP();
        }
    }
}

uint32_t SPI_STM::last_status() const
{
    return _last_status;
}

uint32_t SPI_STM::last_error() const
{
    return _last_error;
}

#endif
