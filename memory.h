/*
 * memory.h
 *
 *  Created on: 15.01.2019
 *      Author: Jakub
 */
#include "stm32f10x.h"
#include "I2C_Driver.h"
#include "fsmc_nand.h"
#include "usart.h"

#ifndef MEMORY_H_
#define MEMORY_H_

void data_from_pc_read(uint8_t*);
NAND_ADDRESS write_read_address_set(uint16_t,uint16_t,uint16_t);
void write_buffer(uint8_t*,NAND_ADDRESS,uint8_t);
void read_buffer(uint8_t*,NAND_ADDRESS,uint8_t);
void data_from_pc_in_memory_write(uint16_t,uint16_t,uint16_t);


#endif /* MEMORY_H_ */

