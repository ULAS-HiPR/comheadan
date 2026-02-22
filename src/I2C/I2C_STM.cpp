#include <I2C/I2C_STM.h>

void I2C_STM::init() {
    // Initialization code for STM32 I2C (if needed)
}

void I2C_STM::write(int addr, uint8_t* data, std::size_t len) {
    HAL_I2C_Master_Transmit(_hi2c, addr << 1, data, len, HAL_MAX_DELAY);
}

void I2C_STM::read(int addr, uint8_t data, uint8_t* buf, std::size_t len) {
    HAL_I2C_Master_Transmit(_hi2c, addr << 1, &data, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(_hi2c, addr << 1, buf, len, HAL_MAX_DELAY);
}

