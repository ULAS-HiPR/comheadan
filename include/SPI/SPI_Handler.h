#ifndef SPI_HANDLER_H
#define SPI_HANDLER_H
#include <cstdint>
#include <cstddef>

class SPI_Handler{
    public:
        // Register-style helpers for sensors that use MSB-as-read-bit SPI.
        virtual bool read(int cs, uint8_t reg, uint8_t *buf, uint16_t len) = 0;
        virtual bool read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) = 0;
        virtual bool write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) = 0;
        virtual bool write_no_cs(uint8_t reg, uint8_t const *buf, uint16_t len) = 0;

        // Raw stream helpers for flash/SD-style command protocols.
        virtual bool transmit(const uint8_t *data, std::size_t len) = 0;
        virtual bool receive(uint8_t *buf, std::size_t len) = 0;
        virtual bool transfer(const uint8_t *tx, uint8_t *rx, std::size_t len) = 0;
        virtual void cs_low() = 0;
        virtual void cs_high() = 0;

        virtual void cs_select(int cs) = 0;
        virtual void cs_deselect(int cs) = 0;
        virtual void delay_ms(int ms) = 0;
        virtual uint32_t last_status() const { return 0; }
        virtual uint32_t last_error() const { return 0; }
        virtual ~SPI_Handler() = default;
};

#endif // SPI_HANDLER_H
