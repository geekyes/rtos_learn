/*
*ֱ���޸�ĳ���ѵĳ���
*/
#include "lcd12864.h"
#include "delay.h"

/**************************************/
//���з���һ���ֽ�
/**************************************/
void LCD_sendbyte(u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
	LCD_SCLK = 0; //����ʱ����
	if (byte&0x80) LCD_SID = 1; //�������λ����
	else LCD_SID = 0;
	LCD_SCLK = 1; //�����ط�������
	byte=byte<<1; //����һλ
	}
}

/****************************************/
//дָ��
/****************************************/
void LCD_write_com(u8 com)
{
	LCD_CS = 1;        
	LCD_sendbyte(0xf8); //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0&com);  //ȡ����λ�����ݷ����δ��ͣ�
	//ÿ���ֽڵ����ݱ����������ֽ�,����λ���ڵ�һ���ֽڵĸ���λ
	LCD_sendbyte(0xf0&(com<<4)); //����λ���ڵڶ����ֽڵĸ���λ 
	LCD_CS = 0;
	delay_ms(1);
}
/******************************************/
//д����
/******************************************/
void LCD_write_dat(u8 dat)
{
	LCD_CS = 1;        
	LCD_sendbyte(0xfa); //����5�������ġ�1��������һ������,11111,RW(0),RS(1),0
	LCD_sendbyte(0xf0&dat); //ȡ����λ�����ݷ����δ��ͣ�
	//ÿ���ֽڵ����ݱ����������ֽ�,����λ���ڵ�һ���ֽڵĸ���λ
	LCD_sendbyte(0xf0&(dat<<4)); //����λ���ڵڶ����ֽ�
	LCD_CS = 0;
	delay_ms(1);
}
/********************************************/
//LCD��ʼ��
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
	LCD_write_com(0x30); //ѡ�����ָ�   
	LCD_write_com(0x0c); //����ʾ�����α꣬������
	LCD_write_com(0x01); //�����ʾ��Ļ����DDRAMλַ����������Ϊ00H
	delay_ms(5); //��������ʱ��ϳ�1.6ms ��˼Ӵ���ʱ
	LCD_write_com(0x02); //��DDRAMλַ��λ,�˴�ò���������ظ�
	LCD_write_com(0x06); //�趨������ƣ�������ʾ���ƶ�
}
/*************************************************/
//��ʾ�ַ���
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
//������ʾ��ַ
/***************************************************/
void LCD_Setaddress(u8 x,u8 y)
{
	//��ַ�ӵ�1�е�1�п�ʼ����0��ʼ
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
	
	LCD_write_com(addr); //�ַ���ʾ��ʼ��ַ
}
/*****************************************************/
//���ַ�����ʾ�ڹ̶�λ��
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
////��ʾ���ֵ�Lcd
///***************************************************/
//void PrintNum(u8 u8Num)
//{
//	u8 u8Tmp = 0;
//	
////	u8Tmp =(u8)(u32Num / 1000) + 48;//ȡǧλ
////	LCD_write_dat(u8Tmp);
////	u32Num %= 1000;//�ӵ�ǧλ
//	u8Tmp =(u8)(u8Num / 100) + 48;//ȡ��λ
//	LCD_write_dat(u8Tmp);
//	u8Num %= 100;//�ӵ���λ
//	u8Tmp =(u8)(u8Num / 10) + 48;//ȡʮλ
//	LCD_write_dat(u8Tmp);
//	u8Num %= 10;//�ӵ�ʮλ
//	u8Tmp =(u8)(u8Num + 48);//ȡ��λ
//	LCD_write_dat(u8Tmp);
//	
//}
