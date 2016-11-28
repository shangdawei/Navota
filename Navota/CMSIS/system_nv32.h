/*
** ###################################################################
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef __SYSTEM_NV32_H_
#define __SYSTEM_NV32_H_                        /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
  CLOCK_CORE = 0,   // Core and System Clock = ICSOUT = REFCLK  / ICS.C2.BDIV
  CLOCK_BUS,        // Bus and Flash Clock = ICSOUT / SIM.BUSDIV.BUSDIV
  CLOCK_OUT,        // Bus Output Clock = ICSOUT / SIM.BUSDIV.BUSDIV / SIM.SOPT.BUSREF
  CLOCK_ETM,        // ETM Clock = ICSOUT / ETM.SC.PS
  CLOCK_RTC,        // RTC and WDOG Clock
  CLOCK_ADC,        // ADC Clock
} ClockType_TypeDef;

/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit (void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate (void);

uint32_t SystemClockGet( ClockType_TypeDef ClockType );

#ifdef __cplusplus
}
#endif

#endif  /* #if !defined(__SYSTEM_NV32_H_) */
