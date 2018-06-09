#ifndef __LCD12864_H
#define __LCD12864_H
#include "sys.h"

#define LCD_CS PAout(5)
	
#define LCD_SID PAout(6)
	
#define LCD_SCLK PAout(7)

//���з���һ��8Bit������
extern void LCD_sendbyte(u8 byte);

//д�����Lcd
extern void LCD_write_com(u8 com);

//д�����ݵ�Lcd
extern void LCD_write_dat(u8 dat);

//Lcd Init
extern void LCD_init(void);

//��ӡ�ַ���
extern void print(char *s);

//��ʾ�ֶ�λ
extern void LCD_Setaddress(u8 x,u8 y);

//��λ��ʾ��
extern void LCD_Putstring(u8 x, u8 y, char *pData);

#endif
