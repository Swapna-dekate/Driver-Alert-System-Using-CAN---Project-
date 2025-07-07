/*
 * sensor.h
 *
 *  Created on: Jun 6, 2025
 *      Author: rahul
 */

#ifndef SENSOR_H_
#define SENSOR_H_



#include <stdint.h>

// Initializes GPIO and Timer for HC-SR04
void HCSR04_Init(void);

// Triggers the ultrasonic sensor
void HCSR04_Trigger(void);

// Delays in microseconds (uses TIM2)
void delay_us(uint32_t us);

// Measures distance and returns value in centimeters
float HCSR04_ReadDistance(void);

#endif /* SENSOR_H_ */

