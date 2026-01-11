#include <SPI/SPI_Pico.h>

void SPI_Pico::write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) {
    printf("spi write called -> hate arduino and dont wanna write actual impl\n");
}

void SPI_Pico::read(int cs, uint8_t reg, uint8_t* buf, uint16_t len) {
    printf("spi read called -> hate arduino and dont wanna write actual impl\n");
}

void SPI_Pico::read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) {
    printf("spi read no cs called -> hate arduino and dont wanna write actual impl\n");
}

void SPI_Pico::write_no_cs(uint8_t* reg, uint8_t const *buf, uint16_t len) {
    printf("spi write no cs called -> hate arduino and dont wanna write actual impl\n");
}

void SPI_Pico::cs_select(int cs) {
    printf("spi cs select called -> hate arduino and dont wanna write actual impl\n");
}

void SPI_Pico::cs_deselect(int cs) {
    printf("spi cs deselect called -> hate arduino and dont wanna write actual impl\n");
}

void SPI_Pico::delay_ms(int ms) {
    delay(ms);
}