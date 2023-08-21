#ifndef __MOTO_H
#define	__MOTO_H

#include "stm32f10x.h"


#define GO 1
#define BK 0

void Weel_Control(int index, int mode);
void Car_MoveUp(void);
void Car_MoveBack(void);
void Car_RotationLeft(void);
void Car_RotationRight(void);
void Car_Stop(void);

#endif
