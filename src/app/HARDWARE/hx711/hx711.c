#include "hx711.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"


#define ADValDO PBin(12)
#define ADValSK PBout(13)

/********
*初始化 HX711通讯的GPIO
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
*读取 HX711 的ADVal转换值
*
* return : ADVal转换值

*  */
uint32_t HX711ReadADVal(void)
{
	uint32_t u32Count = 0;
	uint8_t i;
	
	ADValSK = 0;	//使能 ADVal
	while (ADValDO) ;	//ADVal转换未结束则等待
	//delay_us(1);
	
	for (i = 0; i < 24; i++) {
		ADValSK = 1;	//PD_SCK 置高(发送脉冲)
		u32Count <<= 1;	//下降沿来是变量Count左移一位 右侧补零
		//delay_us(1);
		ADValSK = 0;	//PS_SCK 置低
		if (ADValDO) {
			u32Count++;
			//delay_us(1);
		}
	}
	
	ADValSK = 1;
	/* ^0x800000 原因是采集为负数的时候 ————date:16.7.25 */
	u32Count ^= 0x800000;	//第24个脉冲下降沿来时 转换数据
	//delay_us(1);
	ADValSK = 0;
	//delay_us(1);
	
	return (u32Count);
}


/*
 *选择排序
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


#define ADValN 16 //求平均数的个数
/***********************
*为了让读取的数据相对稳定，求取平均值
* return : 
* 说明：所谓的要提高计算量，最后的返回值有位运算，修改N值就得改return的除数
*        且当 N 过大时，要考虑溢出的问题
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
	SelectSort(Val, ADValN);		//选择排序
	
	return (Val[4] + Val[5] + Val[6] + Val[7] + Val[8] + Val[9] + Val[10] + Val[11]) / 8;
}


/************
* 对物体称重
* u32InitVal : 上电读取的ADVal值
* return : 物体的重量

*/
//初始值
/*8250000,8254200,8259620,8269500,8277000,
  8285200,8293000,8334000,8415000,8497500,8578100,8663644*/


float TestWeight(void)
{
	int32_t ADVal = 0;
	__ASM volatile("cpsid i");//关闭所有中断
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
	__ASM volatile("cpsie i");//开启说有中断
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
