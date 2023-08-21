//----------SYSTEM----------//
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
//----------GPIO------------//
#include "gpio.h"
//----------WHEELS----------//
#include "moto.h"
#include "pwm.h"
//----------MPU6050---------//
#include "mpu_timer.h"
#include "mpu6050.h"
//----------OLED------------//
#include "oled.h"

//------------接线说明---------------
//TB6612丝印标识--------STM32F1主板引脚

//    PWMA    -----------    B6
//    AIN1    -----------    B8
//    AIN2    -----------    B7
//    STBY    -----------    5V
//    VM      -----------    5-12V（外接电源）
//    GND     -----------    GND  （外接电源）
//    VCC     -----------    5V   （逻辑电源）
//    GND     -----------    GND   （逻辑共地）
// 

//------------接线说明---------------

//----------函数声明---------//
void BLUE_Config(void);
void OLED_Display(void);
void Arr_Clear(char *arr,int len);
void Get_Message(void);
void Message_Handler(void);

//----------全局变量声明-----//
//#define MAX_MESSAGE_LENG 20
float Pitch,Roll,Yaw;		//陀螺仪角度P:俯仰角 R:滚转角 Y:偏航角
u8 str[20];					//字符串缓存
char arr[20];					//消息数组缓存

int main(void)
{	
	//变量声明 
	int i;
	u16 t;
	u16 len;

	
	SystemInit(); 				//配置系统时钟为72M   
	delay_init();   			//延时函数初始化

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	USART1_Init(115200);
	

	//-----GPIO配置-----//
	LED_GPIO_Init();
	Wheels_GPIO_Init();    		//初始化gpio口B pin_7/pin_8

	//-----WHEELS-------//
	Wheels_PWM_Init(7199,0);   	//初始化pwm输出 72000 000 /7199+1=10000 
	//BLUE_Config();
	
	//-----OLED-----//
	OLED_Init();				//=====OLED初始化	
	OLED_ShowString(0,0,"SYSTEM Start Up",16);
	OLED_Refresh_Gram();//更新显示到OLED	 
	delay_ms(1000);
	OLED_Clear();
	
	TIM_SetCompare1(TIM4,4000);   	//设置TIM4通道1的占空比  3000/7200
	TIM_SetCompare2(TIM4,4000);   	//设置TIM4通道2的占空比  3000/7200
	TIM_SetCompare3(TIM4,4000);   	//设置TIM4通道3的占空比  3000/7200
	TIM_SetCompare4(TIM4,4000);   	//设置TIM4通道4的占空比  3000/7200
	
	
	//-----MPU6050-----//
	IIC_Init();
	MPU6050_initialize();    	//=====MPU6050初始化	
	DMP_Init();
	//TIM2_Getsample_Int(1999,719);		//50ms任务定时中断
	
	
	printf("OK\r\n");
	
	while(1)
	{
		delay_ms(10);						//基础延时
		
		Read_DMP(&Pitch,&Roll,&Yaw);		//更新MPU陀螺仪数据
		
		if((i++)%10==0)							//一定时间更新OLED显示
		{	
			OLED_Display();
			if(i>=100)
			{	
				i=0;
				LED0 =~LED0;
			}
		}
		
		/*
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n\r\n");
			
			Arr_Clear(arr,20);
			
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				arr[t] = USART_RX_BUF[t];
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}
		*/
		
		Get_Message();
		Message_Handler();
	}
}

//AT指令配置模块，蓝牙4.0模块不能跟手机进行连接
void BLUE_Config(void)
{
	//发送AT指令
	printf("AT\r\n");
	delay_ms(500);
	
	//发送更改模块名字指令（需要复位生效，如果发送AT+RESET搜索蓝牙名字没有变更，那么重新对蓝牙模块上电）
	printf("AT+NAMENano\r\n");
	delay_ms(500);

	//printf("AT+UART=9600,0,0\r\n");
	//delay_ms(500);
	
	//发送复位模块的指令
	printf	("AT+RESET\r\n");
	delay_ms(2000);
}

//OLED显示更新
void OLED_Display(void)
{
	u8 stri[20];
		//OLED_Clear();							//清屏
		sprintf((char *)stri,"Pitch:%.2f",Pitch);//显示陀螺仪P:俯仰角
		OLED_ShowString(16,16,stri,16);
		sprintf((char *)stri,"Roll :%.2f",Roll);	//显示陀螺仪R:滚转角
		OLED_ShowString(16,32,stri,16);
		sprintf((char *)stri,"Yaw  :%.2f",Yaw);	//显示陀螺仪Y:偏航角
		OLED_ShowString(16,48,stri,16);
		OLED_Refresh_Gram();//更新显示到OLED	 
}

//数组清空函数
//参数：数组arr 清空长度
void Arr_Clear(char *arr,int len)
{
	int i;
	for(i = 0; i<len; i++)
	{
		*arr = '\0';
		arr++;
	}
}



//接收串口1消息并将消息缓存到message_arr消息缓存数组中
void Get_Message(void)
{
	u16 len;
	u16 t;
	if(USART_RX_STA&0x8000)
	{					   
		len=USART_RX_STA&0x3fff;									//得到此次接收到的数据长度
		printf("\r\nSTM32 get data:\t");
			
		Arr_Clear(arr,20);									//清除消息缓存重新接收
			
		for(t=0;t<len;t++)
		{
			USART_SendData(USART1, USART_RX_BUF[t]);				//向串口1发送数据
			arr[t] = USART_RX_BUF[t];						//将获得的信息缓存到message_arr消息数组
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);	//等待发送结束
		}
		printf("\r\n");//插入换行
		USART_RX_STA=0;
	}
	//else
		//printf("No Message");
}

void Message_Handler(void)
{
	if(arr[0] == '0' || arr[0] == '\0')				//消息缓存头字符为0不处理数据
	{
		Arr_Clear(arr,20);
		return;
	}
	if(arr[0] == 'A')
	{
		LED0 = 1;
		delay_ms(100);
		LED0 = 0;
		delay_ms(100);
		LED0 = 1;
		delay_ms(100);
		LED0 = 0;
		delay_ms(100);
		LED0 = 1;
		delay_ms(100);
	}
		
	//消息处理逻辑
	printf("working for: ");
	strcpy(str,arr);
	printf(str);
	printf("\r\n");//插入换行
	printf("%c",arr[0]);//插入换行
	printf("\r\n");//插入换行
	
	
}

