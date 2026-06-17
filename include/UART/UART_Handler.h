#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include <cstddef>
#include <cstdint>

class UART_Handler {
public:
    virtual bool write(const std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms = 100U) = 0;
    virtual std::size_t read(std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms = 0U) = 0;
    virtual bool byte_available() const = 0;
    virtual void delay_ms(int ms) {}
    virtual std::uint32_t last_status() const { return 0; }
    virtual std::uint32_t last_error() const { return 0; }
    virtual ~UART_Handler() = default;
};

#endif
