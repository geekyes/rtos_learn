#include "calculate.h"
#include "hx711.h"
#include "24cxx.h"	 
#include "myiic.h"
#include "math.h"
#include "usart.h"

float gprs;
u8 u8Test = 0; //������̷���ֵ����
u8 u8vun = 0 ;//ȥƤ��������
u8 hp0,hp1;//��ʾ��������
float num=0,num1=0,num2=0;//�۸��ܼƱ�������
float weight;//ad�ɼ�����ֵ
u8 a=0;//����������������
u8 b=0;//�궨�����
u8 c=0;//ad����־λ
u8 u8vit=50;//adʵ�ʲɼ�����������
float guw,guw1,guw2;  //ȥƤ�����ֵ
float buf[3]={0};//�궨�ؼ�����
int32_t temp = 0;
u8 xs=0,k=0;//��ʾ��������

/**********************************************
***************12864��ʾ����*******************
***********************************************/

 void xianshi(void)
 {  
	
	LCD_write_com(0x80); //��ʾ��һ��   
	LCD_Putstring(1, 1, "DJ:");
	LCD_write_dat(hp0+0x30);
	LCD_write_dat(hp1+0x30);
	LCD_Putstring(1, 4, "��/g");

	LCD_write_com(0x90);//��ʾ�ڶ���
	LCD_Putstring(2, 1, "ZL:     ");
	LCD_write_dat( (u8)(guw/100)+0x30);
	LCD_write_dat( ((u8)(guw/10))%10+0x30);
	LCD_write_dat( ((u8)(((u16)guw )%10) )+0x30);
	LCD_write_dat('.');
	LCD_write_dat( ((u8)(guw*10))%10+0x30);
//	LCD_write_dat( ((u8)(guw*100))%10+0x30);	 
	LCD_Putstring(2, 8, "g");

	LCD_write_com(0x88);//��ʾ������
	if (num < 1000) {
		LCD_Putstring(3, 1, "D.J.:   ");
		LCD_write_dat( ((u8)(num/100))+0x30);
		LCD_write_dat( ((u8)(num/10))%10+0x30); 
		LCD_write_dat( (u8)(((u16)num)%10)+0x30);
		LCD_Putstring(3, 8, "��");
	}

	LCD_write_com(0x98);//��ʾ������
	if (num < 10000) {
		LCD_Putstring(4, 1, "ZJ:     ");
		LCD_write_dat( ((u8)(num2/1000))%10+0x30);
		LCD_write_dat( ((u8)(num2/100))%10+0x30);
		LCD_write_dat( ((u8)(num2/10))%10+0x30); 
		LCD_write_dat( (u8)(((u16)num)%10)+0x30);
		LCD_Putstring(4, 8, "��");
	}
 }
 
 
 /*************************************
 *****������Ӧ�ļ۸�*******************
 *****�Լ������ж�*********************
 **************************************/
#define KEY1 0xe7    //���� 1
#define KEY2 0xeb    //���� 2
#define KEY3 0xed    //���� 3
#define KEY4 0xee    //���� 4
#define KEY5 0xd7    //���� 5
#define KEY6 0xdb    //���� 6
#define KEY7 0xdd    //���� 7
#define KEY8 0xde    //���� 8
#define KEY9 0xb7    //���� 9
#define KEY0 0xbb    //���� 0
#define KEY_A 0xbd		//������������������
#define KEY_B 0xbe		//�ܼ�ȷ��
#define KEY_C 0x77		//ȥƤ
#define KEY_D 0x7b		//���ư���
#define KEY_E 0x7d		//�궨���,ad����²ɼ�ֵ
#define KEY_F 0x7e		//�궨�ؼ���

//#define KEY1 0x7e    //���� 1
//#define KEY2 0x7d    //���� 2
//#define KEY3 0x7b    //���� 3
//#define KEY4 0x77    //���� 4
//#define KEY5 0xde    //���� 5
//#define KEY6 0xdd    //���� 6
//#define KEY7 0xdb    //���� 7
//#define KEY8 0xd7    //���� 8
//#define KEY9 0xbe    //���� 9
//#define KEY0 0xbd    //���� 0
//#define KEY_A 0xbb		//������������������
//#define KEY_B 0xb7		//�ܼ�ȷ��
//#define KEY_C 0xee		//ȥƤ
//#define KEY_D 0xed		//���ư���
//#define KEY_E 0xeb		//�궨���,ad����²ɼ�ֵ
//#define KEY_F 0xe7		//�궨�ؼ���

void  jianpan(void)
{
	u8Test = MatrixKeyScan(0);//������̷���ֵ
	switch (u8Test) {
		case KEY1: u8Test = 1; break;
		case KEY2: u8Test = 2; break;
		case KEY3: u8Test = 3; break;
		case KEY4: u8Test = 4; break;
		case KEY5: u8Test = 5; break;
		case KEY6: u8Test = 6; break;
		case KEY7: u8Test = 7; break;
		case KEY8: u8Test = 8; break;
		case KEY9: u8Test = 9; break;
	}
					
	if(u8Test==KEY0||u8Test==1||u8Test==2||u8Test==3||u8Test==4||u8Test==5||
		u8Test==6||u8Test==7||u8Test==8||u8Test==9) { 
		a++;
		if(a==1)//������һλ�������ж�
		{
			if(u8Test==KEY0)//�ж��Ƿ���0����
				hp0=0;
			else hp0=u8Test;
		}
		if(a==2)//�����ڶ�λ�������ж�
		 {
		 if(u8Test==KEY0)//�ж��Ƿ���0����
			hp1=0;
		 else hp1=u8Test;
		 }
	}

	if(u8Test==KEY_A)//�����۸������
		hp0=hp1=a=num=0;		

	if(u8Test==KEY_B)//�ܼ�ȷ�ϼ�
	{
		num=(hp0*10+hp1*1)*guw;//ȥƤ��ĵ�����Ǯ����
		num1=num;
		num2+=num1;//�ۼƼ۸����
	}
	
	if(u8Test==KEY_C)//ȥƤ����
	{
		u8vun=!u8vun;
		guw1 =weight;//ad�ɼ������������洢������ȥƤ
		hp0=hp1=0;
	}
	
	LED_RESET; //û���κγ���PB.8Ϊ��
	if(guw>550) LED_SET;  //���ر���
	
	if(u8vun==1)
	{
		if(guw1>100) LED_SET;  //ȥƤ����
		guw=weight-guw1; //ȥƤ����
		LCD_Putstring(1, 7, "peel");
	}//ȥƤ��ʾ
	else if(u8vun==0&&xs == 0) {
			guw=weight;//��ȥƤ����
			LCD_Putstring(1, 6, "  ����");//û��ȥƤ��
	}
	
	if(u8Test==KEY_D)//���ư���
		hp0=hp1=a=num=num1=num2=guw=guw1=0;
	
	if(u8Test==KEY_E)//�궨���,ad����²ɼ�ֵ
	{
		int8_t i;
		int32_t tmp = 0;
		fell_set;//��������һ����ʾ
		delay_ms(20);
		fell_reset;
		tmp = (Filter() + Filter() + Filter()) / 3; //�ɼ�����
		tmp -= ADValA[0];	//���ɾ�����
		guw=weight=0;        //���洢�ı������
		for (i = 0; i < 17; i++) {
		ADValA[i] += tmp;
		}
		fell_set;//��������һ����ʾ
		delay_ms(20);
		fell_reset;
		delay_ms(20);
		fell_set;//��������һ����ʾ
		delay_ms(20);
		fell_reset;
	}
					                							
    if(u8Test==KEY_F) //�궨�ؼ���
	{
//		int32_t temp = 0;
        xs=1;							
		b++;
		LED1_SET;
		if (b != 18) temp = (Filter() + Filter() + Filter() + Filter()) / 4;
							
		if(b==1) {
			k=1;
			AT24CXX_Write(0,(uint8_t *)&(temp),4);
		}
		else if(b==2) {
			k=2;
			AT24CXX_Write(4,(uint8_t *)&temp,4);
		}
		else if(b==3) {
			k=3;
			AT24CXX_Write(8,(uint8_t *)&temp,4);
		}
		else if(b==4) {
			k=4;
			AT24CXX_Write(12,(uint8_t *)&temp,4);
		}
		else if(b==5) {
			k=5;
			AT24CXX_Write(16,(uint8_t *)&temp,4);
		}
		else if(b==6) {
			k=6;
			AT24CXX_Write(20,(uint8_t *)&temp,4);
		}
		else if(b==7) {
			k=7;
			AT24CXX_Write(24,(uint8_t *)&temp,4);
		}
		else if(b==8) {
			k=8;
			AT24CXX_Write(28,(uint8_t *)&temp,4);
		}
		else if(b==9) {
			k=9;
			AT24CXX_Write(32,(uint8_t *)&temp,4);
		}
		else if(b==10) {
			k=10;
			AT24CXX_Write(36,(uint8_t *)&temp,4);
		}
		else if(b==11) {
			k=11;
			AT24CXX_Write(40,(uint8_t *)&temp,4);
		}
		else if(b==12) {
			k=12;
			AT24CXX_Write(44,(uint8_t *)&temp,4);
		}
		else if(b==13) {
			k=13;
			AT24CXX_Write(48,(uint8_t *)&temp,4);
		}
		else if(b==14) {
			k=14;
			AT24CXX_Write(52,(uint8_t *)&temp,4);
		}
		else if(b==15) {
			k=15;
			AT24CXX_Write(56,(uint8_t *)&temp,4);
		}
		else if(b==16) {
			k=16;
			AT24CXX_Write(60,(uint8_t *)&temp,4);
		}
		else if(b==17) {
			k=17;
			AT24CXX_Write(64,(uint8_t *)&temp,4);
		}
		else if(b==18){
            u8 i; b = 0; xs=0;
			LED1_RESET;
            for (i = 0; i < 17; i++) {
            AT24CXX_Read(i * 4, (uint8_t *)&ADValA[i], 4);
//            printf("AT24 [%d] : %d \r\n", i, ADValA[i]);
			}
		}				 
//		printf("temp[%d] :%d \r\n", b, temp);
	}
	
	if(xs == 1)
	{
		static u32 h=0; h++;
		if(h == 1) {	
			LCD_write_com(0x01);
		}
		if(h!=1) {biao_ding();} 
		if(h>18)	h=2; 					 
	}
	
	if(xs == 0) { 				
		xianshi();
	}
					
}
					  
void fell_PB8(void)  //������GPIO�ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO�ṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PB.8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.8

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //PB.8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //PB.8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.8
}
/****************************
****�����궨��ʾ*************
****************************/
void biao_ding(void)
{   
	if(k == 1||k == 5||k==9 ||k==13||k==17) {
		LCD_write_com(0x80); //��ʾ��һ��
		if(k == 1) LCD_Putstring(1, 1, "0g");
		if(k == 5) LCD_Putstring(1, 1, "30g");
		if(k == 9) LCD_Putstring(1, 1, "150g");
		if(k == 13) LCD_Putstring(1, 1, "350g");
		if(k == 17) LCD_Putstring(1, 1, "550g");
		LCD_write_com(0x83); 
		LCD_write_dat( (u8) ((u32)temp/1000000)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/100000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/10000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/1000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/100))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/10))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/1))%10)+0x30);
	}
	
	if(k == 2|| k== 6 || k== 10 || k==14) {
		LCD_write_com(0x90);//��ʾ�ڶ���
		if(k == 2) LCD_Putstring(2, 1, "5g");
		if(k == 6) LCD_Putstring(2, 1, "40g");	
		if(k == 10) LCD_Putstring(2, 1, "200g");
		if(k == 14) LCD_Putstring(2, 1, "400g");
		LCD_write_com(0x93); 
		LCD_write_dat( (u8) ((u32)temp/1000000)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/100000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/10000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/1000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/100))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/10))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/1))%10)+0x30);
	}
	
	if(k == 3 || k==7 || k==11 || k== 15) {
		LCD_write_com(0x88);//��ʾ������
		if(k == 3) LCD_Putstring(3, 1, "10g");
		if(k == 7) LCD_Putstring(3, 1, "50g");
		if( k== 11) LCD_Putstring(3, 1, "250g");
		if(k == 15) LCD_Putstring(3, 1, "450");
		LCD_write_com(0x8B); 
		LCD_write_dat( (u8) ((u32)temp/1000000)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/100000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/10000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/1000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/100))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/10))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/1))%10)+0x30);
	}
	
	if(k == 4 || k==8 || k == 12 || k==16) {
		LCD_write_com(0x98);//��ʾ������
		if(k == 4) LCD_Putstring(4, 1, "20g");
		if(k == 8) LCD_Putstring(4, 1, "100g");
		if(k == 12) LCD_Putstring(4, 1, "300g");
		if ( k == 16)LCD_Putstring(4, 1, "500g");
		LCD_write_com(0x9B); 
		LCD_write_dat( (u8) ((u32)temp/1000000)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/100000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/10000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/1000))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/100))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/10))%10)+0x30);
		LCD_write_dat( (u8) (((u32)(temp/1))%10)+0x30);
	}
}

