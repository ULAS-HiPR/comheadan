#pragma once
#ifdef LINUX

#include "SPI/SPI_Handler.h"
#include <unistd.h>

/*
 * SPI_Mock — no-op SPI implementation for Linux sim builds.
 */
class SPI_Mock : public SPI_Handler {
public:
    bool read(int /*cs*/, uint8_t /*reg*/, uint8_t * buf, uint16_t len) override {
        if (buf) for (uint16_t i = 0; i < len; i++) buf[i] = 0;
        return true;
    }
    bool read_no_cs(uint8_t /*reg*/, uint8_t * buf, uint16_t len) override {
        if (buf) for (uint16_t i = 0; i < len; i++) buf[i] = 0;
        return true;
    }
    bool write(int /*cs*/, uint8_t /*reg*/, uint8_t * /*buf*/, uint16_t /*len*/) override {
        return true;
    }
    bool write_no_cs(uint8_t /*reg*/, uint8_t const * /*buf*/, uint16_t /*len*/) override {
        return true;
    }
    bool transmit(const uint8_t * /*data*/, std::size_t /*len*/) override { return true; }
    bool receive(uint8_t * buf, std::size_t len) override {
        if (buf) for (std::size_t i = 0; i < len; i++) buf[i] = 0;
        return true;
    }
    bool transfer(const uint8_t * /*tx*/, uint8_t * rx, std::size_t len) override {
        if (rx) for (std::size_t i = 0; i < len; i++) rx[i] = 0;
        return true;
    }
    void cs_low() override {}
    void cs_high() override {}
    void cs_select(int /*cs*/) override {}
    void cs_deselect(int /*cs*/) override {}
    void delay_ms(int ms) override {
        usleep(static_cast<useconds_t>(ms) * 1000U);
    }
};

#endif /* LINUX */
