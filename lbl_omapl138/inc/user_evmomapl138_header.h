#ifndef _USER_EVMOMAPL138_H_
#define _USER_EVMOMAPL138_H_

//开始进行用户硬件使用的参数定义
#include "c674xdspinit.h"
#include "constant.h"

// pinmux defines
#define PINMUX_UART0_REG      (3)
#define PINMUX_UART0_MASK     (0xFFFF0000)
#define PINMUX_UART0_VAL      (0x22220000)
#define PINMUX_UART1_REG_0    (0)
#define PINMUX_UART1_MASK_0   (0x00FF0000)
#define PINMUX_UART1_VAL_0    (0x00220000)
#define PINMUX_UART1_REG_1    (4)
#define PINMUX_UART1_MASK_1   (0xFF000000)
#define PINMUX_UART1_VAL_1    (0x22000000)
#define PINMUX_UART2_REG_0    (0)
#define PINMUX_UART2_MASK_0   (0xFF000000)
#define PINMUX_UART2_VAL_0    (0x44000000)
#define PINMUX_UART2_REG_1    (4)
#define PINMUX_UART2_MASK_1   (0x00FF0000)
#define PINMUX_UART2_VAL_1    (0x00220000)

#define UART_FCR_RXFIFTL_1	(0x00000000)
#define UART_FCR_RXFIFTL_4	(0x00000040)
#define UART_FCR_RXFIFTL_8	(0x00000080)
#define UART_FCR_RXFIFTL_14	(0x000000c0)

#define UART_IER_ERBI		(0x00000001)	//UART接收中断使能

#define PINMUX_MCBSP1_REG       (1)
#define PINMUX_MCBSP1_MASK      (0xFFFFFFFF)
#define PINMUX_MCBSP1_VAL       (0x22222220)

// AIC23 Control Register addresses
#define AIC23_LT_LINE_CTL         0x00  // 0
#define AIC23_RT_LINE_CTL         0x01  // 1
#define AIC23_LT_HP_CTL           0x02  // 2
#define AIC23_RT_HP_CTL           0x03  // 3
#define AIC23_ANALOG_AUDIO_CTL    0x04  // 4
#define AIC23_DIGITAL_AUDIO_CTL   0x05  // 5
#define AIC23_POWER_DOWN_CTL      0x06  // 6
#define AIC23_DIGITAL_IF_FORMAT   0x07  // 7
#define AIC23_SAMPLE_RATE_CTL     0x08  // 8
#define AIC23_DIG_IF_ACTIVATE     0x09  // 9
#define AIC23_RESET_REG           0x0F  // F - Writing 0 to this reg triggers reset
#define I2C_PORT_AIC23         (I2C0)

// OMAPL1x EDMA Specific
#define EDMA_EVENT4 4
#define EDMA_EVENT5 5
#define EDMA_EVENT10 10
#define EDMA_EVENT24 24


#define EDMA_EVENT74 74
#define EDMA_EVENT75 75

#define PaRAM_ping_4			(0x4000+32*EDMA_EVENT4)
#define PaRAM_pang_5			(0x4000+32*EDMA_EVENT5)
#define PaRAM_pang_74			(0x4000+32*EDMA_EVENT74)
#define PaRAM_ping_75			(0x4000+32*EDMA_EVENT75)
#define PaRAM_NULL_LINK 	0xFFFF

//该部分在文档OMAP-L138 Applications Processor System Reference Guide.pdf中查找
//寻找GPx的引脚即可查知需要修改哪个寄存器

/*
#define PINMUX_GPIO0H_REG		(0)				
#define PINMUX_GPIO0H_MASK		(0x00000F00)	//GPIO0_13
#define PINMUX_GPIO0H_VAL		(0x00000800)	//gain_type

#define PINMUX_GPIO3H_REG       (7)
#define PINMUX_GPIO3H_MASK      (0xFFFFFFFF)
#define PINMUX_GPIO3H_VAL       (0x11118118)

#define PINMUX_GPIO2L_REG       (6)
#define PINMUX_GPIO2L_MASK      (0xFF000000)
#define PINMUX_GPIO2L_VAL       (0x88000000)

#define PINMUX_GPIO5L_REG       (12)			
#define PINMUX_GPIO5L_MASK      (0x0FFFF0FF)	//GPIO5_3 4 6 7
#define PINMUX_GPIO5L_VAL       (0x01188088)	//3 6:PWM //4 7:CPLD

#define PINMUX_GPIO5H_REG       (11)
#define PINMUX_GPIO5H_MASK      (0xFFFFF0FF)	//GPIO5_8 9 10 11 12
#define PINMUX_GPIO5H_VAL       (0x88888022)	//12:PWM //8 9 10 11:CPLD 

#define PINMUX_GPIO6H_REG       (13)			
#define PINMUX_GPIO6H_MASK      (0x00F00000)	//GPIO6_10
#define PINMUX_GPIO6H_VAL       (0x00800000)

#define PINMUX_GPIO6L_REG       (14)
#define PINMUX_GPIO6L_MASK      (0x000000FF)	//GPIO6_6 7
#define PINMUX_GPIO6L_VAL       (0x00000088)
*/

#define PINMUX_GPIO0H_REG      (0)                    	//管脚复用寄存器GPIO0_8-15
#define PINMUX_GPIO0H_MASK     (0xFFFFFFFF)				//GP0[13] 继电器控制增益
#define PINMUX_GPIO0H_VAL      (0x11888881)

#define PINMUX_GPIO2H_REG      (5)                    	//管脚复用寄存器GPIO2_8-15
#define PINMUX_GPIO2H_MASK     (0xFFFFFFFF)
#define PINMUX_GPIO2H_VAL      (0x11118111)


#define PINMUX_GPIO3H_REG      (7)                    	//管脚复用寄存器GPIO3_8-15
#define PINMUX_GPIO3H_MASK     (0xFFFFFFFF)				//GP3[12] 配置CPLD模式
#define PINMUX_GPIO3H_VAL      (0x11118811)				//GP3[13] 配置CPLD内/同步

#define PINMUX_GPIO3L_REG      (8)                    	//管脚复用寄存器GPIO3_0-7
#define PINMUX_GPIO3L_MASK     (0xFFFFFFFF)
#define PINMUX_GPIO3L_VAL      (0x11118888)

#define PINMUX_GPIO4H_REG      (9)                    	//管脚复用寄存器EMA_D[0]-D[7]
#define PINMUX_GPIO4H_MASK     (0xFFFFFFFF)
#define PINMUX_GPIO4H_VAL      (0x11111111)

#define PINMUX_GPIO5H_REG      (11)				    	//管脚复用寄存器GPIO5_8-15
#define PINMUX_GPIO5H_MASK     (0xFFFFFFFF)				//GP5[9] PWM_P    GP5[13] PWM_Syn
#define PINMUX_GPIO5H_VAL      (0x88888881)				//GP5[11]/[10] 配置CPLD内同步周期

#define PINMUX_GPIO5L_REG      (12)						//管脚复用寄存器 GPIO5_0-7
#define PINMUX_GPIO5L_MASK     (0xFFFFFFFF)				//GP5[6] PWM_N
#define PINMUX_GPIO5L_VAL      (0x11111188) 			//GP5[7] 配置CPLD内同步周期

#define PINMUX_GPIO6H_REG      (13)						//GP6[10] 				
#define PINMUX_GPIO6H_MASK     (0xFFFFFFFF)
#define PINMUX_GPIO6H_VAL      (0x00800000) 

#define PINMUX_GPIO6L_REG      (14)						//管脚复用寄存器 
#define PINMUX_GPIO6L_MASK     (0x000000F0)				//GP6[6] CPLD同步输出DSP
#define PINMUX_GPIO6L_VAL      (0x00000080) 



////////////////////////////////////////////////////////////////////////
/*发射信号引脚功能
*//////////////////////////////////////////////////////////////////////

#define TRANS_GPIO1 GPIO_PIN9
#define TRANS_GPIO2 GPIO_PIN6
#define TRANS_SYNC  GPIO_PIN12 

extern short buffer1[IN_LENGTH];
extern short buffer2[IN_LENGTH];
extern char Trans_flag;

#endif

