#ifdef STM
#ifndef UART_STM_H
#define UART_STM_H

#include "UART_Handler.h"

#ifdef F4
#include "stm32f4xx_hal.h"
#elif F0
#include "stm32f0xx_hal.h"
#endif

class UART_STM : public UART_Handler {
public:
    explicit UART_STM(UART_HandleTypeDef* huart);

    bool write(const std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms = 100U) override;
    std::size_t read(std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms = 0U) override;
    bool byte_available() const override;
    void delay_ms(int ms) override;
    std::uint32_t last_status() const override;
    std::uint32_t last_error() const override;

private:
    bool update_status(HAL_StatusTypeDef status);

    UART_HandleTypeDef* _huart;
    std::uint32_t _last_status;
    std::uint32_t _last_error;
};

#endif
#endif
