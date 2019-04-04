/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gpio.h
 * Author: ruslan
 *
 * Created on 28 июля 2018 г., 15:45
 */

#ifndef GPIO_H
#define GPIO_H

#include <stm32f4xx.h>

extern void gpio_afr_set(GPIO_TypeDef * gpio,uint8_t gpio_pin_n,uint8_t gpio_pin_af);

#endif /* GPIO_H */

