#include <LCD.h>

void delay_ms_LCD(u16 ms)               
{
	u16 a,b;
	for(a=ms;a>0;a--)
		for(b=114;b>0;b--);
}
void write_com(u8 com)	//1602д�����ӳ���
{
	LCD_RS=0;		//RS����������ѡ��̣��ߵ�ƽд���ݣ��͵�ƽд����
	LCD_RW=0;		//RD�Ƕ�дѡ��̣��ߵ�ƽ�����͵�ƽд
	LCD_EN=0;	//1602ѡͨ�ˣ��ߵ�ƽѡͨ���͵�ƽ��ֹ
	LCDIO=com;
	delay_ms_LCD(5);
	LCD_EN=1;
	delay_ms_LCD(5);
	LCD_EN=0;	
}



void write_data(u8 wdata)	//1602д�����ӳ���
{
	LCD_RS=1;	//RS����������ѡ��̣��ߵ�ƽд���ݣ��͵�ƽд����
	LCD_RW=0;	//RD�Ƕ�дѡ��̣��ߵ�ƽ�����͵�ƽд
	LCD_EN=0;	//1602ѡͨ�ˣ��ߵ�ƽѡͨ���͵�ƽ��ֹ
	LCDIO=wdata;
	delay_ms_LCD(5);
	LCD_EN=1;
	delay_ms_LCD(5);
	LCD_EN=0;	
}



void init_lcd()				//1602��ʼ������
{
	
	LCD_EN=0;

	write_com(0x38);	//0011 1000B������ģʽ���ã�����Ϊ8Ϊ���ݿڣ�������ʾ��5*7����
	write_com(0x0c);	//0000 1011B����ʾ����������ã�����ʾ����ʾ��꣬�����˸
	write_com(0x06);	//0000 0110B����ʾ����ƶ����ã�����дһ���ַ�����ַָ���һ�ҹ���һ��дһ���ַ���Ļ��ʾ���ƶ�
	write_com(0x01);	//0000 0001B����ʾ����������ָ���������ʾ����
	write_com(0x80);	//1000 000B���ر���ʾ
	delay_ms_LCD(5);
	
}



void lcd_pos(unsigned char x,unsigned char y)//������ʾλ�� x0~1  y0~
{
	unsigned char pos;
	if(x==0)
		x=0x80;
	else if(x==1)
		x=0xC0;

	pos=x+y;
	write_com(pos);
}

void lcd_display(unsigned char * p)
{
	int i;
	i=0;
	while(p[i]!='\0')//ѭ����ʾ
	{
		write_data(p[i]);
		i++;
	}
}


void lcd_interdisplay(unsigned char * p)
{
	int i;
	i=0;
	while(p[i]!='\0')//ѭ����ʾ
	{
		write_data(p[i]);
		i++;
	}
}






