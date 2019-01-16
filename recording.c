/**
 ******************************Copyright(c)*************************
 *****************Jakub Brzezowski&Bart³omiej Banaœ*****************
 **
 **----------------------------File info----------------------------
 **
 **
 ** File name:               recording.c
 ** Descriptions:            The VS1003 audio recording.
 **-----------------------------------------------------------------
 ** Created by:              Jakub Brzezowski
 ** Created date:            2018-12-16
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

//Header attaching
#include "recording.h"

//-------------------------Functions Code------------------------------

/**
 * @brief Recording 256 bites of audio samples
 * @note  In case of overflowing data buffer of VS1003 program reset audio software.
 * @author Jakub Brzezowski
 * @version 1.2 16-12-2018
 * @param data is data buffer, where voice samples are saving.
 * @retval Function return 1, when writes all buffer. Otherwise returns 0
 */

uint8_t record_voice(uint16_t* data){															//
	for(uint16_t i = 0; i++;i<256){																//
		if(VS1003_ReadReg(SPI_HDAT1) > 0 & VS1003_ReadReg(SPI_HDAT1) < 896){					// Checking overflowing of data buffer.
		data[i] = VS1003_ReadReg(SPI_HDAT0);													// Writes samples.
		}																						//
		else{																					//
			VS1003_SoftReset();																	// If data buffer is overflowed software resetting.
			data[i] = VS1003_ReadReg(SPI_HDAT0);												//
		}																						//
		if (i==255) return 1;																	// End of buffer.
		else {return 0;}																		// Buffer is still writing.
	}																							// End for loop.
}
