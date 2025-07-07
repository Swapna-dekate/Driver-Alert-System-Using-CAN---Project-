/*
 * hcsr_baremetal_f407.c
 * Bare-metal HC-SR04 driver for STM32F407
 * TRIG: PB0 (Output)
 * ECHO: PB1 (Input)
 */

#include "stm32f4xx.h"  // Use STM32F4 header

#define TRIG_PIN   0     // PB0
#define ECHO_PIN   1     // PB1

void HCSR04_Init(void) {
    // Enable GPIOB and TIM2 clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;    // Enable GPIOB clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;     // Enable TIM2 clock

    //TRIG (PB0) - Output push-pull
    GPIOB->MODER &= ~(BV(TRIG_PIN * 2) | BV(TRIG_PIN * 2 + 1)); // Clear All the bits
    GPIOB->MODER |=  BV(TRIG_PIN * 2); // General Purpose Output Mode
    GPIOB->OTYPER &= ~BV(TRIG_PIN);        // Push-pull
    GPIOB->OSPEEDR |=  BV(TRIG_PIN * 2) | BV(TRIG_PIN * 2 + 1);// High speed
    GPIOB->PUPDR &= ~(BV(TRIG_PIN * 2) | BV(TRIG_PIN * 2 + 1));// No pull-up/down
    //ECHO (PB1) --Input 00
    GPIOB->MODER &= (BV(ECHO_PIN * 2) | BV(ECHO_PIN * 2 + 1)); // Input mode
    GPIOB->PUPDR &= ~(BV(ECHO_PIN * 2) | BV(ECHO_PIN * 2 + 1)); // No pull

    //Timer 2 Settings for micro-second delay
    TIM2->PSC = (SystemCoreClock / 1000000) - 1; // Prescaler for 1 µs tick
    TIM2->ARR = 0xFFFFFFFF;
    TIM2->CR1 |= TIM_CR1_CEN; // Enable TIM2
}

void delay_us(uint32_t us) {
    TIM2->CNT = 0;
    while (TIM2->CNT < us);
}

void HCSR04_Trigger(void) {
	//send 10micro seconds HIGH pulse to the trigger pin

	GPIOB->BSRR = (BV(TRIG_PIN + 16));  // Reset
    delay_us(2);				//Short delay to ensure LOW States
    GPIOB->BSRR =  BV(TRIG_PIN);         // Set
    delay_us(10);				//Keep high for 10microSec to send an ultrasonic burst

    GPIOB->BSRR = (BV(TRIG_PIN + 16));  // Reset
}

float HCSR04_ReadDistance(void) {
    uint32_t start, stop;

    HCSR04_Trigger();		//calls the trigger action

    // Wait for echo to go HIGH
    while (!(GPIOB->IDR & BV (ECHO_PIN)));

    start = TIM2->CNT;

    // Wait for echo to go LOW
    while (GPIOB->IDR & (1 << ECHO_PIN));

    stop = TIM2->CNT;

    // Handle counter rollover
    uint32_t duration = (stop >= start) ? (stop - start) : (0xFFFFFFFF - start + stop);

    // Distance (cm) = duration (µs) / 58.0
    return duration / 58.0f;
}


//Distance = (Time × Speed of Sound) / 2
//			= (µs × 0.0343) / 2 ≈ µs / 58
