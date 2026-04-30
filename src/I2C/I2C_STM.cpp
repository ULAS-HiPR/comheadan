#include <I2C/I2C_STM.h>
#include <servo_debug.h>

void I2C_STM::init() {
    // Initialization code for STM32 I2C (if needed)
}

bool I2C_STM::update_status(HAL_StatusTypeDef status) {
    _last_status = static_cast<uint32_t>(status);
    _last_error = HAL_I2C_GetError(_hi2c);
    servo_debug.i2c_last_status = _last_status;
    servo_debug.i2c_last_error = _last_error;
    return status == HAL_OK;
}

bool I2C_STM::write(int addr, uint8_t* data, std::size_t len) {
    servo_debug.i2c_last_op = SERVO_DEBUG_I2C_OP_WRITE;
    servo_debug.i2c_last_address = static_cast<uint8_t>(addr);
    servo_debug.i2c_last_register = len > 0 ? data[0] : 0;
    servo_debug.i2c_last_length = static_cast<uint16_t>(len);
    servo_debug.i2c_write_count++;

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(_hi2c, addr << 1, data, len, HAL_MAX_DELAY);
    return update_status(status);
}

bool I2C_STM::read(int addr, uint8_t data, uint8_t* buf, std::size_t len) {
    servo_debug.i2c_last_op = SERVO_DEBUG_I2C_OP_READ_REG;
    servo_debug.i2c_last_address = static_cast<uint8_t>(addr);
    servo_debug.i2c_last_register = data;
    servo_debug.i2c_last_length = 1;
    servo_debug.i2c_read_count++;

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(_hi2c, addr << 1, &data, 1, HAL_MAX_DELAY);
    if (!update_status(status)) {
        return false;
    }

    servo_debug.i2c_last_op = SERVO_DEBUG_I2C_OP_READ_DATA;
    servo_debug.i2c_last_length = static_cast<uint16_t>(len);
    status = HAL_I2C_Master_Receive(_hi2c, addr << 1, buf, len, HAL_MAX_DELAY);
    return update_status(status);
}

bool I2C_STM::is_ready(int addr, uint32_t trials, uint32_t timeout_ms) {
    servo_debug.i2c_last_op = SERVO_DEBUG_I2C_OP_READY;
    servo_debug.i2c_last_address = static_cast<uint8_t>(addr);

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
