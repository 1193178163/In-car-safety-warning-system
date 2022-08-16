
#include "uart.h"


#define Uart1_Buf_Max 100               //�������ݻ��泤��
u8 xdata  Rec_Buf[Uart1_Buf_Max];  //�������ݻ���
u8 point1 = 0;             //練�ָ��

void Uart_Init()					  			   
{	
	SCON = 0X50;  //UART��ʽ1��8λUART
	REN  = 1;     //���������пڽ�������
	PCON = 0x00;  //SMOD=0;�����ʲ��ӱ�
	TMOD = 0x21;  //T1��ʽ2�����ڲ���������
	TH1  = 0xFD;  //װ��ֵ
	TL1  = 0xFD;
	TR1  = 1;     //������ʱ��1
	EA   = 1;     //��ȫ���жϿ���
	ES   = 1;     //���п��ж�	
}


/*----------------------------
���ʹ�������
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //д���ݵ�UART���ݼĴ���
		while(TI == 0);
		TI = 0;
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}



void CLR_Buf(void)                           // ���ڻ�������
{
	memset(Rec_Buf, 0, Uart1_Buf_Max);      //���

    point1 = 0;                    
}

void RECEIVE_DATA(void) interrupt 4 using 1         //�����ж�	  		
{    
	ES = 0;
	if (RI)
    {
      	RI = 0;                                //���RIλ
		Rec_Buf[point1] = SBUF; 

			
		point1++;               
		if(point1>=Uart1_Buf_Max)          
		{
			point1 = 0;
		}           

    }
	ES =  1;

}










