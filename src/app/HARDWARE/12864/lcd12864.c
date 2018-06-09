/*
*直接修改某网友的程序
*/
#include "lcd12864.h"
#include "delay.h"

/**************************************/
//串行发送一个字节
/**************************************/
void LCD_sendbyte(u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
	LCD_SCLK = 0; //拉低时钟线
	if (byte&0x80) LCD_SID = 1; //发送最高位数据
	else LCD_SID = 0;
	LCD_SCLK = 1; //上升沿发送数据
	byte=byte<<1; //左移一位
	}
}

/****************************************/
//写指令
/****************************************/
void LCD_write_com(u8 com)
{
	LCD_CS = 1;        
	LCD_sendbyte(0xf8); //送入5个连续的“1“，启动一个周期,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0&com);  //取高四位，数据分两次传送，
	//每个字节的内容被送入两个字节,高四位放在第一个字节的高四位
	LCD_sendbyte(0xf0&(com<<4)); //低四位放在第二个字节的高四位 
	LCD_CS = 0;
	delay_ms(1);
}
/******************************************/
//写数据
/******************************************/
void LCD_write_dat(u8 dat)
{
	LCD_CS = 1;        
	LCD_sendbyte(0xfa); //送入5个连续的“1“，启动一个周期,11111,RW(0),RS(1),0
	LCD_sendbyte(0xf0&dat); //取高四位，数据分两次传送，
	//每个字节的内容被送入两个字节,高四位放在第一个字节的高四位
	LCD_sendbyte(0xf0&(dat<<4)); //低四位放在第二个字节
	LCD_CS = 0;
	delay_ms(1);
}
/********************************************/
//LCD初始化
/********************************************/
void LCD_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	LCD_write_com(0x00);
	LCD_write_com(0x30); //选择基本指令集   
	LCD_write_com(0x0c); //开显示，无游标，不反白
	LCD_write_com(0x01); //清除显示屏幕，把DDRAM位址计数器调整为00H
	delay_ms(5); //清屏操作时间较长1.6ms 因此加此延时
	LCD_write_com(0x02); //清DDRAM位址归位,此处貌似与清屏重复
	LCD_write_com(0x06); //设定光标右移，整体显示不移动
}
/*************************************************/
//显示字符串
/*************************************************/
void print(char *s)
{ 
	while(*s != '\0')
	{  
		LCD_write_dat(*s);
		s++;
	}
}
/***************************************************/
//设置显示地址
/***************************************************/
void LCD_Setaddress(u8 x,u8 y)
{
	//地址从第1行第1列开始不从0开始
	u8 addr;
	switch(x)
	{
	case 1: addr=0x80+y-1;
		   break;
	case 2: addr=0x90+y-1;
		   break;
	case 3: addr=0x88+y-1;
		   break;
	case 4: addr=0x98+y-1;
		   break;
	default : break;
	}
	
	LCD_write_com(addr); //字符显示开始地址
}
/*****************************************************/
//让字符串显示在固定位置
/*****************************************************/
void LCD_Putstring(u8 x, u8 y, char *pData)
{
	LCD_Setaddress(x,y);
	
	while(*pData != '\0') {
		LCD_write_dat(*pData++);
	}
}

/*
 *
 *
 * */
//void ShowImage(uint8_t x, uint8_t y, char *p)
//{
//	uint8_t i, j;
//	
//	LCD_write_com(0x34);
//	
//	for (i = 0; i < )
//}



///***************************************************/
////显示数字到Lcd
///***************************************************/
//void PrintNum(u8 u8Num)
//{
//	u8 u8Tmp = 0;
//	
////	u8Tmp =(u8)(u32Num / 1000) + 48;//取千位
////	LCD_write_dat(u8Tmp);
////	u32Num %= 1000;//扔掉千位
//	u8Tmp =(u8)(u8Num / 100) + 48;//取百位
//	LCD_write_dat(u8Tmp);
//	u8Num %= 100;//扔掉百位
//	u8Tmp =(u8)(u8Num / 10) + 48;//取十位
//	LCD_write_dat(u8Tmp);
//	u8Num %= 10;//扔掉十位
//	u8Tmp =(u8)(u8Num + 48);//取个位
//	LCD_write_dat(u8Tmp);
//	
//}
