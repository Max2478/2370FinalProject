/*
 * RNG.c
 *
 *  Created on: May 1, 2025
 *      Author: mmkna
 */

#include "RNG.h"

//extern RNG_HandleTypeDef hrng;

void MX_RNG_Init(void){
	hrng.Instance = RNG;
	if (HAL_RNG_Init(&hrng) != HAL_OK)
	{
		__disable_irq();
		  while (1)
		  {
		  }
	}
}
