#ifndef __NV32_CONFIG_H__
#define __NV32_CONFIG_H__

/******************************************************************************/
//#define USE_FULL_ASSERT

/******************************************************************************/
/*BACKKEY Backdoor Comparison Key
  16-byte flash configuration field that stores default protection settings
  (loaded on reset) and security information that allows the MCU
  to restrict acces to the FTFL module. */
#define NV_BACKKEY     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF

/*EEPROM Protection Register (EEPROT)
  defines which D-Flash sectors are protected against program and erase operations
  Bit[7:7] : <0> Enables EEPROM memory protection
  Bit[7:7] : <1> Disables EEPROM memory protection
  Bit[2:0] : <0> Flash address range: 0x00_0000 - 0x00_001F; protected size: 32 bytes
  Bit[2:0] : <7> Flash address range: 0x00_0000 - 0x00_00FF; protected size: 256 bytes
*/
#define NV_EEPROT      0xFF

/*P-Flash Protection Register (FPROT)
  defines which P-Flash sectors are protected against program and erase operations

*/
#define NV_FPROT       0xFF

/*FSEC Flash security byte (FSEC)
  If SEC field is configured as "MCU security status is secure" and
  MEEN field is configured as "Mass erase is disabled", MCU's security status
  cannot be set back to unsecure state since Mass erase via the debugger is blocked !!!

  Bit[7:6] : KEYEN : These bits enable and disable backdoor key access to the FTFL module
  Bit[7:6] : <2> Backdoor key access enabled
  Bit[7:6] : <3> Backdoor key access disabled

  Bit[1:0] : SEC : define the security state of the MCU
  Bit[1:0] : <2> MCU security status is unsecure ******
  Bit[1:0] : <3> MCU security status is secure
*/
#define NV_FSEC        0xFE   // 11-11-11-10

/*Flash Option Register (FOPT)
*/
#define NV_FOPT        0xFE

/******************************************************************************/
#define CPU_NV32
#define CPU_NV32M0
//#define CPU_NV32M3
//#define CPU_NV32M4

/******************************************************************************/
/*�����ⲿʱ��/�����Ƶ�� in KHz, 0 -- �ڲ�ʱ��ģʽ */
//#define EXTAL_CLK_FREQ_KHZ    0

/*RANGE = 0 : 32 kHz �ĵ�Ƶ��Χ */
//#define EXTAL_CLK_FREQ_KHZ    32

/*RANGE = 1 : 4-24 MHz �ĸ�Ƶ��Χ */
//#define EXTAL_CLK_FREQ_KHZ    4000
//#define EXTAL_CLK_FREQ_KHZ    6000
//#define EXTAL_CLK_FREQ_KHZ    8000
#define EXTAL_CLK_FREQ_KHZ      10000
//#define EXTAL_CLK_FREQ_KHZ    12000
//#define EXTAL_CLK_FREQ_KHZ    16000
//#define EXTAL_CLK_FREQ_KHZ    20000
//#define EXTAL_CLK_FREQ_KHZ    24000


/*�����Ƿ�ʹ���ڲ�г����, 0 - ����, ʹ���ⲿ�������� */
//#define XTAL_CLK_FREQ_KHZ     0
#define XTAL_CLK_FREQ_KHZ       EXTAL_CLK_FREQ_KHZ

/*������ֹͣģʽ�Ƿ�����OSCģ�� 0 - ��ֹ, 1 - ���� */
#define OSC_STOP_ENABLED        ( 1 )

/*�����Ƿ���г���������� 0 - �͹���, 1 - ������ */
#define OSC_HIGH_GRAIN          ( 1 )

/*�����Ƿ����� FLL 0 - ����FLL, �͹���ģʽ 1 - ���� FLL */
#define FLL_ENABLED             ( 1 )

/*�����Ƿ���· FLL 0 - ��·FLL, 1 - ���� FLL */
#define FLL_BYPASSED            ( 0 )

/*

ICS �����ֹ���ģʽ��FEI��FBI��FBILP��FEE��FBE��FBELP ��STOP��

1 FLL �ڲ�����(FEI) ģʽ ��FLL_ENABLED=1, FLL_BYPASSED=0
Ĭ��ģʽ���ڸ�ģʽ�£�FLL ʱ�����ڲ��ο�ʱ�ӿ��ơ�ICS �ṩһ����Դ��FLL ��ʱ�ӡ�

2 FLL �ڲ���·(FBI) ģʽ ��FLL_ENABLED=1, FLL_BYPASSED=1
FLL ʹ�ܲ�ͨ���ڲ��ο�ʱ�������ƣ���������·״̬��ICS �ṩ���ڲ��ο�ʱ�ӷ��������ʱ�ӡ�

3 FLL �ڲ���·�͹���(FBILP)ģʽ ��FLL_ENABLED=0, FLL_BYPASSED=1
FLL ���ò���·��ICS �ṩ���ڲ��ο�ʱ�ӷ�Ƶ������ʱ�ӡ�

4 FLL �ⲿʹ��(FEE)ģʽ ��FLL_ENABLED=1, FLL_BYPASSED=0
�ڸ�ģʽ�£�FLL ʱ�����ⲿ�ο�ʱ�ӿ��ơ�ICS �ṩһ����Դ��FLL ��ʱ�ӡ�

5 FLL �ⲿ��·(FBE)ģʽ ��FLL_ENABLED=1, FLL_BYPASSED=1
 FLL ʹ�ܲ�ͨ���ⲿ�ο�ʱ�������ƣ���������·״̬��ICS �ṩ���ⲿ�ο�ʱ��*��Ƶ*������ʱ�ӡ�

6 FLL �ⲿ��·�ĵ͹���(FBELP) ģʽ ��FLL_ENABLED=0, FLL_BYPASSED=1
FLL ���ò���·��ICS �ṩ���ⲿ�ο�ʱ��*��Ƶ*������ʱ�ӡ�

7 ֹͣSTOPģʽ
FLL ����,�ڲ���ICS �ⲿ��׼ʱ��Դ(OSC_OUT)��ʹ�ܻ��ǽ��ÿ���ѡ�͡�ICS ���ṩ�κ�MCU ʱ��Դ��

*/

/* FEI Mode : 37.5K * 1280 = 48,000,000 */
#define SYSTEM_CORE_CLOCK         ( 48000000 )

/* ����ʱ�ӷ�Ƶ��, Ϊ����ʱ�����÷�Ƶ��ֵ
 * 0 ����ʱ��Ƶ����ICSOUTCLK ��ͬ��
 * 1 ����ʱ��Ƶ����ICSOUTCLK/2��
 */
#define BUSCLK_DIV_BY_2           ( 1 )

/*�����Ƿ�ͨ���ܽ�PH2�������ʱ��: SIM_SOPT.CLKOE */
#define BUSCLK_OUTPUT_ENABLED     ( 0 )

/*����ͨ���ܽ� PH2 �������ʱ�ӵķ�Ƶ: SIM_SOPT.BUSREF */
#define BUSCLK_OUTPUT_REF         ( 0 )     /*  �޷�Ƶ */
//#define BUSCLK_OUTPUT_REF       ( 1 )     /* 2  ��Ƶ */
//#define BUSCLK_OUTPUT_REF       ( 2 )     /* 4  ��Ƶ */
//#define BUSCLK_OUTPUT_REF       ( 3 )     /* 8  ��Ƶ */
//#define BUSCLK_OUTPUT_REF       ( 4 )     /* 16 ��Ƶ */
//#define BUSCLK_OUTPUT_REF       ( 5 )     /* 32 ��Ƶ */
//#define BUSCLK_OUTPUT_REF       ( 6 )     /* 64 ��Ƶ */
//#define BUSCLK_OUTPUT_REF       ( 7 )     /* 128��Ƶ */

/*�����Ƿ����� PB4 ��Ϊ NMI ���������жϵĹܽ� : SIM_SOPT.NMIE ��������MCU POR ֮��Ĭ������ NMI */
#define NMI_PIN_ENABLED           ( 0 )

/*�����Ƿ����� PA5 ��Ϊ RST �ܽ� : SIM_SOPT.RSTPE ��������MCU POR ֮��Ĭ������ RST */
#define RST_PIN_ENABLED           ( 1 )

/*�����Ƿ�����ӳ��SPI0 �ܽ� */
#define SPI0_REMAPPED             ( 0 )

/*�����Ƿ�ʹ�ܿ��Ź�*/
#define WDOG_ENABLED              ( 1 )

/*����TRIM ֵУ׼�ڲ�IRC */
#define ICS_TRIM_ENABLED          ( 1 )

//#define ICS_TRIM_VALUE          0x00      /*trim IRC to 39.0625KHz and FLL output=57MHz */
//#define ICS_TRIM_VALUE          0x4c      /*trim IRC to 39.0625KHz and FLL output=40MHz */
#define ICS_TRIM_VALUE            0x29      /*trim IRC to 39.0625KHz and FLL output=48MHz */


#endif /* NVxx_CONFIG_H_ */
