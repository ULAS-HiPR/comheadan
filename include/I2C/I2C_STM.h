#ifdef STM
#ifndef I2C_STM_H
#define I2C_STM_H
#include "I2C_Handler.h"
#ifdef F4
#include "stm32f4xx_hal.h"
#elif F0
#include "stm32f0xx_hal.h"
#endif


class I2C_STM : public I2C_Handler {
    public:
        explicit I2C_STM(I2C_HandleTypeDef* hi2c, uint8_t address) : _hi2c(hi2c),addr(address){}

        void init();
        bool write(int addr, uint8_t* data, std::size_t len) override ;
        bool read(int addr, uint8_t data, uint8_t* buf, std::size_t len) override ;
        bool is_ready(int addr, uint32_t trials = 2, uint32_t timeout_ms = 10) override;
        void delay_ms(int ms) override;
        uint32_t last_status() const override;
        uint32_t last_error() const override;

    private:
        bool update_status(HAL_StatusTypeDef status);

        I2C_HandleTypeDef* _hi2c;
        uint8_t addr;
        uint32_t _last_status{0};
        uint32_t _last_error{0};
        
         
};

#endif 
#endif
