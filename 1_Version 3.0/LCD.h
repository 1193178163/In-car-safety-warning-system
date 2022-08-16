#ifndef _LCD_H_
#define _LCD_H_

#include <main.h>

#define     LCDIO      P0  //显示屏数据传送口
sbit LCD_RS=P2^6;  //数据或命令选择引脚
sbit LCD_RW=P2^5;	//读写选择
sbit LCD_EN=P2^7;	//1602选通引脚

void init_lcd();
void write_com(u8 com);
void write_data(u8 wdata);
void lcd_display(unsigned char * p);
void lcd_interdisplay(unsigned char * p);
void lcd_pos(unsigned char x,unsigned char y);

#endif