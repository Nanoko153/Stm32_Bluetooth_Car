#include "moto.h"
#include "sys.h"



//***********************************************//
//函数名：	Weel_Control(int index, int mode)
//参数：		index	轮胎编号索引
//			mode    转动模式		1正转	0停止	-1反转
//返回值：	null
//***********************************************//
void Weel_Control(int index, int mode)
{
	int IOin1,IOin2;
	int temp;
	
	IOin1 = index * 2 - 2;			//映射轮胎控制引脚
	IOin2 = index * 2 - 1;
	
	temp = index % 2 == 0 ? 1 : -1;
	mode = mode * temp;
	
	if(mode == 1)					//正转
	{
		PAout(IOin1) = 0;
		PAout(IOin2) = 1;
	}
	if(mode == 0)
	{
		PAout(IOin1) = 0;
		PAout(IOin2) = 0;
	}
	if(mode == -1)					//反转
	{
		PAout(IOin1) = 1;
		PAout(IOin2) = 0;
	}
}

void Car_MoveUp(void)    			//前进
{
	Weel_Control(1,1);
	Weel_Control(2,1);
	Weel_Control(3,1);
	Weel_Control(4,1);
}

void Car_MoveBack(void)     		//后退
{
	Weel_Control(1,-1);
	Weel_Control(2,-1);
	Weel_Control(3,-1);
	Weel_Control(4,-1);
}



void Car_RotationLeft(void)    		//左转
{
	Weel_Control(1,-1);
	Weel_Control(2,1);
	Weel_Control(3,-1);
	Weel_Control(4,1);
}

void Car_RotationRight(void)   		//右转
{
	Weel_Control(1,1);
	Weel_Control(2,-1);
	Weel_Control(3,1);
	Weel_Control(4,-1);
}

void Car_Stop(void)   				//停止
{
	Weel_Control(1,0);
	Weel_Control(2,0);
	Weel_Control(3,0);
	Weel_Control(4,0);
}
