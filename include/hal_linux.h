/*
 * hal_linux.h — STM32 HAL stubs for Linux/native builds.
 *
 * Provides the minimal set of HAL functions that firmware code calls
 * at startup and in timing paths. All peripheral HAL (I2C, SPI, CAN)
 * is handled by the Mock/Linux concrete classes, not here.
 *
 * Include this file from src/platform/linux.cpp in each board repo.
 */
#pragma once
#ifdef LINUX

#include <cstdint>
#include <unistd.h>
#include <chrono>

/* ---- Tick / delay ---- */

inline uint32_t HAL_GetTick() {
    static const auto t0 = std::chrono::steady_clock::now();
    return static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - t0).count());
}

inline void HAL_Delay(uint32_t ms) {
    usleep(static_cast<useconds_t>(ms) * 1000U);
}

/* ---- Startup stubs ---- */

inline void HAL_Init() {}
inline void SystemClock_Config() {}
inline void Error_Handler() { while (1) {} }

/* ---- Timer callback stub (called by STM32 HAL from SysTick IRQ) ---- */
inline void HAL_TIM_PeriodElapsedCallback(void * /*htim*/) {}

#endif /* LINUX */
