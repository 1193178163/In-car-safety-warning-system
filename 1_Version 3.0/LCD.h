#ifndef _LCD_H_
#define _LCD_H_

#include <main.h>

#define     LCDIO      P0  //��ʾ�����ݴ��Ϳ�
sbit LCD_RS=P2^6;  //���ݻ�����ѡ������
sbit LCD_RW=P2^5;	//��дѡ��
sbit LCD_EN=P2^7;	//1602ѡͨ����

void init_lcd();
void write_com(u8 com);
void write_data(u8 wdata);
void lcd_display(unsigned char * p);
void lcd_interdisplay(unsigned char * p);
void lcd_pos(unsigned char x,unsigned char y);

#endif