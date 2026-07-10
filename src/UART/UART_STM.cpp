#include <UART/UART_STM.h>

#ifdef STM

UART_STM::UART_STM(UART_HandleTypeDef* huart)
    : _huart(huart), _last_status(HAL_OK), _last_error(0)
{
}

bool UART_STM::update_status(HAL_StatusTypeDef status)
{
    _last_status = static_cast<std::uint32_t>(status);
    _last_error = HAL_UART_GetError(_huart);
    return status == HAL_OK;
}

bool UART_STM::write(const std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms)
{
    if ((data == nullptr) && (len > 0U)) {
        _last_status = HAL_ERROR;
        _last_error = HAL_UART_ERROR_PE;
        return false;
    }

    while (len > 0U) {
        std::uint16_t chunk = len > UINT16_MAX ? UINT16_MAX : static_cast<std::uint16_t>(len);
        if (!update_status(HAL_UART_Transmit(_huart, const_cast<std::uint8_t*>(data), chunk, timeout_ms))) {
            return false;
        }
        data += chunk;
        len -= chunk;
    }

    return true;
}

std::size_t UART_STM::read(std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms)
{
    if ((data == nullptr) && (len > 0U)) {
        _last_status = HAL_ERROR;
        _last_error = HAL_UART_ERROR_PE;
        return 0U;
    }

    std::size_t received = 0U;
    USART_TypeDef* usart = _huart->Instance;

    while (received < len) {
        // Wait for RXNE
        if (!(usart->ISR & USART_ISR_RXNE)) {
            if (timeout_ms == 0U) {
                break;
            }
            uint32_t start = HAL_GetTick();
            while (!(usart->ISR & USART_ISR_RXNE)) {
                if ((HAL_GetTick() - start) >= timeout_ms) {
                    _last_status = HAL_TIMEOUT;
                    return received;
                }
            }
        }

        // Read byte FIRST — this clears RXNE naturally
        data[received++] = static_cast<uint8_t>(usart->RDR & 0xFFU);

        // NOW safe to clear any error flags — byte already consumed
        const uint32_t error_flags =
            usart->ISR & (USART_ISR_ORE | USART_ISR_FE | USART_ISR_NE | USART_ISR_PE);
        if (error_flags != 0U) {
            if ((error_flags & USART_ISR_ORE) != 0U) _last_error |= HAL_UART_ERROR_ORE;
            if ((error_flags & USART_ISR_FE) != 0U) _last_error |= HAL_UART_ERROR_FE;
            if ((error_flags & USART_ISR_NE) != 0U) _last_error |= HAL_UART_ERROR_NE;
            if ((error_flags & USART_ISR_PE) != 0U) _last_error |= HAL_UART_ERROR_PE;
            usart->ICR = USART_ICR_ORECF | USART_ICR_FECF |
                         USART_ICR_NCF | USART_ICR_PECF;
        }
    }

    _last_status = HAL_OK;
    return received;
}

bool UART_STM::byte_available() const
{
    return (__HAL_UART_GET_FLAG(_huart, UART_FLAG_RXNE) != RESET);
}

void UART_STM::delay_ms(int ms)
{
    if (ms > 0) {
        HAL_Delay(static_cast<std::uint32_t>(ms));
    }
}

std::uint32_t UART_STM::last_status() const
{
    return _last_status;
}

std::uint32_t UART_STM::last_error() const
{
    return _last_error;
}

#endif
