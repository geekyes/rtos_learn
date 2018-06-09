/*-------------------------------------------------------------------------------
文件名称：myiic.c
文件描述：模拟IIC时序控制，
硬件平台：尼莫M3S开发板
编写整理：shifang
固件库  ：V3.5
技术论坛：www.doflye.net
备    注：通过简单修改可以移植到其他开发板，部分资料来源于网络。
---------------------------------------------------------------------------------*/
#include "myiic.h"
#include "delay.h"
 
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 输出高
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_SET;	  	  
	IIC_SCL_SET;
	Delay_us(4);
 	IIC_SDA_CLR;//START:when CLK is high,DATA change form high to low 
	Delay_us(4);
	IIC_SCL_CLR;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_CLR;
	IIC_SDA_CLR;//STOP:when CLK is high DATA change form low to high
 	Delay_us(4);
	IIC_SCL_SET; 
	IIC_SDA_SET;//发送I2C总线结束信号
	Delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_SET;Delay_us(1);	   
	IIC_SCL_SET;Delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_CLR;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_CLR;
	SDA_OUT();
	IIC_SDA_CLR;
	Delay_us(2);
	IIC_SCL_SET;
	Delay_us(2);
	IIC_SCL_CLR;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_CLR;
	SDA_OUT();
	IIC_SDA_SET;
	Delay_us(2);
	IIC_SCL_SET;
	Delay_us(2);
	IIC_SCL_CLR;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_CLR;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA_SET;
		else
			IIC_SDA_CLR;
		txd<<=1; 	  
		Delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_SET;
		Delay_us(2); 
		IIC_SCL_CLR;	
		Delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_CLR; 
        Delay_us(2);
		IIC_SCL_SET;
        receive<<=1;
        if(READ_SDA)receive++;   
		Delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/

























