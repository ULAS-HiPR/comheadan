#ifndef I2C_STM_H
#define I2C_STM_H
#include "I2C_Handler.h"
#include "stm32f4xx_hal.h"


class I2C_STM : public I2C_Handler {
    public:
        explicit I2C_STM(I2C_HandleTypeDef* hi2c, uint8_t address) : addr(address){}

        void init();
        void write(int addr, uint8_t* data, uint len) override ;
        void read(int addr, uint8_t data, uint8_t* buf, uint len) override ;

    private:
        uint8_t addr;
        I2C_HandleTypeDef* _hi2c;
         
};

#endif // I2C_PICO_H