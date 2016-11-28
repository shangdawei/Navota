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
/*定义外部时钟/晶振的频率 in KHz, 0 -- 内部时钟模式 */
//#define EXTAL_CLK_FREQ_KHZ    0

/*RANGE = 0 : 32 kHz 的低频范围 */
//#define EXTAL_CLK_FREQ_KHZ    32

/*RANGE = 1 : 4-24 MHz 的高频范围 */
//#define EXTAL_CLK_FREQ_KHZ    4000
//#define EXTAL_CLK_FREQ_KHZ    6000
//#define EXTAL_CLK_FREQ_KHZ    8000
#define EXTAL_CLK_FREQ_KHZ      10000
//#define EXTAL_CLK_FREQ_KHZ    12000
//#define EXTAL_CLK_FREQ_KHZ    16000
//#define EXTAL_CLK_FREQ_KHZ    20000
//#define EXTAL_CLK_FREQ_KHZ    24000


/*定义是否使用内部谐振器, 0 - 禁用, 使用外部方波输入 */
//#define XTAL_CLK_FREQ_KHZ     0
#define XTAL_CLK_FREQ_KHZ       EXTAL_CLK_FREQ_KHZ

/*定义在停止模式是否启用OSC模块 0 - 禁止, 1 - 启用 */
#define OSC_STOP_ENABLED        ( 1 )

/*定义是否晶振谐振器的增益 0 - 低功耗, 1 - 高增益 */
#define OSC_HIGH_GRAIN          ( 1 )

/*定义是否启用 FLL 0 - 禁用FLL, 低功耗模式 1 - 启用 FLL */
#define FLL_ENABLED             ( 1 )

/*定义是否旁路 FLL 0 - 旁路FLL, 1 - 启用 FLL */
#define FLL_BYPASSED            ( 0 )

/*

ICS 有七种工作模式：FEI、FBI、FBILP、FEE、FBE、FBELP 和STOP。

1 FLL 内部启用(FEI) 模式 ：FLL_ENABLED=1, FLL_BYPASSED=0
默认模式，在该模式下，FLL 时钟由内部参考时钟控制。ICS 提供一个来源于FLL 的时钟。

2 FLL 内部旁路(FBI) 模式 ：FLL_ENABLED=1, FLL_BYPASSED=1
FLL 使能并通过内部参考时钟来控制，但处于旁路状态。ICS 提供从内部参考时钟分配而来的时钟。

3 FLL 内部旁路低功耗(FBILP)模式 ：FLL_ENABLED=0, FLL_BYPASSED=1
FLL 禁用并旁路，ICS 提供由内部参考时钟分频而来的时钟。

4 FLL 外部使用(FEE)模式 ：FLL_ENABLED=1, FLL_BYPASSED=0
在该模式下，FLL 时钟由外部参考时钟控制。ICS 提供一个来源于FLL 的时钟。

5 FLL 外部旁路(FBE)模式 ：FLL_ENABLED=1, FLL_BYPASSED=1
 FLL 使能并通过外部参考时钟来控制，但处于旁路状态。ICS 提供从外部参考时钟*分频*而来的时钟。

6 FLL 外部旁路的低功耗(FBELP) 模式 ：FLL_ENABLED=0, FLL_BYPASSED=1
FLL 禁用并旁路，ICS 提供由外部参考时钟*分频*而来的时钟。

7 停止STOP模式
FLL 禁用,内部或ICS 外部基准时钟源(OSC_OUT)是使能还是禁用可以选型。ICS 不提供任何MCU 时钟源。

*/

/* FEI Mode : 37.5K * 1280 = 48,000,000 */
#define SYSTEM_CORE_CLOCK         ( 48000000 )

/* 总线时钟分频器, 为总线时钟设置分频器值
 * 0 总线时钟频率与ICSOUTCLK 相同。
 * 1 总线时钟频率是ICSOUTCLK/2。
 */
#define BUSCLK_DIV_BY_2           ( 1 )

/*定义是否通过管脚PH2输出总线时钟: SIM_SOPT.CLKOE */
#define BUSCLK_OUTPUT_ENABLED     ( 0 )

/*定义通过管脚 PH2 输出总线时钟的分频: SIM_SOPT.BUSREF */
#define BUSCLK_OUTPUT_REF         ( 0 )     /*  无分频 */
//#define BUSCLK_OUTPUT_REF       ( 1 )     /* 2  分频 */
//#define BUSCLK_OUTPUT_REF       ( 2 )     /* 4  分频 */
//#define BUSCLK_OUTPUT_REF       ( 3 )     /* 8  分频 */
//#define BUSCLK_OUTPUT_REF       ( 4 )     /* 16 分频 */
//#define BUSCLK_OUTPUT_REF       ( 5 )     /* 32 分频 */
//#define BUSCLK_OUTPUT_REF       ( 6 )     /* 64 分频 */
//#define BUSCLK_OUTPUT_REF       ( 7 )     /* 128分频 */

/*定义是否启用 PB4 作为 NMI 不可屏蔽中断的管脚 : SIM_SOPT.NMIE 该引脚在MCU POR 之后默认用作 NMI */
#define NMI_PIN_ENABLED           ( 0 )

/*定义是否启用 PA5 作为 RST 管脚 : SIM_SOPT.RSTPE 该引脚在MCU POR 之后默认用作 RST */
#define RST_PIN_ENABLED           ( 1 )

/*定义是否重新映射SPI0 管脚 */
#define SPI0_REMAPPED             ( 0 )

/*定义是否使能看门狗*/
#define WDOG_ENABLED              ( 1 )

/*定义TRIM 值校准内部IRC */
#define ICS_TRIM_ENABLED          ( 1 )

//#define ICS_TRIM_VALUE          0x00      /*trim IRC to 39.0625KHz and FLL output=57MHz */
//#define ICS_TRIM_VALUE          0x4c      /*trim IRC to 39.0625KHz and FLL output=40MHz */
#define ICS_TRIM_VALUE            0x29      /*trim IRC to 39.0625KHz and FLL output=48MHz */


#endif /* NVxx_CONFIG_H_ */
