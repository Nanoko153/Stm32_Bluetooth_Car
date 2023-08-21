#include "moto.h"
#include "sys.h"



//***********************************************//
//��������	Weel_Control(int index, int mode)
//������		index	��̥�������
//			mode    ת��ģʽ		1��ת	0ֹͣ	-1��ת
//����ֵ��	null
//***********************************************//
void Weel_Control(int index, int mode)
{
	int IOin1,IOin2;
	int temp;
	
	IOin1 = index * 2 - 2;			//ӳ����̥��������
	IOin2 = index * 2 - 1;
	
	temp = index % 2 == 0 ? 1 : -1;
	mode = mode * temp;
	
	if(mode == 1)					//��ת
	{
		PAout(IOin1) = 0;
		PAout(IOin2) = 1;
	}
	if(mode == 0)
	{
		PAout(IOin1) = 0;
		PAout(IOin2) = 0;
	}
	if(mode == -1)					//��ת
	{
		PAout(IOin1) = 1;
		PAout(IOin2) = 0;
	}
}

void Car_MoveUp(void)    			//ǰ��
{
	Weel_Control(1,1);
	Weel_Control(2,1);
	Weel_Control(3,1);
	Weel_Control(4,1);
}

void Car_MoveBack(void)     		//����
{
	Weel_Control(1,-1);
	Weel_Control(2,-1);
	Weel_Control(3,-1);
	Weel_Control(4,-1);
}



void Car_RotationLeft(void)    		//��ת
{
	Weel_Control(1,-1);
	Weel_Control(2,1);
	Weel_Control(3,-1);
	Weel_Control(4,1);
}

void Car_RotationRight(void)   		//��ת
{
	Weel_Control(1,1);
	Weel_Control(2,-1);
	Weel_Control(3,1);
	Weel_Control(4,-1);
}

void Car_Stop(void)   				//ֹͣ
{
	Weel_Control(1,0);
	Weel_Control(2,0);
	Weel_Control(3,0);
	Weel_Control(4,0);
}
