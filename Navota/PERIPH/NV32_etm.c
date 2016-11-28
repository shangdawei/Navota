
/******************************************************************************
* @brief providing APIs for configuring ETM.
*
*******************************************************************************
*
* provide APIs for configuring ETM
******************************************************************************/
#include "NV32_config.h"
#include "NV32_ETM.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/
ETM_CallbackPtr ETM_Callback[3] = {( ETM_CallbackPtr )NULL};


/******************************************************************************
* ETM api lists
*
*//*! @addtogroup ETM_api_list
* @{
*******************************************************************************/
/*******************************************************************************//*!
*
* @brief set the ETM moule clock source and prescale.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    ClockSource           ETM clock source.
* @param[in]    ClockPrescale         prescale factor.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_ClockSet( ETM_Type * pETM, uint8_t u8ClockSource, uint8_t u8ClockPrescale )
{
  uint8_t   u8Temp;
  u8Temp  = ( pETM->SC & 0xE0 );
  u8Temp |= ( ETM_SC_CLKS( u8ClockSource & 0x3 ) | ETM_SC_PS( u8ClockPrescale & 0x7 ) );
  pETM->SC = u8Temp;
}

/*********************************************************************************//*!
*
* @brief general configuration to ETM_No to high ture EPWM mode, clock soure is the
*        system clock, MOD, CnV are the initial value.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    PWMModeSelect         select CPWM , EPWM  or combine pwm mode.
* @param[in]    PWMEdgeSelect         select high true or low true pulse.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_PWMInit( ETM_Type * pETM, uint8_t u8PWMModeSelect, uint8_t u8PWMEdgeSelect )
{
  uint8_t   channels, i;
  ASSERT( ( ETM0 == pETM ) || ( ETM1 == pETM ) || ( ETM2 == pETM ) );

  /* open the clock gate */
  if ( ETM0 == pETM )
  {
    channels = 2;
    SIM->SCGC |= SIM_SCGC_ETM0_MASK;
  }
  else if ( ETM1 == pETM )
  {
    channels = 2;
#if !defined(CPU_NV32M3)
    SIM->SCGC |= SIM_SCGC_ETM1_MASK;
#endif
  }
  else
  {
    channels = 6;
    SIM->SCGC  |= SIM_SCGC_ETM2_MASK;
  }

  pETM->SC  = 0x0;                                    /* disable counter */
  pETM->MOD = ETM_MOD_INIT;

  if ( ETM_PWMMODE_CENTERALLIGNED == u8PWMModeSelect ) /* enable CPWM */
  {
    pETM->SC |= ETM_SC_CPWMS_MASK;
  }
  else if ( ETM_PWMMODE_COMBINE == u8PWMModeSelect )  /* enable combine pwm mode */
  {
    ASSERT( ETM2 == pETM );
    pETM->MODE    |= ETM_MODE_WPDIS_MASK | ETM_MODE_ETMEN_MASK;
    pETM->COMBINE = ETM_COMBINE_COMBINE0_MASK | ETM_COMBINE_COMP0_MASK | ETM_COMBINE_SYNCEN0_MASK | ETM_COMBINE_DTEN0_MASK |
                    ETM_COMBINE_COMBINE1_MASK | ETM_COMBINE_COMP1_MASK | ETM_COMBINE_SYNCEN1_MASK | ETM_COMBINE_DTEN1_MASK |
                    ETM_COMBINE_COMBINE2_MASK | ETM_COMBINE_COMP2_MASK | ETM_COMBINE_SYNCEN2_MASK | ETM_COMBINE_DTEN2_MASK;
    pETM->SC &= ~ETM_SC_CPWMS_MASK;
  }

  if ( ETM_PWM_HIGHTRUEPULSE == u8PWMEdgeSelect )
  {
    /* Configure ETMers PWM High True Pulses */
    for ( i = 0; i < channels; i++ )
    {
      pETM->CONTROLS[i].CnSC = ETM_CnSC_MSB_MASK | ETM_CnSC_ELSB_MASK;
      pETM->CONTROLS[i].CnV  = ETM_C0V_INIT + i * 100;
    }
  }
  else if ( ETM_PWM_LOWTRUEPULSE == u8PWMEdgeSelect )
  {
    /* Configure ETMers for PWM Low True Pulses */
    for ( i = 0; i < channels; i++ )
    {
      pETM->CONTROLS[i].CnSC = ETM_CnSC_MSB_MASK | ETM_CnSC_ELSA_MASK;
      pETM->CONTROLS[i].CnV  = ETM_C0V_INIT + i * 100 ;
    }
  }
}

/*********************************************************************************//*!
*
* @brief general configuration to ETM_No to input capture mode, enable interrupt.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    Channel               channel number to be configured.
* @param[in]    CaptureMode           select capture edge: rising, falling or both.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_InputCaptureInit( ETM_Type * pETM, uint8_t u8ETM_Channel, uint8_t u8CaptureMode )
{
  ASSERT( ( ( ETM0 == pETM ) && ( u8ETM_Channel < 2 ) )   ||
          ( ( ETM1 == pETM ) && ( u8ETM_Channel < 2 ) )   ||
          ( ( ETM2 == pETM ) && ( u8ETM_Channel < 6 ) )
        );

  /* open the clock gate */
  if ( ( ETM0 == pETM ) && ( u8ETM_Channel < 2 ) )
  {
    SIM->SCGC |= SIM_SCGC_ETM0_MASK;
    NVIC_EnableIRQ( ETM0_IRQn );
  }
  else if ( ( ETM1 == pETM )  && ( u8ETM_Channel < 2 ) )
  {
#if !defined(CPU_NV32M3)
    SIM->SCGC |= SIM_SCGC_ETM1_MASK;
    NVIC_EnableIRQ( ETM1_IRQn );
#endif
  }
  else
  {
    SIM->SCGC |= SIM_SCGC_ETM2_MASK;
    NVIC_EnableIRQ( ETM2_IRQn );
  }

  pETM->SC  = 0x0;     /* diable counter */
  pETM->MOD = 0xFFFF;  /* free running */

  if ( ETM_INPUTCAPTURE_RISINGEDGE == u8CaptureMode )     /* enable interrupt, Capture on rising edge */
  {
    pETM->CONTROLS[u8ETM_Channel].CnSC = ETM_CnSC_CHIE_MASK | ETM_CnSC_ELSA_MASK;
  }
  else if ( ETM_INPUTCAPTURE_FALLINGEDGE == u8CaptureMode ) /* Capture on falling edge */
  {
    pETM->CONTROLS[u8ETM_Channel].CnSC = ETM_CnSC_CHIE_MASK | ETM_CnSC_ELSB_MASK;
  }
  else if ( ETM_INPUTCAPTURE_BOTHEDGE == u8CaptureMode )  /* Capture on rising or falling edge */
  {
    pETM->CONTROLS[u8ETM_Channel].CnSC = ETM_CnSC_CHIE_MASK | ETM_CnSC_ELSA_MASK | ETM_CnSC_ELSB_MASK;
  }
}

/*********************************************************************************//*!
*
* @brief general configuration to ETM_No to Dual Edge Capture mode to measure the
*        width or the period  of a pulse.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    ChannelPair           ChannelPair number to be configured: 0, 2, 4.
* @param[in]    CaptureMode           select capture edge: one shot and continuous mode.
* @param[in]    Channel_N_Edge        channel N detect edge.
* @param[in]    Channel_Np1_Edge      channel N+1 detect edge.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_DualEdgeCaptureInit( ETM_Type * pETM, uint8_t u8ChannelPair, uint8_t u8CaptureMode,
                              uint8_t u8Channel_N_Edge, uint8_t u8Channel_Np1_Edge )
{
  ASSERT( ( ETM2 == pETM )  && ( u8ChannelPair < 6 ) && !( u8ChannelPair & 1 ) );
  SIM->SCGC |= SIM_SCGC_ETM2_MASK;

  if ( ( 0 == u8ChannelPair ) || ( 2 == u8ChannelPair ) )
  {
    /* channel filter is active */
  }

  pETM->SC    = 0x0;                  /* diable counter */
  pETM->MOD   = 0xFFFF;
  pETM->MODE |= ETM_MODE_ETMEN_MASK;  /* ETMEN = 1 */
  /* DECAPEN = 1,  ChannelPair/2 * 8 */
  pETM->COMBINE |=  ( ( ETM_COMBINE_DECAPEN0_MASK ) << ( u8ChannelPair * 4 ) );
  pETM->CONTROLS[u8ChannelPair].CnSC &= ~ETM_CnSC_CHF_MASK;       /* CH(n)F and CH(n+1)F bits must be cleared first */
  pETM->CONTROLS[u8ChannelPair + 1].CnSC &= ~ETM_CnSC_CHF_MASK;

  if ( ETM_INPUTCAPTURE_DUALEDGE_ONESHOT == u8CaptureMode )       /* oneshot mode */
  {
    pETM->CONTROLS[u8ChannelPair].CnSC &= ~ETM_CnSC_MSA_MASK;
    pETM->CONTROLS[u8ChannelPair + 1].CnSC &= ~ETM_CnSC_MSA_MASK;
  }
  else if ( ETM_INPUTCAPTURE_DUALEDGE_CONTINUOUS == u8CaptureMode ) /* continuouse mode */
  {
    pETM->CONTROLS[u8ChannelPair].CnSC |= ETM_CnSC_MSA_MASK;
    pETM->CONTROLS[u8ChannelPair + 1].CnSC |= ETM_CnSC_MSA_MASK;
  }

  pETM->CONTROLS[u8ChannelPair].CnSC |= ( u8Channel_N_Edge << 2 ); /* select detec edge */
  pETM->CONTROLS[u8ChannelPair + 1].CnSC |= ( u8Channel_Np1_Edge << 2 );
  pETM->COMBINE |=  ( ETM_COMBINE_DECAP0_MASK << ( u8ChannelPair * 4 ) );
}

/*********************************************************************************//*!
*
* @brief general configuration to ETM_No to input capture mode.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    Channel               channel number to be configured.
* @param[in]    CompareMode           select compare edge: toggle, set and clear.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_OutputCompareInit( ETM_Type * pETM, uint8_t u8ETM_Channel, uint8_t u8CompareMode )
{
  ASSERT( ( ( ETM0 == pETM ) && ( u8ETM_Channel < 2 ) )   ||
          ( ( ETM1 == pETM ) && ( u8ETM_Channel < 2 ) )   ||
          ( ( ETM2 == pETM ) && ( u8ETM_Channel < 6 ) )
        );

  /* open the clock gate */
  if ( ETM0 == pETM )
  {
    SIM->SCGC |= SIM_SCGC_ETM0_MASK;
  }
  else if ( ETM1 == pETM )
  {
#if !defined(CPU_NV32M3)
    SIM->SCGC |= SIM_SCGC_ETM1_MASK;
#endif
  }
  else
  {
    SIM->SCGC |= SIM_SCGC_ETM2_MASK;
  }

  pETM->SC  = 0x0;                                                                    /* diable counter */
  pETM->MOD = ETM_MOD_INIT;
  pETM->CONTROLS[u8ETM_Channel].CnSC = ( ETM_CnSC_MSA_MASK | ( u8CompareMode << 2 ) ); /* select detec edge */
  pETM->CONTROLS[u8ETM_Channel].CnV  = ETM_C0V_INIT;
}

/*********************************************************************************//*!
*
* @brief general configuration to ETM2 to start software synchronization.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_SoftwareSync( ETM_Type * pETM )
{
  ASSERT( ETM2 == pETM );
  pETM->SYNCONF   |= ETM_SYNCONF_SYNCMODE_MASK;   /* recommend enhanced sync mode */
  pETM->SYNC      |= ETM_SYNC_SWSYNC_MASK;
}

/*********************************************************************************//*!
*
* @brief general configuration to ETM to enable hardware synchronization.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    u8TriggerN            select the hardware trigger source.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_HardwareSync( ETM_Type * pETM, uint8_t u8TriggerN )
{
  ASSERT( ETM2 == pETM );
  pETM->SYNCONF   |= ETM_SYNCONF_SYNCMODE_MASK;   /* recommend enhanced sync mode */

  switch ( u8TriggerN )
  {
    case ETM_SYNC_TRIGGER_TRIGGER2:
      pETM->SYNC |= ETM_SYNC_TRIG2_MASK;
      break;

    case ETM_SYNC_TRIGGER_TRIGGER1:
      pETM->SYNC |= ETM_SYNC_TRIG1_MASK;
      break;     /* need configure ETM0CH0 first */

    case ETM_SYNC_TRIGGER_TRIGGER0:
      pETM->SYNC |= ETM_SYNC_TRIG0_MASK;
      break;     /* need configure CMP0 first */

    default:
      break;
  }
}

/*********************************************************************************//*!
*
* @brief general configuration to ETM to enable hardware synchronization, more then 1 trigger.
*
* @param[in]    pETM               pointer to one of three ETM base register address.
* @param[in]    u8TriggerMask      select the hardware trigger source. combine TRIG0~TREG2.(x000xxxx~x111xxxx)
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_HardwareSyncCombine( ETM_Type * pETM, uint8_t u8TriggerMask )
{
  ASSERT( ETM2 == pETM );
  pETM->SYNCONF   |= ETM_SYNCONF_SYNCMODE_MASK;   /* recommend enhanced sync mode */
  pETM->SYNC      &= 0x8F;
  pETM->SYNC      |= ( u8TriggerMask & 0x70 );
}

/*********************************************************************************//*!
*
* @brief generate ETM2 hardware trigger 2,Note: please call ETM_HardwareSyncCombine first.
*
* @param[in]    pETM               pointer to one of three ETM base register address.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_GenerateTrig2( ETM_Type * pETM )
{
  ASSERT( ETM2 == pETM );

  if ( pETM->SYNC & ETM_SYNC_TRIG2_MASK )
  {
#if defined(CPU_NV32)
    SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;
#elif defined(CPU_NV32M3)
    SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;
#elif defined(CPU_NV32M4)
    SIM->SOPT0  |= SIM_SOPT0_ETMSYNC_MASK;
#endif
  }
}


/*********************************************************************************//*!
*
* @brief general configuration to ETM_No to start software synchronization.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    PrescalerValue        system clock divide value, 0 to 3.
* @param[in]    DeadETMeValue         n count clock is inserted, 0 to 63.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_PWMDeadETMeSet( ETM_Type * pETM, uint8_t u8PrescalerValue, uint8_t u8DeadETMeValue )
{
  ASSERT( ETM2 == pETM );
  pETM->COMBINE |= 0x101010;              /* enable dead ETMe insertion */

  if ( !( pETM->MODE & ETM_MODE_WPDIS_MASK ) ) /* if write protection is enabled */
  {
    pETM->MODE |= ETM_MODE_WPDIS_MASK;  /* disable the write protection */
    pETM->DEADETME = ( ETM_DEADETME_DTVAL( u8DeadETMeValue & 0x3F ) | ETM_DEADETME_DTPS( u8PrescalerValue & 0x3 ) );
    pETM->MODE &= ~ETM_MODE_WPDIS_MASK; /* enable the write protection */
  }
  else
  {
    /* if no protection */
    pETM->DEADETME = ( ETM_DEADETME_DTVAL( u8DeadETMeValue & 0x3F ) | ETM_DEADETME_DTPS( u8PrescalerValue & 0x3 ) );
  }

  pETM->SYNC |= ETM_SYNC_SWSYNC_MASK;     /* software sync */
}

/*********************************************************************************//*!
*
* @brief set output mask.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    Channel               pwm channel needed to be masked.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_OutputMaskSet( ETM_Type * pETM, uint8_t u8ETM_Channel )
{
  ASSERT( ( ETM2 == pETM ) && ( u8ETM_Channel < 6 ) );
  pETM->OUTMASK |= ( 1 << u8ETM_Channel );

  if ( pETM->SYNC & ETM_SYNC_SYNCHOM_MASK )           /* if PWM sync is needed */
  {
    pETM->SYNCONF |= ETM_SYNCONF_SYNCMODE_MASK;     /* recommend enhanced sync mode */

    if ( pETM->SYNCONF & ETM_SYNCONF_SWOM_MASK )    /* if software sync is needed*/
    {
      pETM->SYNC |= ETM_SYNC_SWSYNC_MASK;
    }
    else if ( pETM->SYNCONF & ETM_SYNCONF_HWOM_MASK ) /* if hardware sync is needed*/
    {
      pETM->SYNC |= ETM_SYNC_TRIG2_MASK;
#if defined(CPU_NV32)
      SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;         /* hardware sync */
#elif defined(CPU_NV32M3)
      SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;         /* hardware sync */
#elif defined(CPU_NV32M4)
      SIM->SOPT0  |= SIM_SOPT0_ETMSYNC_MASK;         /* hardware sync */
#endif
    }
    else
    {
    }
  }
  else  /* no need to sync, update on the next rising edge of system clock  */
  {
  }
}

/*********************************************************************************//*!
*
* @brief general configuration to ETM_No to start software synchronization.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    Channel               pwm channel needed to be controlled by software.
* @param[in]    ChannelValue          the value to be set,  0 or 1.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_SWOutputControlSet( ETM_Type * pETM, uint8_t u8ETM_Channel, uint8_t u8ChannelValue )
{
  ASSERT( ( ETM2 == pETM ) && ( u8ETM_Channel < 6 ) );

  if ( ETM_SWOCTRL_HIGH == u8ChannelValue )
  {
    pETM->SWOCTRL |= ( 0x0101 << u8ETM_Channel );
  }
  else if ( ETM_SWOCTRL_LOW == u8ChannelValue )
  {
    pETM->SWOCTRL |= ( 1 << u8ETM_Channel );
    pETM->SWOCTRL &= ~( 0x100 << u8ETM_Channel );
  }

  if ( pETM->SYNCONF & ETM_SYNCONF_SWOC_MASK )            /* if PWM sync is needed */
  {
    pETM->SYNCONF |= ETM_SYNCONF_SYNCMODE_MASK;         /* recommend enhanced sync mode */

    if ( pETM->SYNCONF & ETM_SYNCONF_SWSOC_MASK )       /* if software sync is needed*/
    {
      pETM->SYNC |= ETM_SYNC_SWSYNC_MASK;             /* software sync */
    }
    else if ( pETM->SYNCONF & ETM_SYNCONF_HWSOC_MASK )  /* if hardware sync is needed*/
    {
      pETM->SYNC |= ETM_SYNC_TRIG2_MASK;
#if defined(CPU_NV32)
      SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;             /* hardware sync */
#elif defined(CPU_NV32M3)
      SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;             /* hardware sync */
#elif defined(CPU_NV32M4)
      SIM->SOPT0  |= SIM_SOPT0_ETMSYNC_MASK;             /* hardware sync */
#endif
    }
  }
  else  /* no need to sync, update on the next rising edge of system clock  */
  {
  }
}

/*********************************************************************************//*!
*
* @brief set PWM polarity.
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    Channel               pwm channel needed to be controlled by software.
* @param[in]    ActiveValue           the value to be set,  0 or 1.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_PolaritySet( ETM_Type * pETM, uint8_t u8ETM_Channel, uint8_t u8ActiveValue )
{
  ASSERT( ( ETM2 == pETM ) && ( u8ETM_Channel < 6 ) );

  if ( ETM_POLARITY_HIGHACTIVE == u8ActiveValue )
  {
    pETM->POL &=  ~( 1 << u8ETM_Channel );
  }
  else if ( ETM_POLARITY_LOWACTIVE == u8ActiveValue )
  {
    pETM->POL |=  ( 1 << u8ETM_Channel );
  }
}

/*********************************************************************************//*!
*
* @brief set ETM behavior in debug mode.
*
* @param[in]    pETM             pointer to one of three ETM base register address.
* @param[in]    u8DebugMode      debug mode select from 00 to 11.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_SetDebugModeBehavior( ETM_Type * pETM, uint8_t u8DebugMode )
{
  ASSERT( ( ETM2 == pETM ) );
  pETM->CONF &= ~ETM_CONF_BDMMODE_MASK;
  pETM->CONF |= ETM_CONF_BDMMODE( u8DebugMode );
}

/*********************************************************************************//*!
*
* @brief Selects the ratio between the number of counter overflows to the number of ETMes the TOF bit is set.
*
* @param[in]    pETM             pointer to one of three ETM base register address.
* @param[in]    u8TOFNUM         TOF numbers before setting TOF bit, 0~31.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_SetTOFFrequency( ETM_Type * pETM, uint8_t u8TOFNUM )
{
  ASSERT( ( ETM2 == pETM ) );
  pETM->CONF &= ~ETM_CONF_NUMTOF_MASK;
  pETM->CONF |= ETM_CONF_NUMTOF( u8TOFNUM );
}

/*********************************************************************************//*!
*
* @brief swap the output of CH(n) and CH(n+1).
*
* @param[in]    pETM                  pointer to one of three ETM base register address.
* @param[in]    ChannelPair           the pair to be swapped, 0,1,2.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
void ETM_InvertChannel( ETM_Type * pETM, uint8_t u8ChannelPair )
{
  ASSERT( ( ETM2 == pETM )  && u8ChannelPair <= 2 );
  pETM->INVCTRL |= 1 << u8ChannelPair;

  if ( pETM->SYNCONF & ETM_SYNCONF_INVC_MASK )    /* if PWM sync is needed */
  {
    pETM->SYNCONF |= ETM_SYNCONF_SYNCMODE_MASK; /* recommend enhanced sync mode */

    if ( pETM->SYNCONF & ETM_SYNCONF_SWINVC_MASK ) /* if software sync is needed*/
    {
      pETM->SYNC |= ETM_SYNC_SWSYNC_MASK;     /* software sync */
    }
    else if ( pETM->SYNCONF & ETM_SYNCONF_HWINVC_MASK ) /* if hardware sync is needed*/
    {
      pETM->SYNC |= ETM_SYNC_TRIG2_MASK;
#if defined(CPU_NV32)
      SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;             /* hardware sync */
#elif defined(CPU_NV32M3)
      SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;             /* hardware sync */
#elif defined(CPU_NV32M4)
      SIM->SOPT0  |= SIM_SOPT0_ETMSYNC_MASK;             /* hardware sync */
#endif
    }
  }
  else  /* no need to sync, update on the next rising edge of system clock  */
  {
  }
}

/*****************************************************************************//*!
*
* @brief configure the ETM as specified control parameters, CnSC and CnV not
*        included.
*
* @param[in]    pETM          pointer to one of three ETM base register address.
* @param[in]    pConfig     pointer to ETM general parameters.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void ETM_Init( ETM_Type * pETM, ETM_ConfigType * pConfig )
{
  ASSERT( ( ETM0 == pETM ) || ( ETM1 == pETM ) || ( ETM2 == pETM ) );

  if ( ETM0 == pETM )
  {
    SIM->SCGC |= SIM_SCGC_ETM0_MASK;
  }

#if !defined(CPU_NV32M3)
  else if ( ETM1 == pETM )
  {
    SIM->SCGC |= SIM_SCGC_ETM1_MASK;
  }

#endif
  else
  {
    SIM->SCGC |= SIM_SCGC_ETM2_MASK;
  }

  /* diable counter */
  pETM->SC = 0;
  pETM->MODE = pConfig->mode;
  pETM->MOD = pConfig->modulo;
  pETM->CNT = pConfig->cnt;

  if ( pETM->MODE & ETM_MODE_ETMEN_MASK  )
  {
    /* when ETMEN = 1, all other registers can be written */
    pETM->COMBINE   = pConfig->combine;
    pETM->CNTIN     = pConfig->cntin;
    pETM->SYNC      = pConfig->sync;
    pETM->OUTINIT   = pConfig->outinit;
    pETM->OUTMASK   = pConfig->outmask;
    pETM->DEADETME  = pConfig->deadETMe;
    pETM->EXTTRIG   = pConfig->exttrig;
    pETM->POL       = pConfig->pol;
    pETM->FMS       = pConfig->fms;
    pETM->FILTER    = pConfig->filter;
    pETM->FLTCTRL   = pConfig->fltctrl;    /* fault control */
    pETM->FLTPOL    = pConfig->fltpol;
    pETM->CONF      = pConfig->conf;
    pETM->SYNCONF   = pConfig->synconf;
    pETM->SWOCTRL   = pConfig->swoctrl;
    pETM->PWMLOAD   = pConfig->pwmload;
  }

  /* write SC to enable clock */
  pETM->SC = pConfig->sc;
}


/*****************************************************************************//*!
*
* @brief  close the ETM moudle.
*
* @param[in]    pETM          pointer to one of three ETM base register address.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void ETM_DeInit( ETM_Type * pETM )
{
  ASSERT( ( ETM0 == pETM ) || ( ETM1 == pETM ) || ( ETM2 == pETM ) );
  pETM->SC = 0;
  pETM->MOD = 0;
  pETM->CNT = 0;

  if ( ETM2 == pETM )
  {
    pETM->MODE = 0x4;
    pETM->COMBINE = 0;
    pETM->CNTIN = 0;
    pETM->SYNC = 0;
    pETM->OUTINIT = 0;
    pETM->OUTMASK = 0;
    pETM->DEADETME = 0;
    pETM->EXTTRIG = 0;
    pETM->POL = 0;
    pETM->FMS = 0;
    pETM->FILTER = 0;
    pETM->FLTCTRL = 0;
    pETM->FLTPOL = 0;
    pETM->CONF = 0;
    pETM->SYNCONF = 0;
    pETM->SWOCTRL = 0;
    pETM->PWMLOAD = 0;
  }

  /* close the clock gate */
  if ( ETM0 == pETM )
  {
    SIM->SCGC &= ~SIM_SCGC_ETM0_MASK;
    NVIC_DisableIRQ( ETM0_IRQn );
  }

#if !defined(CPU_NV32M3)
  else if ( ETM1 == pETM )
  {
    SIM->SCGC &= ~SIM_SCGC_ETM1_MASK;
    NVIC_DisableIRQ( ETM1_IRQn );
  }

#endif
  else if ( ETM2 == pETM )
  {
    SIM->SCGC &= ~SIM_SCGC_ETM2_MASK;
    NVIC_DisableIRQ( ETM2_IRQn );
  }
}

/*****************************************************************************//*!
*
* @brief configure the ETM  channels, CnSC and CnV are included.
*
* @param[in]    pETM               pointer to one of three ETM base register address.
* @param[in]    ETM_Channel        ETM channel number.
* @param[in]    pTETMCH_Params     pointer to ETM channel general parameters.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void ETM_ChannelInit( ETM_Type * pETM, uint8_t u8ETM_Channel, ETM_ChParamsType * pTETMCH_Params )
{
  ASSERT( ( ETM0 == pETM ) || ( ETM1 == pETM ) || ( ETM2 == pETM ) );

  if ( ETM0 == pETM )
  {
    ASSERT( u8ETM_Channel < 2 );
    SIM->SCGC |= SIM_SCGC_ETM0_MASK;
  }

#if !defined(CPU_NV32M3)
  else if ( ETM1 == pETM )
  {
    ASSERT( u8ETM_Channel < 2 );
    SIM->SCGC |= SIM_SCGC_ETM1_MASK;
  }

#endif
  else
  {
    ASSERT( u8ETM_Channel < 6 );
    SIM->SCGC |= SIM_SCGC_ETM2_MASK;
  }

  pETM->CONTROLS[u8ETM_Channel].CnSC = pTETMCH_Params->u8CnSC;
  pETM->CONTROLS[u8ETM_Channel].CnV = pTETMCH_Params->u16CnV;
  return;
}

/*****************************************************************************//*!
*
* @brief set the ETM channel value register per duty cycle and modulo for combine mode
*        odd channel no must be provided and even channel value register is not changed.
*
* @param[in]    pETM            pointer to one of three ETM base register address.
* @param[in]    ETM_Channel     odd channel no:1,3,5.
* @param[in]    dutyCycle       duty cycle in percentage. e.g. 10, means 10%.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void  ETM_SetDutyCycleCombine( ETM_Type * pETM, uint8_t u8ETM_Channel, uint8_t u8DutyCycle )
{
  uint16_t   cnv = pETM->CONTROLS[u8ETM_Channel - 1].CnV;
  uint16_t   modulo = pETM->MOD;
  ASSERT( ( 1 == u8ETM_Channel ) || ( 3 == u8ETM_Channel ) || ( 5 == u8ETM_Channel ) );
  cnv += ( u8DutyCycle * ( modulo + 1 ) )  / 100;

  if ( cnv > modulo )
  {
    cnv = modulo - 1;
  }

  pETM->CONTROLS[u8ETM_Channel].CnV = cnv ;
  pETM->PWMLOAD |= ETM_PWMLOAD_LDOK_MASK | ( 1 << u8ETM_Channel );
}

/*****************************************************************************//*!
*
* @brief configure the ETMx_SYNCONF register including SW and HW Sync selection.
*
* @param[in]    pETM             pointer to one of three ETM base register address.
* @param[in]    u32ConfigValue   ETMx_SYNCONF register config value.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void  ETM_SyncConfigActivate( ETM_Type * pETM, uint32_t u32ConfigValue )
{
  ASSERT( ( ETM2 == pETM ) );
  pETM->SYNCONF |= u32ConfigValue;
}

/*****************************************************************************//*!
*
* @brief configure the ETMx_SYNCONF register including SW and HW Sync selection.
*
* @param[in]    pETM             pointer to one of three ETM base register address.
* @param[in]    u32ConfigValue   ETMx_SYNCONF register config value.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void  ETM_SyncConfigDeactivate( ETM_Type * pETM, uint32_t u32ConfigValue )
{
  ASSERT( ( ETM2 == pETM ) );
  pETM->SYNCONF &= ~u32ConfigValue;
}

/*****************************************************************************//*!
*
* @brief This function sets the callback function.
*
* @param[in]    pETM          pointer to one of three ETM base register address.
* @param[in]    pfnCallback     functon address.
*
* @return none.
*
* @ Pass/ Fail criteria: none
*
*****************************************************************************/
void  ETM_SetCallback( ETM_Type * pETM, ETM_CallbackPtr pfnCallback )
{
  ETM_Callback[( ( uint32_t )pETM - ( uint32_t )ETM0_BASE ) >> 12] = pfnCallback;
}

/*! @} End of ETM_api_list                                                    */


/*****************************************************************************//*!
*
* @brief  ETM0_Isr interrupt service routine.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void ETM0_Isr( void )
{
  if ( ETM_Callback[0] )
  {
    ETM_Callback[0]();
  }
}

/*****************************************************************************//*!
*
* @brief  ETM1_Isr interrupt service routine.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void ETM1_Isr( void )
{
  if ( ETM_Callback[1] )
  {
    ETM_Callback[1]();
  }
}

/*****************************************************************************//*!
*
* @brief  ETM2_Isr interrupt service routine.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void ETM2_Isr( void )
{
  if ( ETM_Callback[2] )
  {
    ETM_Callback[2]();
  }
}



