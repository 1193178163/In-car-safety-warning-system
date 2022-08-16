#include <LCD.h>

void delay_ms_LCD(u16 ms)               
{
	u16 a,b;
	for(a=ms;a>0;a--)
		for(b=114;b>0;b--);
}
void write_com(u8 com)	//1602写命令子程序
{
	LCD_RS=0;		//RS是数据命令选择短，高电平写数据，低电平写命令
	LCD_RW=0;		//RD是读写选择短，高电平读，低电平写
	LCD_EN=0;	//1602选通端，高电平选通，低电平禁止
	LCDIO=com;
	delay_ms_LCD(5);
	LCD_EN=1;
	delay_ms_LCD(5);
	LCD_EN=0;	
}



void write_data(u8 wdata)	//1602写数据子程序
{
	LCD_RS=1;	//RS是数据命令选择短，高电平写数据，低电平写命令
	LCD_RW=0;	//RD是读写选择短，高电平读，低电平写
	LCD_EN=0;	//1602选通端，高电平选通，低电平禁止
	LCDIO=wdata;
	delay_ms_LCD(5);
	LCD_EN=1;
	delay_ms_LCD(5);
	LCD_EN=0;	
}



void init_lcd()				//1602初始化程序
{
	
	LCD_EN=0;

	write_com(0x38);	//0011 1000B，功能模式设置，设置为8为数据口，两行显示，5*7点阵
	write_com(0x0c);	//0000 1011B，显示开及光标设置，关显示，显示光标，光标闪烁
	write_com(0x06);	//0000 0110B，显示光标移动设置，读或写一个字符，地址指针减一且光标减一，写一个字符屏幕显示不移动
	write_com(0x01);	//0000 0001B，显示清屏，数据指针和所有显示清屏
	write_com(0x80);	//1000 000B，关闭显示
	delay_ms_LCD(5);
	
}



void lcd_pos(unsigned char x,unsigned char y)//设置显示位置 x0~1  y0~
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
	while(p[i]!='\0')//循环显示
	{
		write_data(p[i]);
		i++;
	}
}


void lcd_interdisplay(unsigned char * p)
{
	int i;
	i=0;
	while(p[i]!='\0')//循环显示
	{
		write_data(p[i]);
		i++;
	}
}






