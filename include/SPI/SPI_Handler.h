#ifndef SPI_HANDLER_H
#define SPI_HANDLER_H
#include <cstdint>
#include <cstddef>

class SPI_Handler{
    public:
        virtual void read(int cs, uint8_t reg, uint8_t *buf, uint16_t len) = 0;
        //used for BMP library which handles CS internally
        virtual void read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) = 0;
        virtual void write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) = 0;
        //used for BMP library which handles CS internally
        virtual void write_no_cs(uint8_t reg, uint8_t const *buf, uint16_t len) = 0;
        virtual void cs_select(int cs) = 0;
        virtual void cs_deselect(int cs) = 0;
        virtual void delay_ms(int ms) = 0;
        virtual ~SPI_Handler() = default;
};

#endif // SPI_HANDLER_H