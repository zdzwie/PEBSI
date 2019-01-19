/**
 ******************************Copyright(c)*************************
 *****************Jakub Brzezowski&Bart³omiej Banaœ*****************
 **
 **----------------------------File info----------------------------
 **
 **
 ** File name:               memory.c
 ** Descriptions:            Flash memory driver.
 **-----------------------------------------------------------------
 ** Created by:              Jakub Brzezowski
 ** Created date:            2019-01-15
 ** Version:                 v1.5
 ** Descriptions:            Library uses StPeriph.
 ** Brief:					 First version of these library uses HAL library,
 ** 						 but code aren't effective and during testing have problem with real time operating.
 ** ---------------------------------------------------------------
 ** Modified by:
 ** Modified date:
 ** Version:
 ** Descriptions:
 **
 */
/**
 ******************************Copyright(c)*************************
 *****************Jakub Brzezowski&Bartłomiej Banaś*****************
 **
 **----------------------------File info----------------------------
 **
 **
 ** File name:               memory.c
 ** Descriptions:            Flash memory driver.
 **-----------------------------------------------------------------
 ** Created by:              Jakub Brzezowski
 ** Created date:            2019-01-15
 ** Version:                 v1.5
 ** Descriptions:            Library uses StPeriph.
 ** Brief:					 First version of these library uses HAL library,
 ** 						 but code aren't effective and during testing have problem with real time operating.
 ** ---------------------------------------------------------------
 ** Modified by:
 ** Modified date:
 ** Version:
 ** Descriptions:
 **
 */
#include "memory.h"
//-------------------------Functions Code------------------------------

void data_from_pc_read(uint8_t* buffer) {

	USART_Configuration();

	uint8_t len = sizeof(buffer) / sizeof(buffer[0]);
	uint8_t i = 0;
	uint8_t c;

	while (i < (len - 1)) {
		i++;
		c = USART_receive_data();
		if (c != NULL) {
			buffer[i] = c;
		} else {
			i--;
		}
	}
}

NAND_ADDRESS write_read_address_set(uint16_t zone, uint16_t block, uint16_t page) {

	NAND_ADDRESS WriteReadAddr;

	WriteReadAddr.Zone = zone;
	WriteReadAddr.Block = block;
	WriteReadAddr.Page = page;

	return WriteReadAddr;
}

void write_buffer(uint8_t* buffer, NAND_ADDRESS WriteReadAddr, uint8_t page) {

	FSMC_NANDDeInit(FSMC_Bank2_NAND);
	FSMC_NAND_Init();

	FSMC_NAND_EraseBlock(WriteReadAddr);
	FSMC_NAND_WriteSmallPage(buffer, WriteReadAddr, page);

}

void read_buffer(uint8_t* buffer, NAND_ADDRESS WriteReadAddr, uint8_t page) {

	FSMC_NANDDeInit(FSMC_Bank2_NAND);
	FSMC_NAND_Init();

	FSMC_NAND_WriteSmallPage(buffer, WriteReadAddr, page);

}
void data_from_pc_in_memory_write(uint16_t zone, uint16_t block, uint16_t page) {

	int buffer[256];
	data_from_pc_read(*buffer);

	NAND_IDTypeDef NAND_ID;
	NAND_ADDRESS WriteReadAddr = write_read_address_set(zone, block, page);

}
    }
 I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
    buffer[i] = I2C_ReceiveData(I2C1);
}
