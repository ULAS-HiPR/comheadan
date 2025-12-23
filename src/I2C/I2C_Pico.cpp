#include <I2C/I2C_Pico.h>

void I2C_Pico::init() {
    Wire.begin();
}

void I2C_Pico::write(int addr, uint8_t* data, uint len) {
    Wire.beginTransmission(addr);
    for (uint i = 0; i < len; ++i) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}

void I2C_Pico::read(int addr, uint8_t data, uint8_t* buf, uint len) {
    Wire.beginTransmission(addr);
    Wire.write(data);
    Wire.endTransmission();
    
    Wire.requestFrom(addr, len);
    for (uint i = 0; i < len; ++i) {
        if (Wire.available()) {
            buf[i] = Wire.read();
        }
    }
}

void I2C_Pico::updatePins(pin_size_t sda, pin_size_t scl) {
    Wire.setSDA(sda);
    Wire.setSCL(scl);
}
