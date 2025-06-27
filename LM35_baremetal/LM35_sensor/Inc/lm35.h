/*
 * lm35.h
 *
 *  Created on: Jun 5, 2025
 *      Author: rahul
 */

#ifndef LM35_H_
#define LM35_H_

#include "stm32f4xx.h"


void LM35_Init(void);
float LM35_ReadTemperature(void);

#endif /* LM35_H_ */
