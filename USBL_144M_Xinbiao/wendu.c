
/******************* ³õÊ¼»¯ds18b2×Óº¯Êı**************************/ 
void Init_DS18B20()                 //·´»ØÒ»¸öÓ¦´ğĞÅºÅ£¬ÊÇ·ñÍê³É³õÊ¼»¯
  { 
	volatile short flag;
	volatile Uint32 delay;

	flag = 1;
  	while (flag)
	{
	 IODOR |= 0x80;	//ÉèÖÃÎªÊä³ö×´Ì¬
	 IODATA |= 0xc0;   //Êä³ö¸ßµçÆ½
	 for(delay = 0;delay < 24;delay++)
	 {
	 }//ÑÓÊ±5us
     IODATA &= 0x7f;   //Êä³öµÍµçÆ½
     
     for(delay = 0;delay < 2334;delay++)
	 {
	 }//ÑÓÊ±500us                 

     IODATA |= 0xc0;	 //Êä³ö¸ßµçÆ½
                        
	 for(delay = 0;delay < 264;delay++)
	 {
	 }//ÑÓÊ±55us

	 IODOR &= 0x7f;	   //ÉèÖÃÎªÊäÈë×´Ì¬
                    
	 for(delay = 0;delay < 288;delay++)
	 {
	 }//ÑÓÊ±60us

     if( (IODATA & 0x80) == 0)
		 flag = 0;		   //Ó¦´ğĞÅºÅ£¬ÊÇ·ñÍê³É³õÊ¼»¯
                         
	 for(delay = 0;delay <1920;delay++)
	 {
	 }//ÑÓÊ±400us	 

	 }
 }

/*******  ¶Á×Ö½Ú×Óº¯Êı*******/     
short Read(void) 
 { 
	short dat;
      short i=0;
      volatile Uint32 delay; 
      dat = 0;
      for (i=8;i>0;i--) 
      { 
	      dat>>=1;   //¶Áµ½Êı¾İºóÓÒÒÆÒ»Î»£¬Ö»ĞèÒÆ¶¯7´Î
        IODATA &= 0x7f;	 //Êä³öµÍµçÆ½
                                       
		 for(delay = 0;delay < 29;delay++)
		 {
		 }//ÑÓÊ±6us

        IODATA |= 0x80;
		 for(delay = 0;delay <39;delay++)
		 {
		 }//ÑÓÊ±8us
        IODOR &= 0x7f;	    //ÉèÖÃÎªÊäÈë×´Ì¬
                          
		 for(delay = 0;delay < 24;delay++)
		 {
		 }//ÑÓÊ±5us

        if((IODATA&0x80) == 0x80)     // Èç¹ûÊäÈëÎª¸ßµçÆ½£¬¶Á1
	    {
	    	dat|=0x80;			//È¡»Ø×î¸ßÎ»Êı¾İ
		}
		 for(delay = 0;delay < 240;delay++)
		 {
		 }//ÑÓÊ±50us

		IODOR |= 0x80;			//ÉèÖÃÎªÊä³ö×´Ì¬    
		IODATA |= 0x80;    
  
		 for(delay = 0;delay < 48;delay++)
		 {
		 }//ÑÓÊ±10us

     }
   return(dat); 
 } 

/*******  Ğ´×Ö½Ú×Óº¯Êı*******/ 
void Write(unsigned char cmd)
  { 
       volatile Uint32 delay;
       short i=0; 
	   IODOR |= 0x80;

	   IODATA &= 0x7f;

		 for(delay = 0;delay < 24;delay++)
		 {
		 }//ÑÓÊ±5us

    for (i=8; i>0; i--) 
     {    
	   IODOR |= 0x80;

       IODATA &= 0x7f;

		 for(delay = 0;delay < 29;delay++)
		 {
		 }//ÑÓÊ±6us

       if(cmd&0x01)
	      IODATA |= 0x80;                      //Ğ´ÈëÒ»Î»Êı¾İ 
	   else
	      IODATA &= 0x7f;
		 for(delay = 0;delay < 240;delay++)
		 {
		 }//ÑÓÊ±50us
       IODATA |= 0x80;

		 for(delay = 0;delay < 48;delay++)
		 {
		 }//ÑÓÊ±10us
       cmd >>= 1;    	   //ÓÒÒÆÒ»Î»Êı¾İ                               
   } 
} 

/*         ¶ÁĞòÁĞºÅº¯Êı             */
void read_serial(void)
{
	short i,j;
	volatile Uint32 delay;
	for(j=0;j<8;j++)
	   { 
	     serial[j]=0;
	     for(i=8;i>0;i--) 
	         { 
	          IODOR |= 0x80;	//ÉèÖÃÎªÊä³ö×´Ì¬ 
              IODATA |= 0x80;    //Êä³ö¸ßµçÆ½                             
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//ÑÓÊ±5us

	          serial[j]>>=1;                       //¶Áµ½Êı¾İºóÓÒÒÆÒ»Î»
              IODATA &= 0x7f;	   //Êä³öµÍµçÆ½
							  
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//ÑÓÊ±5us	                                       

              IODOR &= 0x7f;	   //ÉèÖÃÎªÊäÈë×´Ì¬
                        
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//ÑÓÊ±5us

			  if((IODATA&0x80) == 0x80)
			  {
				  serial[j]|=0x80;				  //È¡»Ø×î¸ßÎ»Êı¾İ
			  }
			 for(delay = 0;delay < 240;delay++)
			 {
			 }//ÑÓÊ±50us                              

	         }
	    }
 }

 /******************************************/
/*             ·¢ĞòÁĞºÅº¯Êı               */
/******************************************/

void send_serial()
{
	short i,j,temp;
	volatile Uint32 delay; 
     for (j=0;j<8;j++)
        { 
		temp = serial_x[j];
		IODOR |= 0x80;
		IODATA &= 0x7f;

		 for(delay = 0;delay < 10;delay++)
		 {
		 }//ÑÓÊ±2us

         for(i=8; i>0; i--) 
            { 
			IODOR |= 0x80;
			IODATA &= 0x7f;

			 for(delay = 0;delay < 24;delay++)
			 {
			 }//ÑÓÊ±5us
            //ÑÓÊ±5Î¢Ãë£¬·¢ËÍ¿ªÊ¼
             if(temp&0x01)              //Ğ´Èë×îµÍÎ»Êı¾İ
				IODATA |= 0x80;
	   		 else
				 IODATA &= 0x7f;

			 for(delay = 0;delay < 264;delay++)
			 {
			 }//ÑÓÊ±55us

 			IODATA |= 0x80;

			 for(delay = 0;delay < 24;delay++)
			 {
			 }//ÑÓÊ±5us

             temp>>=1;                  //ÓÒÒÆÒ»Î»Êı¾İ
            } 
        } 
}  
 
/******************************************/
/*            ¶Á³öÎÂ¶Èº¯Êı                */ 
/******************************************/ 
/*»ñÈ¡ÎÂ¶ÈĞÅÏ¢
	º¯Êı¹¦ÄÜ£ºÀûÓÃGPIO1¿ØÖÆDS18B20»ñÈ¡ÎÂ¶ÈĞÅÏ¢
*/

void get_temperature()
{
	short  temp_data0,temp_data1,tem_short; 
	short  translate_OK;
	float tt;
	volatile Uint32 delay; 
	temp_data0 = 0;
	temp_data1 = 0;
	tt = 0.0;
	IODOR |= 0x80;
	IODATA |= 0x80;
    
    //******Ğ´¼Ä´æÆ÷******
	// ³õÊ¼»¯
	Init_DS18B20();                       
	 for(delay = 0;delay < 960;delay++)
	 {
	 }//ÑÓÊ±200us
    // ·¢ËÍÌø¹ıROMµÄIDÆ¥ÅäµÄÃüÁî 
    Write(0xcc);     
    // ·¢ËÍĞ´¼Ä´æÆ÷ÃüÁî 
    Write(0x4E);                         
    Write(0x60);   // TH ×î´ó125ÉãÊÏ¶È
    Write(0xCE);   // TL X-2^8=-50   X=CE
    Write(0x1F);   // ÅäÖÃ¼Ä´æÆ÷, R1 R0 ·Ö±æÂÊÅäÖÃ 9bit

	//******Æô¶¯ÎÂ¶È×ª»»******
	// ³õÊ¼»¯
    Init_DS18B20();
	// ·¢ËÍÌø¹ıROMµÄIDÆ¥ÅäµÄÃüÁî 
    Write(0xcc); 
    // ·¢ËÍÆô¶¯ÎÂ¶È×ª»»ÃüÁî
    Write(0x44);
    

   //******µÈ´ıÎÂ¶È×ª»¯Íê±Ï******                                
//    delay = 500000;  // IOÏß±£³ÖÖÁÉÙ500ms(12bitÊ±)¸ßµçÆ½£¬ÒÔÍê³ÉÎÂ¶È×ª»»
//   	Delay_diff_1us(&delay);
   do
  {
    translate_OK=Read();
   }while(translate_OK != 0xFF);// ²âÊÔ×ª»»ÊÇ·ñ½áÊø£º·´¸´¶ÁÈ¡×ÜÏß£¬Ö±µ½Á¬Ğø8¸ö1³öÏÖ±êÖ¾×ª»»½áÊø

	//******¶ÁÈ¡ÎÂ¶È******
    // ÎÂ¶È×ª»»Íê³É£¬ÔÙ´Î³õÊ¼»¯
    Init_DS18B20();
                           
//    Write(0x55);     // ·¢ËÍ·¢ĞòÁĞºÅÃüÁî
//    send_serial();   // ·¢ĞòÁĞºÅ

    // ·¢ËÍÌø¹ıROMµÄIDÆ¥ÅäµÄÃüÁî
	Write(0xcc);
	                            	
  	// ·¢ËÍ¶ÁÈ¡ÎÂ¶ÈÃüÁî
    Write(0xbe);   
                           
    // Á¬Ğø¶ÁÈ¡Á½¸ö×Ö½ÚÊı¾İ£¬temp_data0µÍÎ»
    temp_data0=Read();                    
    temp_data1=Read(); 
   
    //******ÎÂ¶È»»Ëã******   
	IODOR |= 0x80;
	IODATA |= 0x80;
    tem_short=temp_data1; 
    tem_short<<=8;                                              
    tem_short=(tem_short|temp_data0);     //Á½×Ö½ÚºÏ³ÉÒ»¸öÕûĞÍ±äÁ¿ 
//    tt=tem_short*0.0625;                  //µÃµ½ÕæÊµÊ®½øÖÆÎÂ¶ÈÖµ£¬¾«È·µ½0.0625¶È
//    tt=tt*10+0.5;                         //·Å´óÊ®±¶£¬½«Ğ¡ÊıµãºóµÚÒ»Î»Ò²×ª»»Îª¿ÉÏÔÊ¾Êı×Ö£¬ËÄÉáÎåÈë 
    tt=tem_short>>3;    //µÍ3Î»²»Æğ×÷ÓÃ
    tt=tt*0.5;                  //µÃµ½ÕæÊµÊ®½øÖÆÎÂ¶ÈÖµ£¬¾«È·µ½0.5¶È
	tt=tt*10;                         //·Å´óÊ®±¶£¬½«Ğ¡ÊıµãºóµÚÒ»Î»Ò²×ª»»Îª¿ÉÏÔÊ¾Êı×Ö£¬ËÄÉáÎåÈë
    temperature = tt/10;                  //·µ»ØÕæÊµÎÂ¶ÈÖµ 
} 


void uart_config()
{
	memset(SD_write,0,sizeof(short)*256);
	SD_write[0] = 0x0ee;   //·¢ËÍ×Ö½ÚÍ·
	SD_write[1] = transponder_mode;//¹¤×÷Ä£Ê½
	SD_write[2] = tongbu_flag;//ÃüÁîÎ»
	SD_write[3] = 0x0bb;//Ğ£ÑéÎ»
	SD_write[4] = (NOISE_TH_NEW) & 0x0ff;//¼ì²âÃÅÏŞµÍ8bit
	SD_write[5] = (NOISE_TH_NEW>>8) & 0x0ff;//¼ì²âÃÅÏŞ¸ß8bit
	SD_write[6] = responder_period/10;//Ğ£ÑéÎ»
	SD_write[7] = 0x0cc;//Ğ£ÑéÎ»
	SD_write[8] = ((Time_Delay/1000)) & 0x0ff;//×ª·¢Ê±ÑÓµÍ8bit
	SD_write[9] = ((Time_Delay/1000)>>8) & 0x0ff;//×ª·¢Ê±ÑÓ¸ß8bit
	SD_write[10] = 0x0cc;//Ğ£ÑéÎ»
	SD_write[11] = 0x01;//Ğ£ÑéÎ»
	SD_write[12] = (voltage_AD) & 0x0ff;//µçÑ¹Á¿»¯ÖµµÍ8bit
	SD_write[13] = (voltage_AD>>8) & 0x0ff;//µçÑ¹Á¿»¯Öµ¸ß8bit
	SD_write[14] = 0x01;//Ğ£ÑéÎ»
	SD_write[15] = (RS485_DATA[5]) & 0xff;
	SD_write[16] = ((RS485_DATA[4])) & 0xff;
	SD_write[17] = ((RS485_DATA[3])) & 0xff;
	SD_write[18] = ((RS485_DATA[2])) & 0xff;//Ñ¹Á¦Öµ¸ß8bit
	SD_write[19] = ((Uint16)(temperature/0.5)) & 0xff;//ÎÂ¶ÈÁ¿»¯ÖµµÍ8bit
	SD_write[20] = (((Uint16)(temperature/0.5))>>8) & 0xff;//ÎÂ¶ÈÁ¿»¯Öµ¸ß8bit
	SD_write[21] = (noise_10ms) & 0x0ff;//ÔëÉùÁ¿»¯ÖµµÍ8bit
	SD_write[22] = (noise_10ms>>8) & 0x0ff;//ÔëÉùÁ¿»¯Öµ¸ß8bit
	SD_write[23] = 0x00;//FSKÆµÂÊ1---9kHz
	SD_write[24] = 0x08;//FSKÆµÊ2---9+8*0.5=13kHz
	SD_write[25] = 0x0ff;//Ğ£éÎ»
	SD_write[26] = SD_status;//SD¿¨×´Ì¬£¬1ÎªÕı³££¬0Îª²»Õı³£
 	SD_write[27] = 0x0ff;//Ğ£ÑéÎ»
}






