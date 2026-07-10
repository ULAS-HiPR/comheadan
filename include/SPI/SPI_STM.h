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

        bool write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) override;

        bool read(int cs, uint8_t reg, uint8_t* buf, uint16_t len) override;

        bool read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) override;

        bool write_no_cs(uint8_t reg, const uint8_t *buf, uint16_t len) override;

        bool transmit(const uint8_t *data, std::size_t len) override;

        bool receive(uint8_t *buf, std::size_t len) override;

        bool transfer(const uint8_t *tx, uint8_t *rx, std::size_t len) override;

        void cs_low() override;

        void cs_high() override;

        void cs_select(int cs) override;

        void cs_deselect(int cs) override;

        void delay_ms(int ms) override;

        uint32_t last_status() const override;

        uint32_t last_error() const override;

    private:
        bool update_status(HAL_StatusTypeDef status);

        SPI_HandleTypeDef* _hspi;
        GPIO_TypeDef* _cs_port;
        uint16_t _cs_pin;
        uint32_t _last_status;
        uint32_t _last_error;
        bool _bus_mutex_acquired;
        bool _bus_lock_failed;
};


#endif 
#endif
