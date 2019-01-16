/*
 * memory.h
 *
 *  Created on: 15.01.2019
 *      Author: Jakub
 */
#include "stm32f10x.h"
#include "I2C_Driver.h"

#ifndef MEMORY_H_
#define MEMORY_H_

void eeprom_set_addr(uint32_t addr);
void eeprom_write(uint32_t addr,const void* data,int size);
void eeprom_read(uint32_t addr, void* data, int size);


#endif /* MEMORY_H_ */
