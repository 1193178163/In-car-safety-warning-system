#include <DS18B20.h>


void delay_18B20(unsigned int i)//延时1微秒
{
	while(i--);
}

void ds1820rst()		//ds1820复位
										//DS18B20要求数据线拉低500US即可复位。
{  
	 unsigned char x=0;
	 DQ = 1;          //DQ复位
	 delay_18B20(4);  //延时
	 DQ = 0;          //DQ拉低
	 delay_18B20(100); //精确延时大于480us
	 DQ = 1;          //拉高
	 delay_18B20(40);	 
}  
  
u8 ds1820rd()//读数据
{ 
	 unsigned char i=0;
	 unsigned char dat = 0;
	 for (i=8;i>0;i--)
	 {   DQ = 0; //给脉冲信号
		  dat>>=1;
		  DQ = 1; //给脉冲信号
		  if(DQ)
		  dat|=0x80;
		  delay_18B20(10);
	 }
 	return(dat);
}

void ds1820wr(u8 wdata)//写数据
{
	  unsigned char i=0;
    for (i=8; i>0; i--)
   { DQ = 0;
     DQ = wdata&0x01;
     delay_18B20(10);
     DQ = 1;
     wdata>>=1;
   }
}
	
 
float Readtemp()//读取温度值并转换
{
	u16 temp=0;//整型温度值
	float f_temp; //浮点型温度值   
  ds1820rst();   
 
   ds1820wr(0xCC);//跳过ROM
   _nop_();

  ds1820wr(0x7f);//12 bits温度分辨率x1f, 0x3f, 0x5f ,0x7f温度读数分辨率分别对应0.5, 0.25, 0.125 ,0.0625 

   ds1820rst(); 
   ds1820wr(0xcc);//*跳过读序列号*/
	 _nop_();
   ds1820wr(0x44);//*启动温度转换*/

   ds1820rst();  
   ds1820wr(0xcc);//*跳过读序列号*/ 
   ds1820wr(0xbe);//*读取温度*/ 
   

	 	temp = ds1820rd();     //温度低8位
	temp |= ds1820rd()<<8; //温度高8位
	
	//*******************解析温度数据
	if(temp&0xf800)//负数  
		temp=~temp+1;//补码=反码+1=原码取反+1
	
	f_temp =(temp&0xF)*0.0625;//小数部分
	f_temp =f_temp+((temp&0x07F0)>>4);//整数部分
	 //////////////

	return(f_temp);
 
}



