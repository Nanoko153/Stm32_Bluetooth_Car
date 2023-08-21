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

//------------����˵��---------------
//TB6612˿ӡ��ʶ--------STM32F1��������

//    PWMA    -----------    B6
//    AIN1    -----------    B8
//    AIN2    -----------    B7
//    STBY    -----------    5V
//    VM      -----------    5-12V����ӵ�Դ��
//    GND     -----------    GND  ����ӵ�Դ��
//    VCC     -----------    5V   ���߼���Դ��
//    GND     -----------    GND   ���߼����أ�
// 

//------------����˵��---------------

//----------��������---------//
void BLUE_Config(void);
void OLED_Display(void);
void Arr_Clear(char *arr,int len);
void Get_Message(void);
void Message_Handler(void);

//----------ȫ�ֱ�������-----//
//#define MAX_MESSAGE_LENG 20
float Pitch,Roll,Yaw;		//�����ǽǶ�P:������ R:��ת�� Y:ƫ����
u8 str[20];					//�ַ�������
char arr[20];					//��Ϣ���黺��

int main(void)
{	
	//�������� 
	int i;
	u16 t;
	u16 len;

	
	SystemInit(); 				//����ϵͳʱ��Ϊ72M   
	delay_init();   			//��ʱ������ʼ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	USART1_Init(115200);
	

	//-----GPIO����-----//
	LED_GPIO_Init();
	Wheels_GPIO_Init();    		//��ʼ��gpio��B pin_7/pin_8

	//-----WHEELS-------//
	Wheels_PWM_Init(7199,0);   	//��ʼ��pwm��� 72000 000 /7199+1=10000 
	//BLUE_Config();
	
	//-----OLED-----//
	OLED_Init();				//=====OLED��ʼ��	
	OLED_ShowString(0,0,"SYSTEM Start Up",16);
	OLED_Refresh_Gram();//������ʾ��OLED	 
	delay_ms(1000);
	OLED_Clear();
	
	TIM_SetCompare1(TIM4,4000);   	//����TIM4ͨ��1��ռ�ձ�  3000/7200
	TIM_SetCompare2(TIM4,4000);   	//����TIM4ͨ��2��ռ�ձ�  3000/7200
	TIM_SetCompare3(TIM4,4000);   	//����TIM4ͨ��3��ռ�ձ�  3000/7200
	TIM_SetCompare4(TIM4,4000);   	//����TIM4ͨ��4��ռ�ձ�  3000/7200
	
	
	//-----MPU6050-----//
	IIC_Init();
	MPU6050_initialize();    	//=====MPU6050��ʼ��	
	DMP_Init();
	//TIM2_Getsample_Int(1999,719);		//50ms����ʱ�ж�
	
	
	printf("OK\r\n");
	
	while(1)
	{
		delay_ms(10);						//������ʱ
		
		Read_DMP(&Pitch,&Roll,&Yaw);		//����MPU����������
		
		if((i++)%10==0)							//һ��ʱ�����OLED��ʾ
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
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			
			Arr_Clear(arr,20);
			
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
				arr[t] = USART_RX_BUF[t];
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}
		*/
		
		Get_Message();
		Message_Handler();
	}
}

//ATָ������ģ�飬����4.0ģ�鲻�ܸ��ֻ���������
void BLUE_Config(void)
{
	//����ATָ��
	printf("AT\r\n");
	delay_ms(500);
	
	//���͸���ģ������ָ���Ҫ��λ��Ч���������AT+RESET������������û�б������ô���¶�����ģ���ϵ磩
	printf("AT+NAMENano\r\n");
	delay_ms(500);

	//printf("AT+UART=9600,0,0\r\n");
	//delay_ms(500);
	
	//���͸�λģ���ָ��
	printf	("AT+RESET\r\n");
	delay_ms(2000);
}

//OLED��ʾ����
void OLED_Display(void)
{
	u8 stri[20];
		//OLED_Clear();							//����
		sprintf((char *)stri,"Pitch:%.2f",Pitch);//��ʾ������P:������
		OLED_ShowString(16,16,stri,16);
		sprintf((char *)stri,"Roll :%.2f",Roll);	//��ʾ������R:��ת��
		OLED_ShowString(16,32,stri,16);
		sprintf((char *)stri,"Yaw  :%.2f",Yaw);	//��ʾ������Y:ƫ����
		OLED_ShowString(16,48,stri,16);
		OLED_Refresh_Gram();//������ʾ��OLED	 
}

//������պ���
//����������arr ��ճ���
void Arr_Clear(char *arr,int len)
{
	int i;
	for(i = 0; i<len; i++)
	{
		*arr = '\0';
		arr++;
	}
}



//���մ���1��Ϣ������Ϣ���浽message_arr��Ϣ����������
void Get_Message(void)
{
	u16 len;
	u16 t;
	if(USART_RX_STA&0x8000)
	{					   
		len=USART_RX_STA&0x3fff;									//�õ��˴ν��յ������ݳ���
		printf("\r\nSTM32 get data:\t");
			
		Arr_Clear(arr,20);									//�����Ϣ�������½���
			
		for(t=0;t<len;t++)
		{
			USART_SendData(USART1, USART_RX_BUF[t]);				//�򴮿�1��������
			arr[t] = USART_RX_BUF[t];						//����õ���Ϣ���浽message_arr��Ϣ����
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);	//�ȴ����ͽ���
		}
		printf("\r\n");//���뻻��
		USART_RX_STA=0;
	}
	//else
		//printf("No Message");
}

void Message_Handler(void)
{
	if(arr[0] == '0' || arr[0] == '\0')				//��Ϣ����ͷ�ַ�Ϊ0����������
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
		
	//��Ϣ�����߼�
	printf("working for: ");
	strcpy(str,arr);
	printf(str);
	printf("\r\n");//���뻻��
	printf("%c",arr[0]);//���뻻��
	printf("\r\n");//���뻻��
	
	
}

