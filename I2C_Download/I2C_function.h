/*
 * I2C_function.h
 *
 *  Created on: 2016-10-26
 *      Author: zhjl769
 */

#ifndef I2C_FUNCTION_H_
#define I2C_FUNCTION_H_

/*
 * 将data[]缓冲区的count个字节写入从IIC中的chipAdd地址
 */
void I2C_Write(Uint16 chipAdd, Uint8 *data, Uint16 count, Uint16 slaveaddress)
{
	int i;
	I2C_RSET(I2CCNT,count + 2);      // 设置需发送的字节数
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
		I2C_writeByte(data[i]);
		while(I2C_FGET(I2CSTR,NACK));//检测从IIC是否回复一个ACK位
		while(! I2C_xrdy());
	}
	while(I2C_FGET(I2CSTR,BB));
}

/*
 * 将IIC中的chipAdd地址开始的count个字节读到缓冲区data[]中
 */
void I2C_Read(Uint16 chipAdd, Uint8 *data, Uint16 count, Uint16 slaveaddress)
{
	int i;
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
	I2C_RSET(I2CCNT, count);// 设置需接收的字节数
	I2C_RSET(I2CSAR, slaveaddress);// 设置总线上的从IIC的地址
	I2C_RSET(I2CMDR, _I2C_I2CMDR_TRX_CLR);// 设置总线上的从IIC为接收模式
	I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

	while(!I2C_rrdy());
	for (i = 0;i < count ;i ++)   //接收数据
	{
		// Wait for receive data to come back
		while(!I2C_rrdy());
		// Copy the data out
		data[i] = I2C_readByte();
	}
	while(I2C_FGET(I2CSTR,BB));
}


#endif /* I2C_FUNCTION_H_ */
