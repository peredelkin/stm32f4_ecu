/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.h
 * Author: ruslan
 *
 * Created on 29 июля 2018 г., 15:30
 */

#ifndef MAIN_H
#define MAIN_H

#include <stm32f4xx.h>
#include "gpio.h"

#define CPS_GPIO GPIOA //crankshaft position sensor port
#define CPS_PIN_N 8 //crankshaft position sensor pin
#define CPS_PIN_AF 1 //AF1

void rcc_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; //GPIO_D
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //GPIO_A
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; // DMA1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // TIM1
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //TIM3
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //TIM4
}

void gpio_led_init() {
    //LED4 PD12 Green
    //LED3 PD13 Orange
    //LED5 PD14 Red
    //LED6 PD15 Blue

    GPIOD->MODER |= GPIO_MODER_MODER12_0; //Green Output
    GPIOD->MODER |= GPIO_MODER_MODER13_0; //Orange Output
    GPIOD->MODER |= GPIO_MODER_MODER14_0; //Red Output
    GPIOD->MODER |= GPIO_MODER_MODER15_0; //Blue Output

    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14;
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;
}

void gpio_usart2_init() {
    //TX PD5 AF7
    GPIOA->MODER &= ~GPIO_MODER_MODER2; //Reset Mode 2
    GPIOA->MODER |= GPIO_MODER_MODER2_1; //Set Alternate Mode
   
    GPIOA->OTYPER &= ~GPIO_OTYPER_ODR_2; //Reset Output Type
    
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2;// Reset Speed 2
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2; //Set Very High Speed 2
    
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR2; // Reset PUPD
    
    gpio_afr_set(GPIOA,2,7);
    //GPIOA->AFR[0] &= ~(uint32_t)(0b1111 << ((2 % 8)*4)); //Reset Alternate 2
    //GPIOA->AFR[0] |= (uint32_t)(0b0111 << ((2 % 8)*4)); //Set AF7 to 2
    
    //RX PD6 AF7
    GPIOD->MODER &= ~GPIO_MODER_MODER6; //Reset Mode 2
    GPIOD->MODER |= GPIO_MODER_MODER6_1; //Set Alternate Mode
   
    GPIOD->OTYPER &= ~GPIO_OTYPER_ODR_6; //Reset Output Type
    
    GPIOD->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;// Reset Speed 2
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6; //Set Very High Speed 2
    
    GPIOD->PUPDR &= ~GPIO_PUPDR_PUPDR6; // Reset PUPD
    
    gpio_afr_set(GPIOD,6,7);
    //GPIOD->AFR[0] &= ~(uint32_t)(0b1111 << ((6 % 8)*4)); //Reset Alternate 6
    //GPIOD->AFR[0] |= (uint32_t)(0b0111 << ((6 % 8)*4)); //Set AF7 to 6
}

void gpio_master_timer_init() {
    //Capture TIMER PA8
    CPS_GPIO->MODER &= ~GPIO_MODER_MODER8;
    CPS_GPIO->MODER |= GPIO_MODER_MODER8_1; //ALT

    CPS_GPIO->PUPDR &= ~GPIO_PUPDR_PUPDR8;
    CPS_GPIO->PUPDR |= GPIO_PUPDR_PUPDR8_1; // PU

    gpio_afr_set(CPS_GPIO,CPS_PIN_N,CPS_PIN_AF);
    //CPS_GPIO->AFR[CPS_PIN_N / 8] &= ~(uint32_t) ((0b1111 << ((CPS_PIN_N % 8)*4))); //Clear AF
    //CPS_GPIO->AFR[CPS_PIN_N / 8] |= (uint32_t) (((0b1111 & CPS_PIN_AF) << ((CPS_PIN_N % 8)*4))); //AF1
}

#endif /* MAIN_H */

