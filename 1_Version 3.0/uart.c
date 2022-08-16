
#include "uart.h"


#define Uart1_Buf_Max 100               //串口数据缓存长度
u8 xdata  Rec_Buf[Uart1_Buf_Max];  //串口数据缓存
u8 point1 = 0;             //绶存指针

void Uart_Init()					  			   
{	
	SCON = 0X50;  //UART方式1；8位UART
	REN  = 1;     //不允许串行口接收数据
	PCON = 0x00;  //SMOD=0;波特率不加倍
	TMOD = 0x21;  //T1方式2，用于产生波特率
	TH1  = 0xFD;  //装初值
	TL1  = 0xFD;
	TR1  = 1;     //启动定时器1
	EA   = 1;     //打开全局中断控制
	ES   = 1;     //串行口中断	
}


/*----------------------------
发送串口数据
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //写数据到UART数据寄存器
		while(TI == 0);
		TI = 0;
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}



void CLR_Buf(void)                           // 串口缓存清理
{
	memset(Rec_Buf, 0, Uart1_Buf_Max);      //清空

    point1 = 0;                    
}

void RECEIVE_DATA(void) interrupt 4 using 1         //串口中断	  		
{    
	ES = 0;
	if (RI)
    {
      	RI = 0;                                //清除RI位
		Rec_Buf[point1] = SBUF; 

			
		point1++;               
		if(point1>=Uart1_Buf_Max)          
		{
			point1 = 0;
		}           

    }
	ES =  1;

}











