#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H
#include <cstdint>
#include <cstddef>

class I2C_Handler {
    public:
        virtual bool write(int addr, uint8_t* data, std::size_t len) = 0;
        virtual bool read(int addr, uint8_t data, uint8_t* buf, std::size_t len) = 0;
        virtual bool is_ready(int addr, uint32_t trials = 2, uint32_t timeout_ms = 10) = 0;
        virtual void delay_ms(int ms) {};
        virtual uint32_t last_status() const { return 0; }
        virtual uint32_t last_error() const { return 0; }
        virtual ~I2C_Handler() = default;
};

#endif // I2C_HANDLER_H
