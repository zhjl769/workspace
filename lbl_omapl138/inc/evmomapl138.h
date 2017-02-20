//-----------------------------------------------------------------------------
// \file    evmomapl138.h
// \brief   OMAP-L138 internal registers & bit definitions.
//
//-----------------------------------------------------------------------------

#ifndef EVMOMAPL138_H
#define EVMOMAPL138_H

#include "types.h"
//#include "c6x.h"
#include "evmomapl138_sysconfig.h"
#include "evmomapl138_psc.h"
#include "evmomapl138_pll.h"

//-----------------------------------------------------------------------------
// bit manipulation macros and bitval lookup table declarations.
//-----------------------------------------------------------------------------
#define SETBIT(dest,mask)     (dest |= mask)
#define CLRBIT(dest,mask)     (dest &= ~mask)
#define TGLBIT(dest,mask)     (dest ^= mask)
#define CHKBIT(dest,mask)     (dest & mask)

#define CSL_FMK(PER_REG_FIELD, val)                                         \
    (((val) << CSL_##PER_REG_FIELD##_SHIFT) & CSL_##PER_REG_FIELD##_MASK)

//This macro takes in a string (PER_REG_FIELD) and a value
// shifts is is the specified shift & AND's it with it's mask.
//This prepares the value to be used in another macro by sit or clearing (1 or 0)
//a particular bit in a register, and clearing all the rest of the bits.


/* the Field EXTract macro */
#define CSL_FEXT(reg, PER_REG_FIELD)                                        \
    (((reg) & CSL_##PER_REG_FIELD##_MASK) >> CSL_##PER_REG_FIELD##_SHIFT)
// This macro takes in a register ANDs it with it's mask, and shifts it by shift
//This macro extracts a bit to 1 or 0 by by specifying.



/* the Field INSert macro */
//Register, Register Field, Value
#define CSL_FINS(reg, PER_REG_FIELD, val)                                   \
   ((reg) = ((reg) & ~CSL_##PER_REG_FIELD##_MASK)|CSL_FMK(PER_REG_FIELD, val))
//This macro takes in a register, string, and value.
//This macro inserts a bit (set or cleared) into it's corresponding FIELD in the register, 
//and leaves all the other bits untouched.


/* the "token" macros */

/* the Field MaKe (Token) macro */
#define CSL_FMKT(PER_REG_FIELD, TOKEN)                                      \
    CSL_FMK(PER_REG_FIELD, CSL_##PER_REG_FIELD##_##TOKEN)
//this makco does the same thing as the CSL_FMK, except that it takes
//a tken instead of a value. In other words, the token is expanded and replaced with a value, before
//the CSL_FMK is called with the value.



/* the Field INSert (Token) macro */
#define CSL_FINST(reg, PER_REG_FIELD, TOKEN)                                \
    CSL_FINS((reg), PER_REG_FIELD, CSL_##PER_REG_FIELD##_##TOKEN)

//This macro does the same thing as the CSL_FINS macro execpt witha token, instead of
//a value.

/* the "raw" macros */

/* the Field MaKe (Raw) macro */
#define CSL_FMKR(msb, lsb, val)                                             \
    (((val) & ((1 << ((msb) - (lsb) + 1)) - 1)) << (lsb))

/* the Field EXTract (Raw) macro */
#define CSL_FEXTR(reg, msb, lsb)                                            \
    (((reg) >> (lsb)) & ((1 << ((msb) - (lsb) + 1)) - 1))

/* the Field INSert (Raw) macro */
#define CSL_FINSR(reg, msb, lsb, val)                                       \
    ((reg) = ((reg) &~ (((1 << ((msb) - (lsb) + 1)) - 1) << (lsb)))         \
    | CSL_FMKR(msb, lsb, val))


extern const uint32_t bitval_u32[32];

//-----------------------------------------------------------------------------
// bitmask defines for EMUMGT.
//-----------------------------------------------------------------------------
#define SOFT                  (0x00000002)   // bit 1
#define FREE                  (0x00000001)   // bit 0

//-----------------------------------------------------------------------------
// Return Error Defines
//-----------------------------------------------------------------------------
#define ERR_NO_ERROR             (0)
#define ERR_FAIL                 (1)
#define ERR_INIT_FAIL            (2)
#define ERR_NO_UI_BOARD          (3)
#define ERR_INVALID_PARAMETER    (4)
#define ERR_TIMEOUT              (5)
#define ERR_UART_RX_FIFO_EMPTY   (6)
#define ERR_SPI_BUS_ERROR        (7)
#define ERR_ENET_LINK_INACTIVE   (8)
#define ERR_ENET_PKT_TOO_LARGE   (9)
#define ERR_ENET_Q_EMPTY         (10)
#define ERR_ENET_RX_ERROR        (11)
#define ERR_FLASH_VERIFY         (12)
#define ERR_MMCSD_TIMEOUT        (13)
#define ERR_NAND_BAD_BLOCK       (14)
#define ERR_UIDB_NUM_DEVICES     (15)    //for UI databus muxing control 
#define ERR_UIDB_INCOMPAT_DEV    (16)
#define ERR_UIDB_INVALID_DEVICE  (17)
//-----------------------------------------------------------------------------
// Utility Print Defines
//-----------------------------------------------------------------------------
#define PRINT_NONE                  (0)
#define PRINT_DURING_VERIFY         (1)
#define PRINT_HEADER                (0)
#define PRINT_CONTINUE              (1)


#define CSL_ASYNC_2_FREQ                    (24000000u)

//-----------------------------------------------------------------------------
// power and sleep config registers
//-----------------------------------------------------------------------------
#define PSC0_REG_BASE         (0x01C10000)
#define PSC1_REG_BASE         (0x01E27000)

//-----------------------------------------------------------------------------
// system config registers
//-----------------------------------------------------------------------------
#define SYSCONFIG_REG_BASE    (0x01C14000)

//-----------------------------------------------------------------------------
// PLL registers
//-----------------------------------------------------------------------------
#define PLL0_REG_BASE         (0x01C11000)
#define PLL1_REG_BASE         (0x01E1A000)

//-----------------------------------------------------------------------------
// DDR registers
//-----------------------------------------------------------------------------
#define DDR_REG_BASE          (0xB0000000)
#define DDR_MEM_BASE          (0xC0000000)
#define DDR_MEM_SIZE          (0x08000000)	// 32KB
//#define DDR_MEM_SIZE          (0x08000000)	// 128MB

//-----------------------------------------------------------------------------
// EMIFA registers
//-----------------------------------------------------------------------------
#define EMIFA_REG_BASE        (0x68000000)

//-----------------------------------------------------------------------------
// EMIFA EXTERNAL registers 自己定义的 
//-----------------------------------------------------------------------------
//#define  EMA_CE0_REG     		(0x40000000)
#define  EMA_CE2_REG     		(0x60000000)
#define  EMA_CE4_REG     		(0x64000000)
#define  EMA_CE5_REG    		(0x66000000) 

//-----------------------------------------------------------------------------
// GPIO registers
//-----------------------------------------------------------------------------
#define GPIO_REG_BASE         (0x01E26000)
#define GPIO_BANK_OFFSET      (0x28)

#define GPIO_REV              (GPIO_REG_BASE)
#define GPIO_BINTEN           (GPIO_REG_BASE + 0x08)

#define GPIO_BANK01_BASE      (GPIO_REG_BASE + 0x10)
#define GPIO_BANK23_BASE      (GPIO_BANK01_BASE + GPIO_BANK_OFFSET)
#define GPIO_BANK45_BASE      (GPIO_BANK23_BASE + GPIO_BANK_OFFSET)
#define GPIO_BANK67_BASE      (GPIO_BANK45_BASE + GPIO_BANK_OFFSET)
#define GPIO_BANK8_BASE       (GPIO_BANK67_BASE + GPIO_BANK_OFFSET)

#define GPIO_BUFF_OE_BANK        (2)
#define GPIO_BUFF_OE_PIN         (6)
#define PINMUX_GPIO_BUFF_OE_REG  (6)
#define PINMUX_GPIO_BUFF_OE_MASK (0x000000F0)
#define PINMUX_GPIO_BUFF_OE_VAL  (0x00000080)

//-----------------------------------------------------------------------------
// Timer registers
//-----------------------------------------------------------------------------
#define TIMER0_REG_BASE       (0x01C20000)
#define TIMER1_REG_BASE       (0x01C21000)
#define TIMER2_REG_BASE		  (0x01F0C000)
#define TIMER3_REG_BASE		  (0x01F0D000)	

//-----------------------------------------------------------------------------
// UART registers
//-----------------------------------------------------------------------------
#define UART0_REG_BASE        (0x01C42000)
#define UART1_REG_BASE        (0x01D0C000)
#define UART2_REG_BASE        (0x01D0D000)

//-----------------------------------------------------------------------------
// SPI registers
//-----------------------------------------------------------------------------
#define SPI0_REG_BASE         (0x01C41000)
#define SPI1_REG_BASE         (0x01F0E000)

//-----------------------------------------------------------------------------
// I2C registers
//-----------------------------------------------------------------------------
#define I2C0_REG_BASE         (0x01C22000)
#define I2C1_REG_BASE         (0x01E28000)

//-----------------------------------------------------------------------------
// EMAC registers
//-----------------------------------------------------------------------------
#define EMAC_RAM_BASE         (0x01E20000)
#define EMAC_CTRL_REG_BASE    (0x01E22000)
#define EMAC_REG_BASE         (0x01E23000)
#define MDIO_REG_BASE         (0x01E24000)

//-----------------------------------------------------------------------------
// MMCSD registers
//-----------------------------------------------------------------------------
#define MMCSD0_REG_BASE       (0x01C40000)
#define MMCSD1_REG_BASE       (0x01E1B000)

//-----------------------------------------------------------------------------
// MCASP registers
//-----------------------------------------------------------------------------
#define MCASP_REG_BASE        (0x01D00000)

//-----------------------------------------------------------------------------
// MCBSP registers
//-----------------------------------------------------------------------------
#define MCBSP0_REG_BASE        (0x01D10000)
#define MCBSP1_REG_BASE        (0x01D11000)

//-----------------------------------------------------------------------------
// USB registers
//-----------------------------------------------------------------------------
#define USB_OTG_REG_BASE      (0x01E00000)
#define USB_HOST_REG_BASE     (0x01E25000)

//-----------------------------------------------------------------------------
// VPIF registers
//-----------------------------------------------------------------------------
#define VPIF_REG_BASE            (0x01E17000)
#define VPIF_CAP_CH0_REG_BASE    (0x01E17040)
#define VPIF_CAP_CH1_REG_BASE    (0x01E17080)
#define VPIF_DISP_CH2_REG_BASE   (0x01E170C0)
#define VPIF_DISP_CH3_REG_BASE   (0x01E17140)

//-----------------------------------------------------------------------------
// LCDC registers
//-----------------------------------------------------------------------------
#define LCDC_REG_BASE         (0x01E13000)

//-----------------------------------------------------------------------------
// uPP registers
//-----------------------------------------------------------------------------
#define UPP_REG_BASE          (0x01E16000)

//-----------------------------------------------------------------------------
// RTC registers
//-----------------------------------------------------------------------------
#define RTC_REG_BASE          (0x01C23000)

//-----------------------------------------------------------------------------
// ECAP registers
//-----------------------------------------------------------------------------
#define ECAP_0_REG_BASE          (0x01F06000)
#define ECAP_1_REG_BASE          (0x01F07000)
#define ECAP_2_REG_BASE          (0x01F08000)

//-----------------------------------------------------------------------------
// EHRPWM registers
//-----------------------------------------------------------------------------
#define EHRPWM_0_REG_BASE          (0x01F00000)
#define EHRPWM_1_REG_BASE          (0x01F02000)
//-----------------------------------------------------------------------------
// HRPWM registers
//-----------------------------------------------------------------------------
#define HRPWM_0_REG_BASE           (0x01F01000)
#define HRPWM_1_REG_BASE           (0x01F03000)

//-----------------------------------------------------------------------------
// EDMA3 0 registers
//-----------------------------------------------------------------------------
#define EDMA30CC_0_REG_BASE         (0x01C00000)
#define EDMA30TC_0_REG_BASE         (0x01C08000)
#define EDMA30TC_1_REG_BASE         (0x01C08400)

//-----------------------------------------------------------------------------
// EDMA3 1 registers
//-----------------------------------------------------------------------------
#define EDMA31CC_0_REG_BASE         (0x01E30000)
#define EDMA31TC_0_REG_BASE         (0x01E38000)

//-----------------------------------------------------------------------------
// DSP_interrput controller registers
//-----------------------------------------------------------------------------
#define DSP_INTC_REG_BASE          (0x01800000)

//-----------------------------------------------------------------------------
// Public Function Prototypes
//-----------------------------------------------------------------------------
uint32_t EVMOMAPL138_init(void);
uint32_t EVMOMAPL138_initRAM(void);
void EVMOMAPL138_enableDsp(void);
void EVMOMAPL138_pinmuxConfig(uint32_t in_reg, uint32_t in_mask, uint32_t in_val);
void EVMOMAPL138_lpscTransition(psc_regs_t *psc_reg, uint32_t in_domain, uint8_t in_module, uint8_t in_next_state);
void UTIL_printMem(uint32_t begin_addr, uint8_t *buffer, uint32_t length, uint8_t continuation);
uint8_t UTIL_isUIBoardAttached(void);
uint32_t config_pll0(uint32_t clkmode, uint32_t pllm, uint32_t postdiv, uint32_t plldiv1, uint32_t plldiv2, uint32_t plldiv3, uint32_t plldiv7);
uint32_t config_pll1(uint32_t pllm, uint32_t postdiv, uint32_t plldiv1, uint32_t plldiv2, uint32_t plldiv3);

#endif
