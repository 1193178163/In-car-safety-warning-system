#include <main.h>
#include <LCD.h>
#include "SGP30.h"
#include "SIM800.h"
#include <DS18B20.h>
#include "uart.h"


sbit Key1=P3^4;	 //���ر�����
sbit Key2=P3^5;	 //�򿪺�����
sbit Buzzer=P2^4;//���������͵�ƽ������
sbit HC_SR501=P3^2;//���� ����⵽�����ʱ�� P3^2�����յ���
sbit C2H5OH=P1^7; //�ƾ�Ũ�ȼ�⣬ͨ�����ڵ�λ�����Ʊ�����ֵ����λ������ͨ����ʽ����
sbit pie=P1^6;//��ݮ�ɼ���ʻԱƣ��ʱ�����ߵ�ƽ


	
void delay_ms_main(u16 ms)               
{
	u16 a,b;
	for(a=ms;a>0;a--)
		for(b=114;b>0;b--);
}
void init()	   //��ʼ������
{
	TMOD=0x01; //��ʱ��T0Ϊ������ʽ1
	TH0=(65536-46080)/256;    //����Ϊ11.0592Mʱ����ʱ��0���20��ǡ��Ϊ1s
  TL0=(65536-46080)%256;          //��ʱ��0
	ET0=1;	  //�򿪶�ʱ��0�ж�������      
	TR0=1;	  //������ʱ��0
	EA=1;	  //���ж��ܿ���
	pie=0;
	
}

u8 siren=1,n=0,f=0,t=0;  //siren��������ֵΪ1ʱ��������װ��.  n ��ʱ������ѭ��������f ��ʱ����timer_function_exits�����ں�����
u16 CO2Data;//����CO2Ũ�ȱ���
float temp;
void timer_function_exits(){
	  char msg[]="Human body detected in the car";		//��������	
		
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
		/*��ʾӢ��Temp��*/
		lcd_pos(0,0);
		lcd_display("Temp:");
		/*��ʾ CO2��*/
		lcd_pos(1,1);           
		lcd_display("CO2:"); 
		}

}


void display_data()
{
		u8 buf[6]={0}; 
		u32 sgp30_dat;   //400~60000ppm,������ֻ���400~9999ppm
		


		SGP30_Init();   //��ʼ��SGP30


		while(1)
	{
		EA=0; //�ر��жϣ���ֹ�жϸ������ݶ�ȡ
		/*��ȡ�¶���ֵ����ʾ*/
		temp=ReadTemp();//��ȡ�¶�
		sprintf(buf,"%.2f",temp);  //�¶���ֵװ������
		EA=1;
		delay_ms_main(100);  //����ʱ���жϽ���
		timer_function_exits();  //�������Ƿ���Ҫ���ű���
		EA=0;
		write_com(0x86);
		lcd_display(buf);//��ʾ�¶���ֵ
		lcd_pos(0,12);
		write_data(0xdf);       //��ʾ�ַ����㡱 ASCILL��Ϊ0xdf
		lcd_pos(0,13);      
		write_data(0x43);      // ��ʾ�ַ���C�� ASCILL��Ϊ0x43
		EA=1;
		delay_ms_main(100);  //����ʱ���жϽ���
		timer_function_exits();  //�������Ƿ���Ҫ���ű���
		EA=0 ;
		/*��ȡCO2��ֵ����ʾ*/
		SGP30_Write(0x20,0x08);
		(u32)sgp30_dat = (u32)SGP30_Read();				//��ȡSGP30��ֵ
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;	//ȡ��CO2Ũ��ֵ
		//TVOCData = sgp30_dat & 0x0000ffff;      //ȡ��TVOCֵ
		sprintf(buf,"%d",CO2Data);  							//CO2��ֵװ������
		EA=1;
				if(temp>=30&&siren==1)	  //�¶ȸ������ñ������¶�
		{
			Buzzer=0;								//��������
			write_com(0x8F);
			lcd_display("<");
		}
				if(CO2Data>=1000&&siren==1)	  //�¶ȸ������ñ������¶�
		{
			Buzzer=0;								//��������
			write_com(0xCF);
			lcd_display("<");
		}
		delay_ms_main(300);  //����ʱ���жϽ���
		timer_function_exits();  //�������Ƿ���Ҫ���ű���
		EA=0;


		if(400<=CO2Data<1000)   		//ѡ�������ʾ��λ��
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
		Buzzer=1;								//�������ر�
		timer_function_exits();  //�������Ƿ���Ҫ���ű���

	}
	

}


void  time1_int(void) interrupt 1  //��ʱ������
{
	TH0=(65536-46080)/256;  				 //����Ϊ11.0592Mʱ����ʱ��0���20��ǡ��Ϊ1s
  TL0=(65536-46080)%256;           //����ʱ��0���¸���ֵ
			
		if(Key1==0){
				delay_ms_main(10);  //������
				if(Key1==0){
					while(Key1==0);  //������
					Buzzer=1;        //�رշ�����
					Buzzer=0;
					delay_ms_main(150);
					Buzzer=1; 
					siren=~siren;    //����ģʽת���������أ��ء�����
				}
			}
	
		if(Key2==0){//�����ⰴť�����º��൱������Ϩ��
				delay_ms_main(10);  //������
				if(Key2==0){
					while(Key2==0);  //������
					Buzzer=1;
					Buzzer=0;
					delay_ms_main(150);
					Buzzer=1; 
					t=1;  //t=1�����ʱ���жϺ���ʾTemp����CO2
					
					for(n=0;n<25;n++){
						init_lcd();
						write_com(0xC4);
						lcd_interdisplay("Checking..");
						if(HC_SR501==0){f=1; break;}          //��⵽��
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
		init();   //��ʼ��
		SGP30_Init();   //��ʼ��SGP30
		init_lcd();
		
		/*��ʾӢ��Temp��*/
		lcd_pos(0,0);
		lcd_display("Temp:");
		/*��ʾ CO2��*/
		lcd_pos(1,1);           
		lcd_display("CO2:"); 
	
		display_data();
		while(1);


}