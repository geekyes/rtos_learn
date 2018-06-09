#include "calculate.h"
#include "hx711.h"
#include "24cxx.h"	 
#include "myiic.h"
#include "math.h"
#include "usart.h"

float gprs;
u8 u8Test = 0; //矩阵键盘返回值定义
u8 u8vun = 0 ;//去皮锁定定义
u8 hp0,hp1;//显示变量定义
float num=0,num1=0,num2=0;//价格总计变量定义
float weight;//ad采集回来值
u8 a=0;//按键计数变量定义
u8 b=0;//标定点计数
u8 c=0;//ad零点标志位
u8 u8vit=50;//ad实际采集回来的重量
float guw,guw1,guw2;  //去皮保存的值
float buf[3]={0};//标定关键点存放
int32_t temp = 0;
u8 xs=0,k=0;//显示锁定变量

/**********************************************
***************12864显示函数*******************
***********************************************/

 void xianshi(void)
 {  
	
	LCD_write_com(0x80); //显示第一行   
	LCD_Putstring(1, 1, "DJ:");
	LCD_write_dat(hp0+0x30);
	LCD_write_dat(hp1+0x30);
	LCD_Putstring(1, 4, "￥/g");

	LCD_write_com(0x90);//显示第二行
	LCD_Putstring(2, 1, "ZL:     ");
	LCD_write_dat( (u8)(guw/100)+0x30);
	LCD_write_dat( ((u8)(guw/10))%10+0x30);
	LCD_write_dat( ((u8)(((u16)guw )%10) )+0x30);
	LCD_write_dat('.');
	LCD_write_dat( ((u8)(guw*10))%10+0x30);
//	LCD_write_dat( ((u8)(guw*100))%10+0x30);	 
	LCD_Putstring(2, 8, "g");

	LCD_write_com(0x88);//显示第三行
	if (num < 1000) {
		LCD_Putstring(3, 1, "D.J.:   ");
		LCD_write_dat( ((u8)(num/100))+0x30);
		LCD_write_dat( ((u8)(num/10))%10+0x30); 
		LCD_write_dat( (u8)(((u16)num)%10)+0x30);
		LCD_Putstring(3, 8, "￥");
	}

	LCD_write_com(0x98);//显示第四行
	if (num < 10000) {
		LCD_Putstring(4, 1, "ZJ:     ");
		LCD_write_dat( ((u8)(num2/1000))%10+0x30);
		LCD_write_dat( ((u8)(num2/100))%10+0x30);
		LCD_write_dat( ((u8)(num2/10))%10+0x30); 
		LCD_write_dat( (u8)(((u16)num)%10)+0x30);
		LCD_Putstring(4, 8, "￥");
	}
 }
 
 
 /*************************************
 *****计算相应的价格*******************
 *****以及按键判断*********************
 **************************************/
#define KEY1 0xe7    //数字 1
#define KEY2 0xeb    //数字 2
#define KEY3 0xed    //数字 3
#define KEY4 0xee    //数字 4
#define KEY5 0xd7    //数字 5
#define KEY6 0xdb    //数字 6
#define KEY7 0xdd    //数字 7
#define KEY8 0xde    //数字 8
#define KEY9 0xb7    //数字 9
#define KEY0 0xbb    //数字 0
#define KEY_A 0xbd		//单个单价输入错误清楚
#define KEY_B 0xbe		//总价确定
#define KEY_C 0x77		//去皮
#define KEY_D 0x7b		//复计按键
#define KEY_E 0x7d		//标定零点,ad会从新采集值
#define KEY_F 0x7e		//标定关键点

//#define KEY1 0x7e    //数字 1
//#define KEY2 0x7d    //数字 2
//#define KEY3 0x7b    //数字 3
//#define KEY4 0x77    //数字 4
//#define KEY5 0xde    //数字 5
//#define KEY6 0xdd    //数字 6
//#define KEY7 0xdb    //数字 7
//#define KEY8 0xd7    //数字 8
//#define KEY9 0xbe    //数字 9
//#define KEY0 0xbd    //数字 0
//#define KEY_A 0xbb		//单个单价输入错误清楚
//#define KEY_B 0xb7		//总价确定
//#define KEY_C 0xee		//去皮
//#define KEY_D 0xed		//复计按键
//#define KEY_E 0xeb		//标定零点,ad会从新采集值
//#define KEY_F 0xe7		//标定关键点

void  jianpan(void)
{
	u8Test = MatrixKeyScan(0);//矩阵键盘返回值
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
		if(a==1)//按键第一位数输入判断
		{
			if(u8Test==KEY0)//判断是否有0输入
				hp0=0;
			else hp0=u8Test;
		}
		if(a==2)//按键第二位数输入判断
		 {
		 if(u8Test==KEY0)//判断是否有0输入
			hp1=0;
		 else hp1=u8Test;
		 }
	}

	if(u8Test==KEY_A)//单个价格清除键
		hp0=hp1=a=num=0;		

	if(u8Test==KEY_B)//总价确认键
	{
		num=(hp0*10+hp1*1)*guw;//去皮后的单个价钱计算
		num1=num;
		num2+=num1;//累计价格计算
	}
	
	if(u8Test==KEY_C)//去皮按键
	{
		u8vun=!u8vun;
		guw1 =weight;//ad采集回来的重量存储，用于去皮
		hp0=hp1=0;
	}
	
	LED_RESET; //没有任何超限PB.8为低
	if(guw>550) LED_SET;  //超重报警
	
	if(u8vun==1)
	{
		if(guw1>100) LED_SET;  //去皮报警
		guw=weight-guw1; //去皮重量
		LCD_Putstring(1, 7, "peel");
	}//去皮显示
	else if(u8vun==0&&xs == 0) {
			guw=weight;//无去皮重量
			LCD_Putstring(1, 6, "  ——");//没有去皮显
	}
	
	if(u8Test==KEY_D)//复计按键
		hp0=hp1=a=num=num1=num2=guw=guw1=0;
	
	if(u8Test==KEY_E)//标定零点,ad会从新采集值
	{
		int8_t i;
		int32_t tmp = 0;
		fell_set;//蜂鸣器响一声提示
		delay_ms(20);
		fell_reset;
		tmp = (Filter() + Filter() + Filter()) / 3; //采集三次
		tmp -= ADValA[0];	//生成纠正量
		guw=weight=0;        //将存储的变量清空
		for (i = 0; i < 17; i++) {
		ADValA[i] += tmp;
		}
		fell_set;//蜂鸣器响一声提示
		delay_ms(20);
		fell_reset;
		delay_ms(20);
		fell_set;//蜂鸣器响一声提示
		delay_ms(20);
		fell_reset;
	}
					                							
    if(u8Test==KEY_F) //标定关键点
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
					  
void fell_PB8(void)  //报警器GPIO口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PB.8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.8

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //PB.8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //PB.8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.8
}
/****************************
****按键标定显示*************
****************************/
void biao_ding(void)
{   
	if(k == 1||k == 5||k==9 ||k==13||k==17) {
		LCD_write_com(0x80); //显示第一行
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
		LCD_write_com(0x90);//显示第二行
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
		LCD_write_com(0x88);//显示第三行
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
		LCD_write_com(0x98);//显示第四行
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

