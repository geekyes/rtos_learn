#include "stm32f10x.h"
#include "lcd12864.h"
#include "delay.h"
#include "matrixKey.h"
#include "usart.h"
#include "calculate.h"
#include "hx711.h"
#include "usart.h"
#include "timer.h"
#include "24cxx.h"


int32_t ADValA[17]={0};//存储分段参考值

int main()
{	
	fell_PB8();   //报警器函数初始化
	MatrixKeyInit(1);  //矩阵键盘初始化4x4
	delay_init(72);      //系统内部延时函数
	delay_ms(1000);
	LCD_init();        //lcd12864液晶显示屏初始化
	delay_ms(100);
	LED1_SET;
	LCD_Putstring(2, 3, "启动中...");
	uart_init(9600);   // 串口初始化以及波特率
	delay_ms(1000);
	AT24CXX_Init();			//IIC初始化 
	HX711Init();		    // AD (HX711) 通讯 GPIO Init
	while(AT24CXX_Check())//检测不到24c02，用于保存数据
	{
		printf("AT24CXX Error!!!\r\n");
	}
	printf("AT24CXX OK!!!\r\n");
	
	{//读取参考值
		int8_t i;
		
		for (i = 0; i < 17; i++) //装载上一次标定的值
		{
			AT24CXX_Read(i * 4, (uint8_t *)&ADValA[i], 4);
			printf("AT24 [%d] : %d \r\n", i, ADValA[i]);
		}
	}
	
	{//开机校零
		int8_t i;
		int32_t tmp = 0;
		
		tmp = (Filter() + Filter() + Filter()) / 3; //采集三次
		tmp -= ADValA[0];	//生成纠正量
		for (i = 0; i < 17; i++) {
			ADValA[i] += tmp;
		}
		for (i = 0; i < 17; i++) {//查看纠正量是否正确
			printf("AT [%d] %d\r\n", i, ADValA[i]);
		}
	}
	LCD_write_com(0x01);
	LED1_RESET;
	TIM3_Int_Init(10 - 1, 7200 - 1); //分频：100us 周期：1ms
	
	//loop
	while (1) {
		//code 
		weight=TestWeight();//ad采集回来重量，给参数weight
	}

}
