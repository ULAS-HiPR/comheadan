#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H
#include <cstdint>
#include <cstddef>

class I2C_Handler {
    public:
        virtual void write(int addr, uint8_t* data, std::size_t len) = 0;
        virtual void read(int addr, uint8_t data, uint8_t* buf, std::size_t len) = 0;
        virtual void delay_ms(int ms) {};
        virtual ~I2C_Handler() = default;
};

#endif // I2C_HANDLER_H