#ifdef SPI_PICO_H
#include <SPI/SPI_Pico.h>

bool SPI_Pico::write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) {
    printf("spi write called -> hate arduino and dont wanna write actual impl\n");
    return false;
}

bool SPI_Pico::read(int cs, uint8_t reg, uint8_t* buf, uint16_t len) {
    printf("spi read called -> hate arduino and dont wanna write actual impl\n");
    return false;
}

bool SPI_Pico::read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) {
    printf("spi read no cs called -> hate arduino and dont wanna write actual impl\n");
    return false;
}

bool SPI_Pico::write_no_cs(uint8_t reg, uint8_t const *buf, uint16_t len) {
    printf("spi write no cs called -> hate arduino and dont wanna write actual impl\n");
    return false;
}

bool SPI_Pico::transmit(const uint8_t *data, std::size_t len) {
    printf("spi transmit called -> hate arduino and dont wanna write actual impl\n");
    return false;
}

bool SPI_Pico::receive(uint8_t *buf, std::size_t len) {
    printf("spi receive called -> hate arduino and dont wanna write actual impl\n");
    return false;
}

bool SPI_Pico::transfer(const uint8_t *tx, uint8_t *rx, std::size_t len) {
    printf("spi transfer called -> hate arduino and dont wanna write actual impl\n");
    return false;
}

void SPI_Pico::cs_low() {
    cs_select(cs);
}

void SPI_Pico::cs_high() {
    cs_deselect(cs);
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

#endif
