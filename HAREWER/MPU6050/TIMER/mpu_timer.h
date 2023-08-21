#ifndef __MPU_TIMER_H
#define __MPU_TIMER_H

#include "stm32f10x.h"
#include "string.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"

extern float Pitch,Roll,Yaw;

void TIM2_Getsample_Int(u16 arr,u16 psc);

#endif
