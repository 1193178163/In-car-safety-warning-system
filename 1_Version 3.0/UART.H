#ifndef __UART_H__
#define __UART_H__

#include <stdlib.h>
#include <main.h>


//�������߱�������
void Uart_Init();
void SendData(unsigned char ch);
void SendString(char *s);
//bit Hand(unsigned char *a);
void CLR_Buf(void);


extern u8 xdata  Rec_Buf[];  //�������ݻ���
extern u8 point1;             //練�ָ��


#endif