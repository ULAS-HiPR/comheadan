#ifndef I2C_PICO_H
#define I2C_PICO_H
#include "I2C_Handler.h"
#include <Arduino.h>
#include <Wire.h>

class I2C_Pico : public I2C_Handler {
    public:
        explicit I2C_Pico(uint8_t address) : addr(address){}

        void init();
        void write(int addr, uint8_t* data, uint len) override ;
        void read(int addr, uint8_t data, uint8_t* buf, uint len) override ;
        void updatePins(pin_size_t sda, pin_size_t scl) ;
        
    private:
        uint8_t addr;
         
};

#endif // I2C_PICO_H