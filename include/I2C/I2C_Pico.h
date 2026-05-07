#ifdef I2C_PICO_H

#include "I2C_Handler.h"
#include <Arduino.h>
#include <Wire.h>

class I2C_Pico : public I2C_Handler {
    public:
        explicit I2C_Pico(uint8_t address) : addr(address){}

        void init();
        bool write(int addr, uint8_t* data, std::size_t len) override ;
        bool read(int addr, uint8_t data, uint8_t* buf, std::size_t len) override ;
        bool is_ready(int addr, uint32_t trials = 2, uint32_t timeout_ms = 10) override;

    private:
        uint8_t addr;
         
};

#endif // I2C_PICO_H
