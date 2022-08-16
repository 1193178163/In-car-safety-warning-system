#include <main.h>
#include <LCD.h>
#include "SGP30.h"
#include "SIM800.h"
#include <DS18B20.h>
#include "uart.h"


sbit Key1=P3^4;	 //开关报警器
sbit Key2=P3^5;	 //打开红外检测
sbit Buzzer=P2^4;//蜂鸣器（低电平触发）
sbit HC_SR501=P3^2;//红外 ，检测到活动人体时， P3^2将接收到高
sbit C2H5OH=P1^7; //酒精浓度检测，通过调节电位器控制报警阈值。电位器电阻通过公式计算
sbit pie=P1^6;//树莓派检测驾驶员疲惫时发出高电平


	
void delay_ms_main(u16 ms)               
{
	u16 a,b;
	for(a=ms;a>0;a--)
		for(b=114;b>0;b--);
}
void init()	   //初始化函数
{
	TMOD=0x01; //定时器T0为工作方式1
	TH0=(65536-46080)/256;    //晶振为11.0592M时，定时器0溢出20次恰好为1s
  TL0=(65536-46080)%256;          //定时器0
	ET0=1;	  //打开定时器0中断允许开关      
	TR0=1;	  //启动定时器0
	EA=1;	  //打开中断总开关
	pie=0;
	
}

u8 siren=1,n=0,f=0,t=0;  //siren警报器，值为1时开启报警装置.  n 定时器用于循环函数。f 定时器和timer_function_exits共用于红外检测
u16 CO2Data;//定义CO2浓度变量
float temp;
void timer_function_exits(){
	  char msg[]="Human body detected in the car";		//短信内容	
		
		EA=0;
		Buzzer=1;
		if(f==1){
		f=0;
		init_lcd();
		write_com(0x83);
		lcd_display("Human body");
		write_com(0xC4);
		lcd_display("detected");
		delay_ms_main(1500);
		init_lcd();
		write_com(0xC4);
		lcd_display("sending..");
		EA=1;
			TR0=0;
		Uart_Init();
		sendwarming("17620042017",msg);
			TR0=1;
			
		EA=0;
		init_lcd();
		write_com(0xC4);
		lcd_display("Alert sent");		}
		EA=1;
	
		if(t==1){
			t=0;
		init_lcd();
		/*显示英文Temp：*/
		lcd_pos(0,0);
		lcd_display("Temp:");
		/*显示 CO2：*/
		lcd_pos(1,1);           
		lcd_display("CO2:"); 
		}

}


void display_data()
{
		u8 buf[6]={0}; 
		u32 sgp30_dat;   //400~60000ppm,本程序只检测400~9999ppm
		


		SGP30_Init();   //初始化SGP30


		while(1)
	{
		EA=0; //关闭中断，防止中断干扰数据读取
		/*读取温度数值并显示*/
		temp=ReadTemp();//读取温度
		sprintf(buf,"%.2f",temp);  //温度数值装入数组
		EA=1;
		delay_ms_main(100);  //给定时器中断进入
		timer_function_exits();  //进入检测是否需要发信报警
		EA=0;
		write_com(0x86);
		lcd_display(buf);//显示温度数值
		lcd_pos(0,12);
		write_data(0xdf);       //显示字符“°” ASCILL即为0xdf
		lcd_pos(0,13);      
		write_data(0x43);      // 显示字符“C” ASCILL即为0x43
		EA=1;
		delay_ms_main(100);  //给定时器中断进入
		timer_function_exits();  //进入检测是否需要发信报警
		EA=0 ;
		/*读取CO2数值并显示*/
		SGP30_Write(0x20,0x08);
		(u32)sgp30_dat = (u32)SGP30_Read();				//读取SGP30的值
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;	//取出CO2浓度值
		//TVOCData = sgp30_dat & 0x0000ffff;      //取出TVOC值
		sprintf(buf,"%d",CO2Data);  							//CO2数值装入数组
		EA=1;
				if(temp>=30&&siren==1)	  //温度高于设置报警的温度
		{
			Buzzer=0;								//蜂鸣器打开
			write_com(0x8F);
			lcd_display("<");
		}
				if(CO2Data>=1000&&siren==1)	  //温度高于设置报警的温度
		{
			Buzzer=0;								//蜂鸣器打开
			write_com(0xCF);
			lcd_display("<");
		}
		delay_ms_main(300);  //给定时器中断进入
		timer_function_exits();  //进入检测是否需要发信报警
		EA=0;


		if(400<=CO2Data<1000)   		//选择符号显示的位置
		{write_com(0xC9);	lcd_display(" ppm ");	write_com(0xC6); lcd_display(buf);}
		else if(1000<=CO2Data<10000)
		{write_com(0xCA); lcd_display(" ppm ");	write_com(0xC6); lcd_display(buf);}
		else   								
		{write_com(0xC6); lcd_display("error   ");	}


		EA=1;

		delay_ms_main(200);
		write_com(0x8F);
		lcd_display(" ");
		write_com(0xCF);
		lcd_display(" ");
		Buzzer=1;								//蜂鸣器关闭
		timer_function_exits();  //进入检测是否需要发信报警

	}
	

}


void  time1_int(void) interrupt 1  //定时器函数
{
	TH0=(65536-46080)/256;  				 //晶振为11.0592M时，定时器0溢出20次恰好为1s
  TL0=(65536-46080)%256;           //给定时器0重新赋初值
			
		if(Key1==0){
				delay_ms_main(10);  //防抖动
				if(Key1==0){
					while(Key1==0);  //防长按
					Buzzer=1;        //关闭蜂鸣器
					Buzzer=0;
					delay_ms_main(150);
					Buzzer=1; 
					siren=~siren;    //警报模式转换（开→关，关→开）
				}
			}
	
		if(Key2==0){//红外检测按钮，按下后相当于汽车熄火
				delay_ms_main(10);  //防抖动
				if(Key2==0){
					while(Key2==0);  //防长按
					Buzzer=1;
					Buzzer=0;
					delay_ms_main(150);
					Buzzer=1; 
					t=1;  //t=1则出定时器中断后显示Temp：和CO2
					
					for(n=0;n<25;n++){
						init_lcd();
						write_com(0xC4);
						lcd_interdisplay("Checking..");
						if(HC_SR501==0){f=1; break;}          //检测到人
							delay_ms_main(320);}}	} 

		if(C2H5OH==0){
			delay_ms_main(3000);
			if(C2H5OH==0){
				t=1;
				init_lcd();
				Buzzer=0;
				write_com(0x81);
				lcd_interdisplay("Warning: high");
				write_com(0xC1);
				lcd_interdisplay("alcohol content");
				delay_ms_main(1600);
				Buzzer=1;
			}
		}
		
		if (pie==1){
				t=1;
				pie=0;
				init_lcd();
				Buzzer=0;
				write_com(0x81);
				lcd_interdisplay("Please don't ");
				write_com(0xC1);
				lcd_interdisplay("drive tired");
				delay_ms_main(3000);
				Buzzer=1;
		}
		
		
			
}




void main()
{	
		init();   //初始化
		SGP30_Init();   //初始化SGP30
		init_lcd();
		
		/*显示英文Temp：*/
		lcd_pos(0,0);
		lcd_display("Temp:");
		/*显示 CO2：*/
		lcd_pos(1,1);           
		lcd_display("CO2:"); 
	
		display_data();
		while(1);


}