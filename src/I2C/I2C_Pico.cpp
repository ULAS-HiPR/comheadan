#ifdef I2C_PICO_H

#include <I2C/I2C_Pico.h>

void I2C_Pico::init() {
    Wire.begin();
}

bool I2C_Pico::write(int addr, uint8_t* data, std::size_t len) {
    Wire.beginTransmission(addr);
    for (std::size_t i = 0; i < len; ++i) {
        Wire.write(data[i]);
    }
    return Wire.endTransmission() == 0;
}

bool I2C_Pico::read(int addr, uint8_t data, uint8_t* buf, std::size_t len) {
    Wire.beginTransmission(addr);
    Wire.write(data);
    if (Wire.endTransmission(false) != 0) {
        return false;
    }
    
    if (Wire.requestFrom(addr, len) != len) {
        return false;
    }

    for (std::size_t i = 0; i < len; ++i) {
        if (Wire.available()) {
            buf[i] = Wire.read();
        } else {
            return false;
        }
    }
    return true;
}

bool I2C_Pico::is_ready(int addr, uint32_t, uint32_t) {
    Wire.beginTransmission(addr);
    return Wire.endTransmission() == 0;
}

#endif
