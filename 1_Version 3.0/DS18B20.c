#include <DS18B20.h>


void delay_18B20(unsigned int i)//��ʱ1΢��
{
	while(i--);
}

void ds1820rst()		//ds1820��λ
										//DS18B20Ҫ������������500US���ɸ�λ��
{  
	 unsigned char x=0;
	 DQ = 1;          //DQ��λ
	 delay_18B20(4);  //��ʱ
	 DQ = 0;          //DQ����
	 delay_18B20(100); //��ȷ��ʱ����480us
	 DQ = 1;          //����
	 delay_18B20(40);	 
}  
  
u8 ds1820rd()//������
{ 
	 unsigned char i=0;
	 unsigned char dat = 0;
	 for (i=8;i>0;i--)
	 {   DQ = 0; //�������ź�
		  dat>>=1;
		  DQ = 1; //�������ź�
		  if(DQ)
		  dat|=0x80;
		  delay_18B20(10);
	 }
 	return(dat);
}

void ds1820wr(u8 wdata)//д����
{
	  unsigned char i=0;
    for (i=8; i>0; i--)
   { DQ = 0;
     DQ = wdata&0x01;
     delay_18B20(10);
     DQ = 1;
     wdata>>=1;
   }
}
	
 
float Readtemp()//��ȡ�¶�ֵ��ת��
{
	u16 temp=0;//�����¶�ֵ
	float f_temp; //�������¶�ֵ   
  ds1820rst();   
 
   ds1820wr(0xCC);//����ROM
   _nop_();

  ds1820wr(0x7f);//12 bits�¶ȷֱ���x1f, 0x3f, 0x5f ,0x7f�¶ȶ����ֱ��ʷֱ��Ӧ0.5, 0.25, 0.125 ,0.0625 

   ds1820rst(); 
   ds1820wr(0xcc);//*���������к�*/
	 _nop_();
   ds1820wr(0x44);//*�����¶�ת��*/

   ds1820rst();  
   ds1820wr(0xcc);//*���������к�*/ 
   ds1820wr(0xbe);//*��ȡ�¶�*/ 
   

	 	temp = ds1820rd();     //�¶ȵ�8λ
	temp |= ds1820rd()<<8; //�¶ȸ�8λ
	
	//*******************�����¶�����
	if(temp&0xf800)//����  
		temp=~temp+1;//����=����+1=ԭ��ȡ��+1
	
	f_temp =(temp&0xF)*0.0625;//С������
	f_temp =f_temp+((temp&0x07F0)>>4);//��������
	 //////////////

	return(f_temp);
 
}



