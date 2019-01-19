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

void eeprom_set_addr(uint32_t addr);
void eeprom_write(uint32_t addr,const void* data,int size);
void eeprom_read(uint32_t addr, void* data, int size);

#endif /* I2C_DRIVER_H_ */


