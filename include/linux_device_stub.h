/* Stub CMSIS device header for Linux/native builds.
 * Provides the minimal symbols that FreeRTOS and CMSIS-RTOS v2 expect
 * from a device header, without any real hardware definitions.
 */
#pragma once
#ifdef LINUX

#include <stdint.h>

/* FreeRTOS expects SystemCoreClock to exist on Cortex-M but we don't use it on Linux. */
static inline uint32_t SystemCoreClock_unused(void) { return 0; }

/* Stub out any NVIC / SCB / SysTick references that may leak through headers. */
#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS 4
#endif

#endif /* LINUX */
