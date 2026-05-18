#pragma once
#ifdef LINUX

#include "I2C/I2C_Handler.h"
#include <unistd.h>

/*
 * I2C_Mock — no-op I2C implementation for Linux sim builds.
 * Sensor drivers require an I2C_Handler; this satisfies the interface
 * without touching real hardware. All reads return zeroed buffers.
 */
class I2C_Mock : public I2C_Handler {
public:
    bool write(int /*addr*/, uint8_t * /*data*/, std::size_t /*len*/) override {
        return true;
    }
    bool read(int /*addr*/, uint8_t /*reg*/, uint8_t * buf, std::size_t len) override {
        if (buf) for (std::size_t i = 0; i < len; i++) buf[i] = 0;
        return true;
    }
    bool is_ready(int /*addr*/, uint32_t /*trials*/, uint32_t /*timeout_ms*/) override {
        return true;
    }
    void delay_ms(int ms) override {
        usleep(static_cast<useconds_t>(ms) * 1000U);
    }
};

#endif /* LINUX */
