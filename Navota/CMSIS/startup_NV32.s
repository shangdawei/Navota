;/*****************************************************************************
;* File Name          : startup_NV32.s
;* Description        : NV32F100 devices vector table for EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR
;*                        address
;*                      - Configure the system clock
;*                      - Branches to main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M0 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;
;  Unless required by applicable law or agreed to in writing, software
;  distributed under the License is distributed on an "AS IS" BASIS,
;  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;  See the License for the specific language governing permissions and
;  limitations under the License.
;
;*******************************************************************************
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler                  ; Reset Handler

        DCD     NMI_Handler                    ; NMI Handler
        DCD     HardFault_Handler              ; Hard Fault Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     SVC_Handler                    ; SVCall Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     PendSV_Handler                 ; PendSV Handler
        DCD     SysTick_Handler                ; SysTick Handler

        ; External Interrupts
        DCD     Reserved16_IRQHandler  ; Reserved interrupt 16
        DCD     Reserved17_IRQHandler  ; Reserved interrupt 17
        DCD     Reserved18_IRQHandler  ; Reserved interrupt 18
        DCD     Reserved19_IRQHandler  ; Reserved interrupt 19
        DCD     Reserved20_IRQHandler  ; Reserved interrupt 20
        DCD     ETMRH_IRQHandler  ; ETMRH command complete/read collision interrupt
        DCD     LVD_LVW_IRQHandler  ; Low Voltage Detect, Low Voltage Warning
        DCD     IRQ_IRQHandler  ; External interrupt
        DCD     I2C0_IRQHandler  ; I2C0 interrupt
        DCD     Reserved25_IRQHandler  ; Reserved interrupt 25
        DCD     SPI0_IRQHandler  ; SPI0 interrupt
        DCD     SPI1_IRQHandler  ; SPI1 interrupt
        DCD     UART0_IRQHandler  ; UART0 status/error interrupt
        DCD     UART1_IRQHandler  ; UART1 status/error interrupt
        DCD     UART2_IRQHandler  ; UART2 status/error interrupt
        DCD     ADC0_IRQHandler  ; ADC0 interrupt
        DCD     ACMP0_IRQHandler  ; ACMP0 interrupt
        DCD     ETM0_IRQHandler  ; ETM0 Single interrupt vector for all sources
        DCD     ETM1_IRQHandler  ; ETM1 Single interrupt vector for all sources
        DCD     ETM2_IRQHandler  ; ETM2 Single interrupt vector for all sources
        DCD     RTC_IRQHandler  ; RTC overflow
        DCD     ACMP1_IRQHandler  ; ACMP1 interrupt
        DCD     PIT_CH0_IRQHandler  ; PIT CH0 overflow
        DCD     PIT_CH1_IRQHandler  ; PIT CH1 overflow
        DCD     KBI0_IRQHandler  ; Keyboard interrupt 0
        DCD     KBI1_IRQHandler  ; Keyboard interrupt 1
        DCD     Reserved42_IRQHandler  ; Reserved interrupt 42
        DCD     ICS_IRQHandler  ; MCG interrupt
        DCD     Watchdog_IRQHandler  ; WDOG Interrupt
        DCD     Reserved45_IRQHandler  ; Reserved interrupt 45
        DCD     Reserved46_IRQHandler  ; Reserved interrupt 46
        DCD     Reserved47_IRQHandler  ; Reserved interrupt 47

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        ;PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK  Reserved16_IRQHandler
        PUBWEAK  Reserved17_IRQHandler
        PUBWEAK  Reserved18_IRQHandler
        PUBWEAK  Reserved19_IRQHandler
        PUBWEAK  Reserved20_IRQHandler
        PUBWEAK  ETMRH_IRQHandler
        PUBWEAK  LVD_LVW_IRQHandler
        PUBWEAK  IRQ_IRQHandler
        PUBWEAK  I2C0_IRQHandler
        PUBWEAK  Reserved25_IRQHandler
        PUBWEAK  SPI0_IRQHandler
        PUBWEAK  SPI1_IRQHandler
        PUBWEAK  UART0_IRQHandler
        PUBWEAK  UART1_IRQHandler
        PUBWEAK  UART2_IRQHandler
        PUBWEAK  ADC0_IRQHandler
        PUBWEAK  ACMP0_IRQHandler
        PUBWEAK  ETM0_IRQHandler
        PUBWEAK  ETM1_IRQHandler
        PUBWEAK  ETM2_IRQHandler
        PUBWEAK  RTC_IRQHandler
        PUBWEAK  ACMP1_IRQHandler
        PUBWEAK  PIT_CH0_IRQHandler
        PUBWEAK  PIT_CH1_IRQHandler
        PUBWEAK  KBI0_IRQHandler
        PUBWEAK  KBI1_IRQHandler
        PUBWEAK  Reserved42_IRQHandler
        PUBWEAK  ICS_IRQHandler
        PUBWEAK  Watchdog_IRQHandler
        PUBWEAK  Reserved45_IRQHandler
        PUBWEAK  Reserved46_IRQHandler
        PUBWEAK  Reserved47_IRQHandler
        PUBWEAK  DefaultISR
        PUBWEAK  NMI_Handler
        PUBWEAK  SVC_Handler
        PUBWEAK  PendSV_Handler
        PUBWEAK  SysTick_Handler
        PUBWEAK  HardFault_Handler

HardFault_Handler
NMI_Handler
SVC_Handler
PendSV_Handler
SysTick_Handler
Reserved16_IRQHandler
Reserved17_IRQHandler
Reserved18_IRQHandler
Reserved19_IRQHandler
Reserved20_IRQHandler
ETMRH_IRQHandler
LVD_LVW_IRQHandler
IRQ_IRQHandler
I2C0_IRQHandler
Reserved25_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
ADC0_IRQHandler
ACMP0_IRQHandler
ETM0_IRQHandler
ETM1_IRQHandler
ETM2_IRQHandler
RTC_IRQHandler
ACMP1_IRQHandler
PIT_CH0_IRQHandler
PIT_CH1_IRQHandler
KBI0_IRQHandler
KBI1_IRQHandler
Reserved42_IRQHandler
ICS_IRQHandler
Watchdog_IRQHandler
Reserved45_IRQHandler
Reserved46_IRQHandler
Reserved47_IRQHandler
DefaultISR
        B       .

        END
