#ifndef SPI_STM_H
#define SPI_STM_H
#include "SPI_Handler.h"
#include <SPI.h>
#include <cstdio>

class SPI_STM : public SPI_Handler {
    public:
        explicit SPI_STM(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs) : sck(sck), mosi(mosi), miso(miso), cs(cs) {
            printf("making spi -> i hate ardino and dont wanna write actual impl");
        }

        void write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) override ;
        void read(int cs, uint8_t reg, uint8_t* buf, uint16_t len) override ;
        //used for BMP library which handles CS internally
        void read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) override;
        void write_no_cs(uint8_t reg, uint8_t const *buf, uint16_t len) override;

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