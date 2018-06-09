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


int32_t ADValA[17]={0};//�洢�ֶβο�ֵ

int main()
{	
	fell_PB8();   //������������ʼ��
	MatrixKeyInit(1);  //������̳�ʼ��4x4
	delay_init(72);      //ϵͳ�ڲ���ʱ����
	delay_ms(1000);
	LCD_init();        //lcd12864Һ����ʾ����ʼ��
	delay_ms(100);
	LED1_SET;
	LCD_Putstring(2, 3, "������...");
	uart_init(9600);   // ���ڳ�ʼ���Լ�������
	delay_ms(1000);
	AT24CXX_Init();			//IIC��ʼ�� 
	HX711Init();		    // AD (HX711) ͨѶ GPIO Init
	while(AT24CXX_Check())//��ⲻ��24c02�����ڱ�������
	{
		printf("AT24CXX Error!!!\r\n");
	}
	printf("AT24CXX OK!!!\r\n");
	
	{//��ȡ�ο�ֵ
		int8_t i;
		
		for (i = 0; i < 17; i++) //װ����һ�α궨��ֵ
		{
			AT24CXX_Read(i * 4, (uint8_t *)&ADValA[i], 4);
			printf("AT24 [%d] : %d \r\n", i, ADValA[i]);
		}
	}
	
	{//����У��
		int8_t i;
		int32_t tmp = 0;
		
		tmp = (Filter() + Filter() + Filter()) / 3; //�ɼ�����
		tmp -= ADValA[0];	//���ɾ�����
		for (i = 0; i < 17; i++) {
			ADValA[i] += tmp;
		}
		for (i = 0; i < 17; i++) {//�鿴�������Ƿ���ȷ
			printf("AT [%d] %d\r\n", i, ADValA[i]);
		}
	}
	LCD_write_com(0x01);
	LED1_RESET;
	TIM3_Int_Init(10 - 1, 7200 - 1); //��Ƶ��100us ���ڣ�1ms
	
	//loop
	while (1) {
		//code 
		weight=TestWeight();//ad�ɼ�����������������weight
	}

}
