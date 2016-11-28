#include "system_NV32.h"
#include "NV32.h"
#include "NV32_config.h"
#include "NV32_wdog.h"
#include "NV32_ics.h"
#include "NV32_sim.h"

#pragma location = "NV"
__root const NV_Type NV = {
  NV_BACKKEY, 0xFF, 0xFF, 0xFF, 0xFF, NV_EEPROT, NV_FPROT, NV_FSEC, NV_FOPT };

/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
uint32_t SystemCoreClock = SYSTEM_CORE_CLOCK;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit( void )
{
  extern uint32_t __vector_table;
  SCB->VTOR = (uint32_t) &__vector_table;

  SIM_ConfigType sSIMConfig = {
    {
      0 }, 0 };
  ICS_ConfigType sICSConfig = {
    0 };

#if ( WDOG_ENABLED > 0 )
  /* Disable the watchdog ETMer but enable update */
  WDOG_DisableWDOGEnableUpdate( );
#else
  /* Disable the watchdog ETMer */
  WDOG_Disable();
#endif

  EFMCR &= 0xFFFF0001;  // set wait state 1

  /*Enable SWD/RESET/NMI/BUSCLKOUT pins, SPI0 Remap */

#if (NMI_PIN_ENABLED == 0)
  sSIMConfig.sBits.bDisableNMI = 1;
#endif

#if (RST_PIN_ENABLED == 0)
  sSIMConfig.sBits.bDisableRESET = 1;
#endif

#if (SPI0_REMAPPED > 0 )
  sSIMConfig.u32PinSel |= SIM_PINSEL_SPI0PS_MASK;
#endif

#if (BUSCLK_OUTPUT_ENABLED > 0)
  sSIMConfig.sBits.bEnableCLKOUT = 1; /* output bus clock if enabled */
  sSIMConfig.u8BusRef = BUSCLK_OUTPUT_REF;
#endif

  /* make sure clocks to peripheral modules are enabled */
  sSIMConfig.u32SCGC |= SIM_SCGC_SWD_MASK | SIM_SCGC_FLASH_MASK;

#if (BUSCLK_DIV_BY_2 > 0)
  /* bus clock divided by 2 */
  sSIMConfig.sBits.bBusDiv = 1;
#endif

  SIM_Init( &sSIMConfig ); /* initialize SIM */

  /* initialize ICS to the default state : FEI @ 48 MHZ */
  ICS_DeInit( );

#if (ICS_TRIM_ENABLED > 0 )
  /* if not trimmed, do trim first */
  ICS_Trim( ICS_TRIM_VALUE );
#endif

#if  (EXTAL_CLK_FREQ_KHZ == 0)              /* Uses IRC */

  /* reference clock frequency in KHz; use value 32 for 31.25KHz to 39.0625KHz */
  sICSConfig.u32ClkFreq = 32;

#if ( FLL_BYPASSED == 0 )
  sICSConfig.u8ClkMode = ICS_CLK_MODE_FEI; /* ICS to the default state */

#elif ( FLL_ENABLED > 0 )
  sICSConfig.u8ClkMode = ICS_CLK_MODE_FBI;

#else
  sICSConfig.u8ClkMode = ICS_CLK_MODE_FBILP;

#endif

#else // (EXTAL_CLK_FREQ_KHZ != 0)          /* Uses OSC */

  /* reference clock frequency in KHz; use value 32 for 31.25KHz to 39.0625KHz */
  sICSConfig.u32ClkFreq = EXTAL_CLK_FREQ_KHZ;
  sICSConfig.oscConfig.bEnable = 1; /* enable OSC */

#if (OSC_STOP_ENABLED > 0 )
  sICSConfig.oscConfig.bStopEnable = 1; /* enabled in stop mode */
#endif

#if (OSC_HIGH_GRAIN > 0)
  sICSConfig.oscConfig.bGain = 1; /* high gain */
#endif

#if  (EXTAL_CLK_FREQ_KHZ >= 4000)
  sICSConfig.oscConfig.bRange = 1; /* high range */
#endif

#if ( FLL_BYPASSED == 0 )

#if ( EXTAL_CLK_FREQ_KHZ != XTAL_CLK_FREQ_KHZ )
  sICSConfig.u8ClkMode = ICS_CLK_MODE_FEE_OSC; /* external active oscillator */
#else
  sICSConfig.u8ClkMode = ICS_CLK_MODE_FEE; /* external passive crystal */
#endif

#else // ( FLL_BYPASSED != 0 )

#if ( FLL_ENABLED == 0 )
  sICSConfig.u8ClkMode = ICS_CLK_MODE_FBELP;
#elif ( EXTAL_CLK_FREQ_KHZ != XTAL_CLK_FREQ_KHZ )
  sICSConfig.u8ClkMode = ICS_CLK_MODE_FBE_OSC; /* external active oscillator */
#else
  sICSConfig.u8ClkMode = ICS_CLK_MODE_FBE; /* external passive crystal */
#endif

#endif

#endif

  ICS_Init( &sICSConfig ); /* initialize ICS */

  SystemCoreClockUpdate( );
}

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate( void )
{
  uint8_t CLKS = ICS->C1 & ICS_C1_CLKS_MASK;
  uint8_t IREFS = ICS->C1 & ICS_C1_IREFS_MASK;
  uint8_t RDIV = ( ICS->C1 & ICS_C1_RDIV_MASK ) >> ICS_C1_RDIV_SHIFT;  // 0 - 32, 1 - 64, 6 - 2048
  uint8_t BDIV = ( ICS->C2 & ICS_C2_BDIV_MASK ) >> ICS_C2_BDIV_SHIFT;  // 0 - 1, 1 - 2, 7 - 128
  uint32_t RDIV_Val = 32;

  SystemCoreClock = EXTAL_CLK_FREQ_KHZ;  // 外部时钟源
  if ( SystemCoreClock == 0 )
    SystemCoreClock = 32768;  // 内部时钟源

  if ( CLKS == ICS_C1_CLKS( 0 ) )  // FLL 的输出
  {
    if ( IREFS )  // 选择内部参考时钟源
    {
      SystemCoreClock = 48000000;  // 37500 * 1280
    }
    else  // 选择外部参考时钟源
    {
      for ( uint32_t i = 0; i < RDIV; i++ )
        RDIV_Val <<= 1;

      SystemCoreClock = EXTAL_CLK_FREQ_KHZ * 1000 * 10;
      SystemCoreClock /= RDIV_Val;
      SystemCoreClock *= 128;
    }
  }

  for ( uint32_t i = 0; i < BDIV; i++ )
    SystemCoreClock >>= 1;
}

uint32_t SystemClockGet( ClockType_TypeDef ClockType )
{
  uint32_t Result = 0;
  uint8_t BUSREF = ( SIM->SOPT & SIM_SOPT_BUSREF_MASK ) >> SIM_SOPT_BUSREF_SHIFT;  // 0 - 1, 1 - 2, 7 - 128

  switch ( ClockType )
  {
    case CLOCK_CORE:
      Result = SystemCoreClock;
      break;

    case CLOCK_BUS:
      Result = SystemCoreClock;
      if ( SIM->BUSDIV )
        Result >>= 1;
      break;

    case CLOCK_OUT:  // 0 - 1, 1 - 2, 7 - 128
      Result = SystemClockGet( CLOCK_BUS );
      for ( uint32_t i = 0; i < BUSREF; i++ )
        Result >>= 1;
      break;
  }

  return Result;
}

