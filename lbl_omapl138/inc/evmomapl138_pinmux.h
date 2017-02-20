//-----------------------------------------------------------------------------
// \file    evmomapl138_pinmux.h
// \brief   OMAP-L138 internal registers & bit definitions.
//
//-----------------------------------------------------------------------------

#ifndef EVMOMAPL138_PINMUX_H
#define EVMOMAPL138_PINMUX_H

//MII ETHERNET
#define PINMUX_MII_REG_0         (2)
#define PINMUX_MII_MASK_0        (0xFFFFFFF0)
#define PINMUX_MII_VAL_0         (0x88888880)
#define PINMUX_MII_REG_1         (3)
#define PINMUX_MII_MASK_1        (0xFFFFFFFF)
#define PINMUX_MII_VAL_1         (0x88888888)
#define PINMUX_MII_EN_REG 		 (6)
#define PINMUX_MII_EN_MASK    	 (0x000000F0)
#define PINMUX_MII_EN_VAL     	 (0x00000080)

//RMII ETHERNET
#define PINMUX_RMII_REG_0        (14)
#define PINMUX_RMII_MASK_0       (0xFFFFFF00)
#define PINMUX_RMII_VAL_0        (0x88888800)
#define PINMUX_RMII_REG_1        (15)
#define PINMUX_RMII_MASK_1       (0x000000FF)
#define PINMUX_RMII_VAL_1        (0x00000080)


//MDIO
#define PINMUX_MDIO_REG          (4)
#define PINMUX_MDIO_MASK         (0x000000FF)
#define PINMUX_MDIO_VAL          (0x00000088)


//ADC 
#define PINMUX_ADC_REG_0        (15)
#define PINMUX_ADC_MASK_0       (0xFFFFFF00)
#define PINMUX_ADC_VAL_0        (0x44444400)
#define PINMUX_ADC_REG_1        (16)
#define PINMUX_ADC_MASK_1       (0x000000FF)
#define PINMUX_ADC_VAL_1        (0x00000044)
#define PINMUX_ADC_REG_2        (18)
#define PINMUX_ADC_MASK_2       (0x00FFFFFF)
#define PINMUX_ADC_VAL_2        (0x00444444)

//DAC
#define PINMUX_DAC_REG_0        (13)
#define PINMUX_DAC_MASK_0       (0xFFFF0000)
#define PINMUX_DAC_VAL_0        (0x44440000)
#define PINMUX_DAC_REG_1        (14)
#define PINMUX_DAC_MASK_1       (0xFFFFFF00)
#define PINMUX_DAC_VAL_1        (0x44444400)
#define PINMUX_DAC_REG_2        (15)
#define PINMUX_DAC_MASK_2       (0x000000FF)
#define PINMUX_DAC_VAL_2        (0x00000044)
#define PINMUX_DAC_REG_3        (16)
#define PINMUX_DAC_MASK_3       (0x000000FF)
#define PINMUX_DAC_VAL_3        (0x00000044)
#define PINMUX_DAC_REG_4        (17)
#define PINMUX_DAC_MASK_4       (0x000000FF)
#define PINMUX_DAC_VAL_4        (0x00000044)

//SVIDEO_IN
#define PINMUX_SVID_IN_REG_2	(14)		
#define PINMUX_SVID_IN_MASK_2	(0X000000F0)
#define PINMUX_SVID_IN_VAL_2	(0X00000010)
#define PINMUX_SVID_IN_REG_0	(15)		
#define PINMUX_SVID_IN_MASK_0	(0XFFFFFF00)
#define PINMUX_SVID_IN_VAL_0	(0X11111100)
#define PINMUX_SVID_IN_REG_1	(16)
#define PINMUX_SVID_IN_MASK_1	(0X000000FF)
#define PINMUX_SVID_IN_VAL_1	(0X00000011)


//SVIDEO_OUT

#define PINMUX_SVID_OUT_REG_0	(16)
#define PINMUX_SVID_OUT_MASK_0	(0XFFFFFF00)
#define PINMUX_SVID_OUT_VAL_0	(0X11111100)
#define PINMUX_SVID_OUT_REG_1	(17)
#define PINMUX_SVID_OUT_MASK_1	(0XFFFFFFFF)
#define PINMUX_SVID_OUT_VAL_1	(0X11111111)
#define PINMUX_SVID_OUT_REG_2	(18)
#define PINMUX_SVID_OUT_MASK_2	(0X000000FF)
#define PINMUX_SVID_OUT_VAL_2	(0X00000011)
#define PINMUX_SVID_OUT_REG_3	(19)
#define PINMUX_SVID_OUT_MASK_3	(0X00FFFF00)
#define PINMUX_SVID_OUT_VAL_3	(0X00101000)


//CVIDEO_IN
#define PINMUX_CVID_IN_REG_0	(14)
#define PINMUX_CVID_IN_MASK_0	(0XFFFFFF0F)
#define PINMUX_CVID_IN_VAL_0	(0X11111101)
#define PINMUX_CVID_IN_REG_1	(15)
#define PINMUX_CVID_IN_MASK_1	(0X000000FF)
#define PINMUX_CVID_IN_VAL_1	(0X00000011)

//TODO DETERMINE IF THE TWO VIDEO OUT PMUX DEFS ARE CORRECT THERE IS A CONFLICT BETWEEN MATRIX AND SCHEMATIC
//CVIDEO_OUT
#define PINMUX_CVID_OUT_REG_0	(16)
#define PINMUX_CVID_OUT_MASK_0	(0XFFFFFF00)
#define PINMUX_CVID_OUT_VAL_0	(0X11111100)
#define PINMUX_CVID_OUT_REG_1	(17)
#define PINMUX_CVID_OUT_MASK_1	(0XFFFFFFFF)
#define PINMUX_CVID_OUT_VAL_1	(0X11111111)
#define PINMUX_CVID_OUT_REG_2	(18)
#define PINMUX_CVID_OUT_MASK_2	(0X000000FF)
#define PINMUX_CVID_OUT_VAL_2	(0X00000011)
#define PINMUX_CVID_OUT_REG_3	(19)
#define PINMUX_CVID_OUT_MASK_3	(0X00F0F000)
#define PINMUX_CVID_OUT_VAL_3	(0X00101000)

//CAMERA
#define PINMUX_CAMERA_REG_0		(14)
#define PINMUX_CAMERA_MASK_0	(0XFFFFFFFF)
#define PINMUX_CAMERA_VAL_0		(0X11111111)
#define PINMUX_CAMERA_REG_1		(15)
#define PINMUX_CAMERA_MASK_1	(0XFF00FFFF)
#define PINMUX_CAMERA_OUT_VAL_1	(0X11001111)
#define PINMUX_CAMERA_REG_2		(16)
#define PINMUX_CAMERA_MASK_2	(0X000000FF)
#define PINMUX_CVID_OUT_VAL_2	(0X00000011)

//CHARACTER LCD
#define PINMUX_LCD_D_BUS_0_REG      16
#define PINMUX_LCD_D_BUS_0_MASK     (0xFFFFFF00)
#define PINMUX_LCD_D_BUS_0_VALUE    (0x22222200)
#define PINMUX_LCD_D_BUS_1_REG      17
#define PINMUX_LCD_D_BUS_1_MASK     (0xFFFFFFFF)
#define PINMUX_LCD_D_BUS_1_VALUE    (0x22222222)
#define PINMUX_LCD_D_BUS_2_REG      18
#define PINMUX_LCD_D_BUS_2_MASK     (0x000000FF)
#define PINMUX_LCD_D_BUS_2_VALUE    (0x00000022)
#define PINMUX_LCD_CONTROL_0_REG    18
#define PINMUX_LCD_CONTROL_0_MASK   (0xFF000000)
#define PINMUX_LCD_CONTROL_0_VALUE  (0x22000000)
#define PINMUX_LCD_CONTROL_1_REG    19
#define PINMUX_LCD_CONTROL_1_MASK   (0x000000FF)
#define PINMUX_LCD_CONTROL_1_VALUE  (0x00000022)

//GRAPHIC LCD
//#define PINMUX_LCD_D_BUS_0_REG		(16)
#define PINMUX_LCD_D_BUS_0_MASK		(0xFFFFFF00)
#define PINMUX_LCD_D_BUS_0_VALUE	(0x22222200)
//#define PINMUX_LCD_D_BUS_1_REG		(17)
#define PINMUX_LCD_D_BUS_1_MASK		(0xFFFFFFFF)
#define PINMUX_LCD_D_BUS_1_VALUE	(0x22222222)
//#define PINMUX_LCD_D_BUS_2_REG		(18)
#define PINMUX_LCD_D_BUS_2_MASK		(0x000000FF)
#define PINMUX_LCD_D_BUS_2_VALUE	(0x00000022)
//#define PINMUX_LCD_CONTROL_0_REG	(18)
#define PINMUX_LCD_CONTROL_0_MASK	(0xFF000000)
#define PINMUX_LCD_CONTROL_0_VALUE	(0x22000000)
//#define PINMUX_LCD_CONTROL_1_REG	(1)
#define PINMUX_LCD_CONTROL_1_MASK	(0x000000FF)
#define PINMUX_LCD_CONTROL_1_VALUE	(0x00000022)

/*************************************************
此处是对GPIO寄存器0的配置
*************************************************/

#define PINMUX0_GPIO0H_REG             (0)
#define PINMUX0_GPIO0H_MASK       (0xFFFFFFFF)
#define PINMUX0_GPIO0H_VAL        (0xFF808888)


/*************************************************
此处是对GPIO寄存器8的配置
*************************************************/

#define PINMUX8_GPIO3L_REG             (8)    //主要是GPIO0-15
#define PINMUX8_GPIO3L_MASK       (0xFFFFFFFF)
#define PINMUX8_GPIO3L_VAL        (0xFFFF8888)//定义3-4、5、6、7为一般的GPIO端口

/*************************************************
此处是对GPIO寄存器 9的配置
*************************************************/

#define PINMUX7_GPIO3H_REG             (7)    //主要是GPIO16-32
#define PINMUX7_GPIO3H_MASK       (0xFFFFFFFF)
#define PINMUX7_GPIO3H_VAL        (0xFF111111) //定义3-9为一般的GPIO端口 3-12/13/15为片选信号
                                               //EMA-WE/OE

/*************************************************
此处是对GPIO寄存器 6的配置
*************************************************/

#define PINMUX6_GPIO2L_REG             (6)    //
#define PINMUX6_GPIO2L_MASK       (0xFFFFFFFF)
#define PINMUX6_GPIO2L_VAL        (0xFFFFFFFF) 

/*************************************************
此处是对GPIO寄存器 13的配置
*************************************************/

#define PINMUX13GPIO6H_REG             (13)    //
#define PINMUX13_GPIO6H_MASK       (0xFFFFFFFF)
#define PINMUX13_GPIO6H_VAL        (0xFFFF1111) //定义复用功能

/*************************************************
此处是对GPIO寄存器 12的配置
*************************************************/

#define PINMUX12_GPIO5L_REG             (12)    //
#define PINMUX12_GPIO5L_MASK       (0xFFFFFFFF)
#define PINMUX12_GPIO5L_VAL        (0x11111FF8) //定义复用功能

/*************************************************
此处是对GPIO寄存器 11的配置
*************************************************/

#define PINMUX11_GPIO5H_REG             (11)    //
#define PINMUX11_GPIO5H_MASK       (0xFFFFFFFF)
#define PINMUX11_GPIO5H_VAL        (0x8FFFFF8F) //定义复用功能

/*************************************************
此处是对GPIO寄存器 9的配置
*************************************************/

#define PINMUX9_GPIO4H_REG             (9)        //
#define PINMUX9_GPIO4H_MASK       (0xFFFFFFFF)
#define PINMUX9_GPIO4H_VAL        (0x11111111)    //定义复用功能

/*************************************************
此处是对GPIO寄存器 5的配置
*************************************************/

#define PINMUX5_GPIO2H_REG             (5)        //
#define PINMUX5_GPIO2H_MASK       (0xFFFFFFFF)
#define PINMUX5_GPIO2H_VAL        (0x11FFFFFF)    //定义复用功能


#endif
