#include "SIM800.h"
#include "uart.h"

//常量
#define Success 1U
#define Failure 0U

//定义变量
unsigned long  Time_Cont = 0;       //定时器计数器




void sendwarming(char phoneNumber[],char msg[])
{			  
//	char phoneNumber[] = "13428091613";		//替换成需要被拨打电话的号码
//	char msg[] = "";		//短信内容	
		
	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog();
	delay_ms_SIM(10);

	if (sendCommand("AT+CPIN?\r\n", "READY", 1000, 10) == Success);
	else errorLog();
	delay_ms_SIM(10);


	if (sendCommand("AT+CMGF=1\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog();
	delay_ms_SIM(10);

	if (sendCommand("AT+CSMP=17,167,0,8\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog();
	delay_ms_SIM(10);

	if (sendCommand("AT+CSCS=\"GSM\"\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog();
	delay_ms_SIM(10);

	sendMessage(phoneNumber,msg);		//发送短信

}



void sendMessage(char *number,char *msg)
{
	xdata char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //清空
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog();

	if (sendCommand(msg, msg, 3000, 10) == Success);
	else errorLog();

	memset(send_buf, 0, 40);    //清空
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else errorLog();
}


void errorLog()
{
	while (1)
	{
	  	if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
		{
			soft_reset();
		}
		delay_ms_SIM(200);
	}
}

void soft_reset(void)	 //制造重启命令
{
   ((void (code *) (void)) 0x0000) ();
}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		SendString(Command); 		//发送GPRS指令

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms_SIM(100);
			Time_Cont += 100;
			if (strstr(Rec_Buf, Response) != NULL)
			{
				
				CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	
	CLR_Buf();
	return Failure;
}

//****************************************************
//MS延时函数
//****************************************************
void delay_ms_SIM(unsigned int n)
{
	unsigned int  i,j;
		for(i=0;i<n;i++)
			for(j=0;j<114;j++);
}