#ifdef SPI_PICO_H

#include "SPI_Handler.h"
#include <Arduino.h>
#include <SPI.h>
#include <cstdio>

class SPI_Pico : public SPI_Handler {
    public:
        explicit SPI_Pico(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs) : sck(sck), mosi(mosi), miso(miso), cs(cs) {
            printf("making spi -> i hate ardino and dont wanna write actual impl");
        }

        bool write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) override ;
        bool read(int cs, uint8_t reg, uint8_t* buf, uint16_t len) override ;
        //used for BMP library which handles CS internally
        bool read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) override;
        bool write_no_cs(uint8_t reg, uint8_t const *buf, uint16_t len) override;

        bool transmit(const uint8_t *data, std::size_t len) override;
        bool receive(uint8_t *buf, std::size_t len) override;
        bool transfer(const uint8_t *tx, uint8_t *rx, std::size_t len) override;
        void cs_low() override;
        void cs_high() override;

        void cs_select(int cs) override ;
        void cs_deselect(int cs) override ;
        void delay_ms(int ms) override ;

    private:
        uint8_t sck;
        uint8_t mosi;
        uint8_t miso;
        uint8_t cs;
         
};


#endif // SPI_PICO_H
