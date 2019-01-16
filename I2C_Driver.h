/*
 * I2C_Driver.h
 *
 *  Created on: 15.01.2019
 *      Author: Jakub
 */

#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include "stm32f10x.h"

void gpio_init(void);
void i2c_config(void);
void transmission_init(void);

#endif /* I2C_DRIVER_H_ */


