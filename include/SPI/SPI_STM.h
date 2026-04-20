#ifdef STM
#ifndef SPI_STM_H
#define SPI_STM_H
#include "SPI_Handler.h"
#ifdef F4
#include "stm32f4xx_hal.h"
#elif F0
#include "stm32f0xx_hal.h"
#endif
#include <cstdio>

class SPI_STM : public SPI_Handler {
    public:
        
        SPI_STM(SPI_HandleTypeDef* hspi, GPIO_TypeDef* cs_port, uint16_t cs_pin);

        void write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) override;

        void read(int cs, uint8_t reg, uint8_t* buf, uint16_t len) override;

        void read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) override;

        void write_no_cs(uint8_t reg, const uint8_t *buf, uint16_t len) override;

        void cs_select(int cs) override;

        void cs_deselect(int cs) override;

        void delay_ms(int ms) override;

    private:
        SPI_HandleTypeDef* _hspi;
        GPIO_TypeDef* _cs_port;
        uint16_t _cs_pin;

         
};


#endif 
#endif