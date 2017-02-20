/*
 * I2C_function.h
 *
 *  Created on: 2016-10-26
 *      Author: zhjl769
 */

#ifndef I2C_FUNCTION_H_
#define I2C_FUNCTION_H_



void delay(long x)
{
	long i;
	for(i = 0;i < x;i ++)
	{
	}
}

/*
 * 将data[]缓冲区的count个字节写入从IIC中的chipAdd地址
 */
void I2C_Write(Uint16 chipAdd, Uint16 *data, Uint16 count, Uint16 slaveaddress)
{
	Uint32 i, Block,Block1;
	if(count > 64)
	{
		Block = count/64;
		Block1 = count%64;
		for(i = 0; i < Block; i++)
		{
			I2C_RSET(I2CCNT,64 + 2);      // 设置需发送的字节数
			I2C_RSET(I2CSAR, slaveaddress);  // 设置总线上的从IIC的地址
			I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_TRX_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);
			// 发送从IIC中的存储地址
			I2C_writeByte(chipAdd >> 8);
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(!I2C_xrdy());

			I2C_writeByte(chipAdd & 0xff);
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(!I2C_xrdy());

			// 发送64个字节
			for (i = 0; i < 64; i ++)
			{
				I2C_writeByte(data[i] >> 8);
				while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
				while(! I2C_xrdy());

				I2C_writeByte(data[i] & 0xff);
				while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
				while(! I2C_xrdy());
			}
			while(I2C_FGET(I2CSTR,BB));
			delay(60000);
			chipAdd += 128;
		}
		for(i = 0; i < Block1; i++)
		{
			I2C_RSET(I2CCNT,Block1 + 2);      // 设置需发送的字节数
			I2C_RSET(I2CSAR, slaveaddress);  // 设置总线上的从IIC的地址
			I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_TRX_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);
			// 发送从IIC中的存储地址
			I2C_writeByte(chipAdd >> 8);
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(!I2C_xrdy());

			I2C_writeByte(chipAdd & 0xff);
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(!I2C_xrdy());

			// 发送64个字节
			for (i = 0; i < 64; i ++)
			{
				I2C_writeByte(data[i] >> 8);
				while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
				while(! I2C_xrdy());

				I2C_writeByte(data[i] & 0xff);
				while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
				while(! I2C_xrdy());
			}
			while(I2C_FGET(I2CSTR,BB));
			chipAdd += 128;
		}
	}
	else
	{
		I2C_RSET(I2CCNT,(count << 1) + 2);      // 设置需发送的字节数
		I2C_RSET(I2CSAR, slaveaddress);  // 设置总线上的从IIC的地址
		I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_TRX_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);
		// 发送从IIC中的存储地址
		I2C_writeByte(chipAdd >> 8);
		while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
		while(!I2C_xrdy());

		I2C_writeByte(chipAdd & 0xff);
		while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
		while(!I2C_xrdy());

		// 发送count 个字节
		for (i = 0;i < count ;i ++)
		{
			I2C_writeByte(data[i] >> 8);
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(! I2C_xrdy());

			I2C_writeByte(data[i] & 0xff);
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(! I2C_xrdy());
		}
		while(I2C_FGET(I2CSTR,BB));
	}
}

/*
 * 将IIC中的chipAdd地址开始的count个字节读到缓冲区data[]中
 */
void I2C_Read(Uint16 chipAdd, Uint16 *data, Uint16 count, Uint16 slaveaddress)
{
	Uint32 i, Block,Block1;
	if(count > 64)
	{
		Block = count/64;
		Block1 = count%64;
		for(i = 0; i < Block; i++)
		{
			I2C_RSET(I2CCNT, 2);
			I2C_RSET(I2CMDR, 0x0020);//复位I2CMDR，使能I2C模块，其他清零
			I2C_RSET(I2CSAR, slaveaddress);// 设置总线上的从IIC的地址
			I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_TRX_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

			I2C_writeByte(chipAdd >> 8);// 发送从IIC中的存储地址
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(!I2C_xrdy());

			I2C_writeByte(chipAdd & 0xff);
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(!I2C_xrdy());

			I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_CLR);//复位I2CMDR的STT位
			I2C_RSET(I2CCNT, 128);// 设置需接收的字节数
			I2C_RSET(I2CSAR, slaveaddress);// 设置总线上的从IIC的地址
			I2C_RSET(I2CMDR, _I2C_I2CMDR_TRX_CLR);// 设置总线上的从IIC为接收模式
			I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

			while(!I2C_rrdy());
			for (i = 0;i < 64 ;i ++)   //接收数据
			{
				while(!I2C_rrdy());// Wait for receive data to come back
				data[i] = (I2C_readByte() << 8) & 0xff00;// Copy the data out
				while(!I2C_rrdy());// Wait for receive data to come back
				data[i] |= (I2C_readByte() & 0x00ff);// Copy the data out
			}
			while(I2C_FGET(I2CSTR,BB));
			delay(90000);
			chipAdd += 128;
		}
		for(i = 0; i < Block1; i++)
		{
			I2C_RSET(I2CCNT, 2);
			I2C_RSET(I2CMDR, 0x0020);//复位I2CMDR，使能I2C模块，其他清零
			I2C_RSET(I2CSAR, slaveaddress);// 设置总线上的从IIC的地址
			I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_TRX_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

			I2C_writeByte(chipAdd >> 8);// 发送从IIC中的存储地址
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(!I2C_xrdy());

			I2C_writeByte(chipAdd & 0xff);
			while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
			while(!I2C_xrdy());

			I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_CLR);//复位I2CMDR的STT位
			I2C_RSET(I2CCNT, Block1 << 1);// 设置需接收的字节数
			I2C_RSET(I2CSAR, slaveaddress);// 设置总线上的从IIC的地址
			I2C_RSET(I2CMDR, _I2C_I2CMDR_TRX_CLR);// 设置总线上的从IIC为接收模式
			I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

			while(!I2C_rrdy());
			for (i = 0;i < Block1 ;i ++)   //接收数据
			{
				while(!I2C_rrdy());// Wait for receive data to come back
				data[i] = (I2C_readByte() << 8) & 0xff00;// Copy the data out
				while(!I2C_rrdy());// Wait for receive data to come back
				data[i] |= (I2C_readByte() & 0x00ff);// Copy the data out
			}
			while(I2C_FGET(I2CSTR,BB));
			chipAdd += 128;
		}
	}
	else
	{
		I2C_RSET(I2CCNT, 2);
		I2C_RSET(I2CMDR, 0x0020);//复位I2CMDR，使能I2C模块，其他清零
		I2C_RSET(I2CSAR, slaveaddress);// 设置总线上的从IIC的地址
		I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_TRX_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

		I2C_writeByte(chipAdd >> 8);// 发送从IIC中的存储地址
		while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
		while(!I2C_xrdy());

		I2C_writeByte(chipAdd & 0xff);
		while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
		while(!I2C_xrdy());

		I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_CLR);//复位I2CMDR的STT位
		I2C_RSET(I2CCNT, count << 1);// 设置需接收的字节数
		I2C_RSET(I2CSAR, slaveaddress);// 设置总线上的从IIC的地址
		I2C_RSET(I2CMDR, _I2C_I2CMDR_TRX_CLR);// 设置总线上的从IIC为接收模式
		I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

		while(!I2C_rrdy());
		for (i = 0;i < count ;i ++)   //接收数据
		{
			// Wait for receive data to come back
			while(!I2C_rrdy());
			// Copy the data out
			data[i] = (I2C_readByte() << 8) & 0xff00;
			// Wait for receive data to come back
			while(!I2C_rrdy());
			// Copy the data out
			data[i] |= (I2C_readByte() & 0x00ff);
		}
		while(I2C_FGET(I2CSTR,BB));
	}
}

/*DSP的时钟配置*/

PLL_Config PLLconfig = {
		0,    //PLL锁定，与进入IDLE之前的状态保持一致， Initialize After Idle
		2,    // 时钟发生器切换到旁路模式，并重新开始PLL的锁相过程。Initialize On Break
		25,   //输入时钟乘以该值并除以PLLDIV就是时钟频率。 PLL Multiply value//48MHz
		0,   //0，不分频，1,2分频，2三分频，3四分频。 PLL Divide value
};


/*IIC配置*/
I2C_Config I2Cconfig = {
		0x0000, /* I2COAR */
		0xFFFF, /* I2CIER */
		0xFFFF, /* I2CSTR */
		18, /* I2CCLKL */
		12, /* I2CCLKH */   //这两个值决定I2C的写入与读取的速度，(100/(I2CPSC + 1))/(I2CCLKL+I2CCLKH+10)
		0x00FF, /* I2CCNT */
		0x0050, /* I2CSAR */
		0x0020, /* I2CMDR */
		0xFFFF, /* I2CISRC */
		0x0018, /* I2CPSC  100/(I2CPSC + 1) = 4MHz*/
};

I2C_Setup I2Cinit = {
		1, /* 7 or 10 bit address mode */
		0x0000, /* own address − don’t care if master */
		48, /* clkout value (Mhz) */
		100, /* a number between 10 and 400 */
		0, /* number of bits/byte to be received or */
		/* transmitted (8 bits) */
		0, /* DLB mode */
		1 /* FREE mode of operation */
};

#endif /* I2C_FUNCTION_H_ */
