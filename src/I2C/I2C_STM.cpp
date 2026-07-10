#include <I2C/I2C_STM.h>

namespace {
constexpr uint32_t kI2cTransferTimeoutMs = 100U;
}

#ifdef COMHEADAN_USE_SERVO_DEBUG
#include <servo_debug.h>
#endif

namespace {

void debug_status(uint32_t status, uint32_t error) {
#ifdef COMHEADAN_USE_SERVO_DEBUG
    servo_debug.i2c_last_status = status;
    servo_debug.i2c_last_error = error;
#else
    (void)status;
    (void)error;
#endif
}

void debug_write(int addr, uint8_t reg, std::size_t len) {
#ifdef COMHEADAN_USE_SERVO_DEBUG
    servo_debug.i2c_last_op = SERVO_DEBUG_I2C_OP_WRITE;
    servo_debug.i2c_last_address = static_cast<uint8_t>(addr);
    servo_debug.i2c_last_register = reg;
    servo_debug.i2c_last_length = static_cast<uint16_t>(len);
    servo_debug.i2c_write_count++;
#else
    (void)addr;
    (void)reg;
    (void)len;
#endif
}

void debug_read_reg(int addr, uint8_t reg) {
#ifdef COMHEADAN_USE_SERVO_DEBUG
    servo_debug.i2c_last_op = SERVO_DEBUG_I2C_OP_READ_REG;
    servo_debug.i2c_last_address = static_cast<uint8_t>(addr);
    servo_debug.i2c_last_register = reg;
    servo_debug.i2c_last_length = 1;
    servo_debug.i2c_read_count++;
#else
    (void)addr;
    (void)reg;
#endif
}

void debug_read_data(std::size_t len) {
#ifdef COMHEADAN_USE_SERVO_DEBUG
    servo_debug.i2c_last_op = SERVO_DEBUG_I2C_OP_READ_DATA;
    servo_debug.i2c_last_length = static_cast<uint16_t>(len);
#else
    (void)len;
#endif
}

void debug_ready(int addr) {
#ifdef COMHEADAN_USE_SERVO_DEBUG
    servo_debug.i2c_last_op = SERVO_DEBUG_I2C_OP_READY;
    servo_debug.i2c_last_address = static_cast<uint8_t>(addr);
#else
    (void)addr;
#endif
}

} // namespace

void I2C_STM::init() {
    // Initialization code for STM32 I2C (if needed)
}

bool I2C_STM::update_status(HAL_StatusTypeDef status) {
    _last_status = static_cast<uint32_t>(status);
    _last_error = HAL_I2C_GetError(_hi2c);
    debug_status(_last_status, _last_error);
    return status == HAL_OK;
}

bool I2C_STM::write(int addr, uint8_t* data, std::size_t len) {
    debug_write(addr, (len > 0 && data != nullptr) ? data[0] : 0, len);

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(
        _hi2c, addr << 1, data, len, kI2cTransferTimeoutMs);
    return update_status(status);
}

bool I2C_STM::read(int addr, uint8_t data, uint8_t* buf, std::size_t len) {
    debug_read_reg(addr, data);

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(
        _hi2c, addr << 1, &data, 1, kI2cTransferTimeoutMs);
    if (!update_status(status)) {
        return false;
    }

    debug_read_data(len);
    status = HAL_I2C_Master_Receive(
        _hi2c, addr << 1, buf, len, kI2cTransferTimeoutMs);
    return update_status(status);
}

bool I2C_STM::is_ready(int addr, uint32_t trials, uint32_t timeout_ms) {
    debug_ready(addr);

    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(
        _hi2c,
        static_cast<uint16_t>(addr << 1),
        trials,
        timeout_ms
    );
    return update_status(status);
}

void I2C_STM::delay_ms(int ms) {
    if (ms > 0) {
        HAL_Delay(static_cast<uint32_t>(ms));
    }
}

uint32_t I2C_STM::last_status() const {
    return _last_status;
}

uint32_t I2C_STM::last_error() const {
    return _last_error;
}
