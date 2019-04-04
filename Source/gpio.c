/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gpio.h"

void gpio_afr_set(GPIO_TypeDef * gpio,uint8_t gpio_pin_n,uint8_t gpio_pin_af) {
    gpio->AFR[gpio_pin_n / 8] &= ~(uint32_t) ((0b1111 << ((gpio_pin_n % 8)*4))); //Clear AF
    gpio->AFR[gpio_pin_n / 8] |= (uint32_t) (((0b1111 & gpio_pin_af) << ((gpio_pin_n % 8)*4))); //Set AF
}