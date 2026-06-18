#include <UART/UART_STM.h>

#ifdef STM

namespace {

std::uint32_t clear_uart_error_flags(UART_HandleTypeDef* huart)
{
    std::uint32_t error = 0U;

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        __HAL_UART_CLEAR_OREFLAG(huart);
        error |= HAL_UART_ERROR_ORE;
    }
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
        __HAL_UART_CLEAR_FEFLAG(huart);
        error |= HAL_UART_ERROR_FE;
    }
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET) {
        __HAL_UART_CLEAR_NEFLAG(huart);
        error |= HAL_UART_ERROR_NE;
    }
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
        __HAL_UART_CLEAR_PEFLAG(huart);
        error |= HAL_UART_ERROR_PE;
    }

    return error;
}

} // namespace

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

    while (received < len) {
        std::uint32_t cleared_error = clear_uart_error_flags(_huart);
        if (cleared_error != 0U) {
            _last_status = HAL_ERROR;
            _last_error = cleared_error;
        }

        if ((timeout_ms == 0U) && !byte_available()) {
            break;
        }

        HAL_StatusTypeDef status = HAL_UART_Receive(
            _huart,
            &data[received],
            1U,
            timeout_ms
        );

        if (status == HAL_OK) {
            update_status(status);
            received++;
            continue;
        }

        if (status == HAL_TIMEOUT) {
            update_status(status);
            break;
        }

        update_status(status);
        break;
    }

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
