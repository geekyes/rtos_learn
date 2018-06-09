#ifndef __LCD12864_H
#define __LCD12864_H
#include "sys.h"

#define LCD_CS PAout(5)
	
#define LCD_SID PAout(6)
	
#define LCD_SCLK PAout(7)

//串行发送一个8Bit的数据
extern void LCD_sendbyte(u8 byte);

//写入命令到Lcd
extern void LCD_write_com(u8 com);

//写入数据到Lcd
extern void LCD_write_dat(u8 dat);

//Lcd Init
extern void LCD_init(void);

//打印字符串
extern void print(char *s);

//显示字定位
extern void LCD_Setaddress(u8 x,u8 y);

//定位显示字
extern void LCD_Putstring(u8 x, u8 y, char *pData);

#endif
