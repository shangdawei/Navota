#include "NV32.h"
#include "NV32_rtc.h"
#include "NV32_gpio.h"

void RTC_IRQHandler( void )
{
  RTC_ClrFlags();
  GPIO_Toggle(GPIOB, GPIO_PTE7_MASK);
  GPIO_Toggle(GPIOB, GPIO_PTH1_MASK);
  GPIO_Toggle(GPIOB, GPIO_PTH2_MASK);
}

void RTC_Setup( void )
{
  RTC_ConfigType  sRTCConfig;
  RTC_ConfigType  *pRTCConfig = &sRTCConfig;

  /* configure RTC to 1Hz interrupt frequency */
  pRTCConfig->u16ModuloValue = 9;
  pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
  pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
  pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/
  RTC_Init(pRTCConfig);
}

void GPIO_Setup( void )
{
  GPIO_Init(GPIOB, GPIO_PTE7_MASK, GPIO_PinOutput);
  GPIO_Init(GPIOB, GPIO_PTH1_MASK, GPIO_PinOutput);
  GPIO_Init(GPIOB, GPIO_PTH2_MASK, GPIO_PinOutput);
}

int main()
{
  GPIO_Setup();
  RTC_Setup();

  while (1)
  {
  }
}
