
#ifndef __SIM800_H__
#define __SIM800_H__

#include <REG52.h>






//函数或者变量声明
extern void delay_ms_SIM(unsigned int n);
extern unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
extern void errorLog();
extern void soft_reset(void);
extern void sendMessage(char *number,char *msg);
extern void sendwarming(char phoneNumber[],char msg[]);



#endif