#ifndef __GPIO_H
#define	__GPIO_H

#include "stm32f10x.h"

#define LED0  	PCout(13)

void LED_GPIO_Init(void);
void Wheels_GPIO_Init(void);
void Bule_GPIO_Init(void);

#endif

