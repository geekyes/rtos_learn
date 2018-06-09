#include "hx711.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"


#define ADValDO PBin(12)
#define ADValSK PBout(13)

/********
*��ʼ�� HX711ͨѶ��GPIO
* return : void
* */
void HX711Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//ADValDO GPIO IN FLOATING
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//ADValDO GPIO OUT PP
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}



/***********
*��ȡ HX711 ��ADValת��ֵ
*
* return : ADValת��ֵ

*  */
uint32_t HX711ReadADVal(void)
{
	uint32_t u32Count = 0;
	uint8_t i;
	
	ADValSK = 0;	//ʹ�� ADVal
	while (ADValDO) ;	//ADValת��δ������ȴ�
	//delay_us(1);
	
	for (i = 0; i < 24; i++) {
		ADValSK = 1;	//PD_SCK �ø�(��������)
		u32Count <<= 1;	//�½������Ǳ���Count����һλ �Ҳಹ��
		//delay_us(1);
		ADValSK = 0;	//PS_SCK �õ�
		if (ADValDO) {
			u32Count++;
			//delay_us(1);
		}
	}
	
	ADValSK = 1;
	/* ^0x800000 ԭ���ǲɼ�Ϊ������ʱ�� ��������date:16.7.25 */
	u32Count ^= 0x800000;	//��24�������½�����ʱ ת������
	//delay_us(1);
	ADValSK = 0;
	//delay_us(1);
	
	return (u32Count);
}


/*
 *ѡ������
 * */
static void SelectSort(uint32_t arr[], int16_t Num)
{
    int8_t i, j, k = 0;
	int Next = 0, This = 0;

    for (i = Num - 1; i >= 0; i--) {
        k = i;
        for (j = i - 1; j >= 0; j--) {
            Next = arr[j];
            This = arr[k];
            if (This < Next) {
                k = j;
            }
        }
        if (i != k) {
            arr[i] ^= arr[k];
            arr[k] ^= arr[i];
			arr[i] ^= arr[k];
        }
    }
}


#define ADValN 16 //��ƽ�����ĸ���
/***********************
*Ϊ���ö�ȡ����������ȶ�����ȡƽ��ֵ
* return : 
* ˵������ν��Ҫ��߼����������ķ���ֵ��λ���㣬�޸�Nֵ�͵ø�return�ĳ���
*        �ҵ� N ����ʱ��Ҫ�������������
*/
int32_t Filter(void)
{
	int8_t i;
	uint32_t Val[ADValN] = {0};
	
	//
	for (i = ADValN - 1; i >= 0; i--) {
		Val[i] = HX711ReadADVal();
		delay_ms(50);
	}
	SelectSort(Val, ADValN);		//ѡ������
	
	return (Val[4] + Val[5] + Val[6] + Val[7] + Val[8] + Val[9] + Val[10] + Val[11]) / 8;
}


/************
* ���������
* u32InitVal : �ϵ��ȡ��ADValֵ
* return : ���������

*/
//��ʼֵ
/*8250000,8254200,8259620,8269500,8277000,
  8285200,8293000,8334000,8415000,8497500,8578100,8663644*/


float TestWeight(void)
{
	int32_t ADVal = 0;
	__ASM volatile("cpsid i");//�ر������ж�
	int32_t ZERO = ADValA[0];
	int32_t ADVal5 = ADValA[1];
	int32_t ADVal10 = ADValA[2];
	int32_t ADVal20 = ADValA[3];
	int32_t ADVal30 = ADValA[4];
	int32_t ADVal40 = ADValA[5];
	int32_t ADVal50 = ADValA[6];

	int32_t ADVal100 = ADValA[7];
	int32_t ADVal150 = ADValA[8];
	int32_t ADVal200 = ADValA[9];
	int32_t ADVal250 = ADValA[10];
	int32_t ADVal300 = ADValA[11];
	int32_t ADVal350 = ADValA[12];
	int32_t ADVal400 = ADValA[13];
	int32_t ADVal450 = ADValA[14];
	int32_t ADVal500 = ADValA[15];
	int32_t ADVal550 = ADValA[16];
	__ASM volatile("cpsie i");//����˵���ж�
	float mass = 0;
	
	ADVal = Filter();
	//printf("ADVal: %d\r\n", ADVal);
								
	if(ADVal<ZERO)	ADVal=ZERO;
	
	if((ADVal>=ZERO)&&(ADVal<=ADVal5))			mass=5.0*(ADVal-ZERO)/(ADVal5-ZERO);
	if((ADVal>ADVal5)&&(ADVal<=ADVal10))		mass=5.0*(ADVal-ADVal5)/(ADVal10-ADVal5)+5;
	if((ADVal>ADVal10)&&(ADVal<=ADVal20))		mass=10.0*(ADVal-ADVal10)/(ADVal20-ADVal10)+10;
	if((ADVal>ADVal20)&&(ADVal<=ADVal30))		mass=10.0*(ADVal-ADVal20)/(ADVal30-ADVal20)+20;
	if((ADVal>ADVal30)&&(ADVal<=ADVal40))		mass=10.0*(ADVal-ADVal30)/(ADVal40-ADVal30)+29.5;
	if((ADVal>ADVal40)&&(ADVal<=ADVal50))		mass=10.0*(ADVal-ADVal40)/(ADVal50-ADVal40)+40;
	
	if((ADVal>ADVal50)&&(ADVal<=ADVal100))		mass=50.0*(ADVal-ADVal50)/(ADVal100-ADVal50)+50;
	if((ADVal>ADVal100)&&(ADVal<=ADVal150))		mass=50.0*(ADVal-ADVal100)/(ADVal150-ADVal100)+100;
	if((ADVal>ADVal150)&&(ADVal<=ADVal200))		mass=50.0*(ADVal-ADVal150)/(ADVal200-ADVal150)+150;
	if((ADVal>ADVal200)&&(ADVal<=ADVal250))		mass=50.0*(ADVal-ADVal200)/(ADVal250-ADVal200)+200;
	if((ADVal>ADVal250)&&(ADVal<=ADVal300))		mass=50.0*(ADVal-ADVal250)/(ADVal300-ADVal250)+250;
	if((ADVal>ADVal300)&&(ADVal<=ADVal350))		mass=50.0*(ADVal-ADVal300)/(ADVal350-ADVal300)+300;
	if((ADVal>ADVal350)&&(ADVal<=ADVal400))		mass=50.0*(ADVal-ADVal350)/(ADVal400-ADVal350)+350;
	if((ADVal>ADVal400)&&(ADVal<=ADVal450))		mass=50.0*(ADVal-ADVal400)/(ADVal450-ADVal400)+400;
	if((ADVal>ADVal450)&&(ADVal<=ADVal500))		mass=50.0*(ADVal-ADVal450)/(ADVal500-ADVal450)+450;
	if((ADVal>ADVal500))						mass=50.0*(ADVal-ADVal500)/(ADVal550-ADVal500)+500;


	if (mass > 1000) return 999.0;
	else if (mass < 0) return 0;
	else return mass;
}
