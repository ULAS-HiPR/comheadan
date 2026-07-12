#include <UART/UART_STM.h>

#ifdef STM

UART_STM::UART_STM(UART_HandleTypeDef* huart)
    : _huart(huart), _last_status(HAL_OK), _last_error(0), _rx_overrun_recoveries(0)
{
}

bool UART_STM::update_status(HAL_StatusTypeDef status)
{
    _last_status = static_cast<std::uint32_t>(status);
    _last_error = HAL_UART_GetError(_huart);
    return status == HAL_OK;
}

bool UART_STM::write(const std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms)
{
    if ((data == nullptr) && (len > 0U)) {
        _last_status = HAL_ERROR;
        _last_error = HAL_UART_ERROR_PE;
        return false;
    }

    while (len > 0U) {
        std::uint16_t chunk = len > UINT16_MAX ? UINT16_MAX : static_cast<std::uint16_t>(len);
        if (!update_status(HAL_UART_Transmit(_huart, const_cast<std::uint8_t*>(data), chunk, timeout_ms))) {
            return false;
        }
        data += chunk;
        len -= chunk;
    }

    return true;
}

std::size_t UART_STM::read(std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms)
{
    if ((data == nullptr) && (len > 0U)) {
        _last_status = HAL_ERROR;
        _last_error = HAL_UART_ERROR_PE;
        return 0U;
    }

    if (__HAL_UART_GET_FLAG(_huart, UART_FLAG_ORE) != RESET) {
        __HAL_UART_CLEAR_OREFLAG(_huart);
        ++_rx_overrun_recoveries;
    }

    std::size_t received = 0U;
    while (received < len) {
        const HAL_StatusTypeDef status = HAL_UART_Receive(
            _huart,
            data + received,
            1U,
            timeout_ms);
        if (!update_status(status)) {
            return received;
        }
        ++received;
    }

    update_status(HAL_OK);
    return received;
}

bool UART_STM::byte_available() const
{
    return (__HAL_UART_GET_FLAG(_huart, UART_FLAG_RXNE) != RESET);
}

void UART_STM::delay_ms(int ms)
{
    if (ms > 0) {
        HAL_Delay(static_cast<std::uint32_t>(ms));
    }
}

std::uint32_t UART_STM::last_status() const
{
    return _last_status;
}

std::uint32_t UART_STM::last_error() const
{
    return _last_error;
}

std::uint32_t UART_STM::rx_overrun_recoveries() const
{
    return _rx_overrun_recoveries;
}

#endif
