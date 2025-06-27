/*
 * lm35.c
 *
 *  Created on: Jun 5, 2025
 *      Author: neha
 */

#include "lm35.h"
#include "stm32f4xx.h"

//Pinout Configuration
// GND-GND PA0--Vout  Vcc-5v Stm

void LM35_Init(void) // set up PA0 and ADC1 TO read analog voltage from the LM35
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 	//GPIOA enable for PA0
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  //for ADC 1 Enaable

	GPIOA->MODER |= BV(0) | BV(1);  //Mode set as analog input 11 for PA0 pin
	ADC1->CR2=0;	//disabled before config
	ADC1->SQR3=0; //Regular Sequence register 3 PA0=ADC1_IN0
	ADC1->SQR1=0; 	// 0 means 1 conversion
	ADC1->SMPR2 |= BV(0) | BV(1) | BV(2);  // Set bits 2:0 = 111 => 480 cycles
	ADC1->CR2 |= ADC_CR2_ADON;   //after all config turn on adc 1

}


float LM35_ReadTemperature(void)  // Start an ADC Conversion and Calculate temo in 0C
{
	ADC1->CR2 |= ADC_CR2_SWSTART;     /// Start software  triggered conversion
	while(!(ADC1->SR & ADC_SR_EOC));//wait untill end of conversion flag is set

	uint16_t adc_val= ADC1->DR;	//12 bit register data is read.(0 to 4095)

	 // Convert to voltage (3.3V ref, 12-bit ADC)
	float voltage =(adc_val * 3.3f) / 4095.0f;        // convert ADC Value to voltage                         
	//As  LM35 gives 10mV per degree  celcius
	float temparature = voltage/0.01;  //voltage * 100;
	return temparature;
}



