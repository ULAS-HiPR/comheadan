#include <SPI/SPI_STM.h>

SPI_STM::SPI_STM(SPI_HandleTypeDef* hspi, GPIO_TypeDef* cs_port, uint16_t cs_pin) : _hspi(hspi), _cs_port(cs_port), _cs_pin(cs_pin)
{
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_SET);
}

void SPI_STM::cs_select(int)
{
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_RESET);
}

void SPI_STM::cs_deselect(int)
{
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_SET);
}

void SPI_STM::write(int cs, uint8_t reg, uint8_t *buf, uint16_t len)
{
    cs_select(0);
    HAL_SPI_Transmit(_hspi, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(_hspi, buf, len, HAL_MAX_DELAY);
    cs_deselect(0);
}


void SPI_STM::read(int cs, uint8_t reg, uint8_t* buf, uint16_t len)
{
    cs_select(0);
    uint8_t addr = reg | 0x80; // read bit
    HAL_SPI_Transmit(_hspi, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(_hspi, buf, len, HAL_MAX_DELAY);

    cs_deselect(0);
}

void SPI_STM::read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t addr = reg | 0x80;
    HAL_SPI_Transmit(_hspi, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(_hspi, buf, len, HAL_MAX_DELAY);
}


void SPI_STM::write_no_cs(uint8_t reg, const uint8_t *buf, uint16_t len)
{
    HAL_SPI_Transmit(_hspi, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(_hspi, (uint8_t*)buf, len, HAL_MAX_DELAY);
}


void SPI_STM::delay_ms(int ms)
{
    HAL_Delay(ms);
}