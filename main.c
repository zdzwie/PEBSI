#include "stm32f10x.h"
#include "vs1003.h"
#include "recording.h"
#include "I2C_Driver.h"
#include "fsmc_nand.h"
#include "usart.h"

#define PATTERN_ZONE 0;
#define PATTERN_BLOCK 0;
#define PATTERN_PAGE 0;

/**
 * @brief MODE 0 - TEST 1 - Flash memory test.
 * @brief MODE 1 - TEST 2 - Transmit recording voice to PC by USART
 * @brief MODE 2 - TEST 3 - Record voice, write to memory, read and play by lineout VS1003
 * @brief MODE 3 - WRITE MEL pattern from PC.
 * @brief MODE 4 - Normal work.
 */
#define MODE 0

int main(void){

#if MODE == 0
	FSMC_NAND_Test();
#elif MODE == 1

	uint16_t data_buffer[256];
	record_voice(data_buffer);

#elif MODE == 2

#elif MODE == 3
	data_from_pc_in_memory_write(PATTERN_ZONE,PATTERN_BLOCK,PATTERN_PAGE);
#elif MODE == 4

#endif

}

