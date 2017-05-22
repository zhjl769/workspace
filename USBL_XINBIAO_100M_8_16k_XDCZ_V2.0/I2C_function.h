/*
 * I2C_function.h
 *
 *  Created on: 2016-10-26
 *      Author: zhjl769
 */

#ifndef I2C_FUNCTION_H_
#define I2C_FUNCTION_H_
//-----------------------I2Cר���ӳٺ���---------------------------
void delay(long x)
{
	long i;
	for(i = 0;i < x;i ++)
	{
	}
}
//-----------------------I2Cר��У�麯��---------------------------
int check( Uint8 *sword, Uint8 *dword, int len)
{
	while (len > 0)
	{
		if (*sword != *dword)
		{
			return -1;
		}
		sword++;
		dword++;
		len--;
	}
	return 1;
}


/*
 * ��data[]��������count���ֽ�д���IIC�е�chipAdd��ַ
 */
void I2C_Write(Uint16 chipAdd, Uint8 *data, Uint16 count, Uint16 slaveaddress)
{
	int i;
	I2C_RSET(I2CCNT,count + 2);      // �����跢�͵��ֽ���
	I2C_RSET(I2CSAR, slaveaddress);  // ���������ϵĴ�IIC�ĵ�ַ
	I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_TRX_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);
	// ���ʹ�IIC�еĴ洢��ַ
	I2C_writeByte(chipAdd >> 8);
	while(I2C_FGET(I2CSTR,NACK));//����IIC�Ƿ�ظ�һ��ACKλ
	while(!I2C_xrdy());

	I2C_writeByte(chipAdd & 0xff);
	while(I2C_FGET(I2CSTR,NACK));//����IIC�Ƿ�ظ�һ��ACKλ
	while(!I2C_xrdy());

	// ����count ���ֽ�
	for (i = 0;i < count ;i ++)
	{
		I2C_writeByte(data[i]);
		while(I2C_FGET(I2CSTR,NACK));//����IIC�Ƿ�ظ�һ��ACKλ
		while(! I2C_xrdy());
	}
	while(I2C_FGET(I2CSTR,BB));
}

/*
 * ��IIC�е�chipAdd��ַ��ʼ��count���ֽڶ���������data[]��
 */
void I2C_Read(Uint16 chipAdd, Uint8 *data, Uint16 count, Uint16 slaveaddress)
{
	int i;
	I2C_RSET(I2CCNT, 2);
	I2C_RSET(I2CMDR, 0x0020);//��λI2CMDR��ʹ��I2Cģ�飬��������
	I2C_RSET(I2CSAR, slaveaddress);// ���������ϵĴ�IIC�ĵ�ַ
	I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_TRX_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

	I2C_writeByte(chipAdd >> 8);// ���ʹ�IIC�еĴ洢��ַ
	while(I2C_FGET(I2CSTR,NACK));//����IIC�Ƿ�ظ�һ��ACKλ
	while(!I2C_xrdy());

	I2C_writeByte(chipAdd & 0xff);
	while(I2C_FGET(I2CSTR,NACK));//����IIC�Ƿ�ظ�һ��ACKλ
	while(!I2C_xrdy());

	I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_CLR);//��λI2CMDR��STTλ
	I2C_RSET(I2CCNT, count);// ��������յ��ֽ���
	I2C_RSET(I2CSAR, slaveaddress);// ���������ϵĴ�IIC�ĵ�ַ
	I2C_RSET(I2CMDR, _I2C_I2CMDR_TRX_CLR);// ���������ϵĴ�IICΪ����ģʽ
	I2C_RSET(I2CMDR, _I2C_I2CMDR_STT_MASK|_I2C_I2CMDR_STP_MASK|_I2C_I2CMDR_IRS_MASK|_I2C_I2CMDR_MST_MASK);

	while(!I2C_rrdy());
	for (i = 0;i < count ;i ++)   //��������
	{
		// Wait for receive data to come back
		while(!I2C_rrdy());
		// Copy the data out
		data[i] = I2C_readByte();
	}
	while(I2C_FGET(I2CSTR,BB));
}

void Read_const_I2C()
{
	Uint32 i, j ;
	Uint32 ii = 0;				//���������ݼ���
	Uint32 addr = 0;   			//��IIC�豸����ʼ���ݵ�ַ
	Uint8 Rtmp[Page_Length];	//��IIC�豸��ȡ���ݵĻ���ռ�
	/*������Ԥ������*/
	for(i=0;i<12416;i++)
	{
		Pwm_List[i] = 0;
	}
	/*��ʼ��ȡ*/
	for(i=0;i<97;i++)//����97�����ݣ�ÿ������128�ֽ�
	{
		I2C_Read(addr, Rtmp, sizeof(Rtmp), EEPROM_addr); //����д���128Bytes������
		for (j=0; j < sizeof(Rtmp); j++)
		{
			Pwm_List[ii] = Rtmp[j];					//Pwm_List�е����ݰ��Ƶ���ʱ�ռ�
			ii = ii+1;
		}
	}
}



#endif /* I2C_FUNCTION_H_ */