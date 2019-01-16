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
#include "memory.h"
//-------------------------Functions Code------------------------------

/**
 * @brief Setting adres of register, where data was written.
 * @note Data adress is four-bytes.
 * @author Jakub Brzezowski
 * @version 1.2 15-01-2019
 * @param addr is address, where data was written.
 * @retval none
 */
void eeprom_set_addr(uint32_t addr)
{
 I2C_GenerateSTART(I2C1, ENABLE);
 while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
 I2C_Send7bitAddress(I2C1, 0xa0, I2C_Direction_Transmitter);
 while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);

 I2C_SendData(I2C1, addr);
 while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
}

/**
 * @brief Write data in memory.
 * @author Jakub Brzezowski
 * @version 1.2 15-01-2019
 * @param addr is address, where data was written.
 * @param data is data to save in memomry.
 * @param size is size of data.
 * @retval none
 */
void eeprom_write(uint32_t addr, const void* data, int size)
{
 int i;
 const uint8_t* buffer = (uint8_t*)data;

 eeprom_set_addr(addr);
 for (i = 0; i < size; i++) {
 I2C_SendData(I2C1, buffer[i]);
 while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
 }
 I2C_GenerateSTOP(I2C1, ENABLE);
}
/**
 * @brief Read data from memory.
 * @author Jakub Brzezowski
 * @version 1.2 15-01-2019
 * @param addr is address, from data was written.
 * @param data is place, where read data, was written.
 * @param size is size of data.
 * @retval none
 */
void eeprom_read(uint32_t addr, void* data, int size)
{
 int i;
 uint8_t* buffer = (uint8_t*)data;

 eeprom_set_addr(addr);

 I2C_GenerateSTART(I2C1, ENABLE);
 while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

 I2C_AcknowledgeConfig(I2C1, ENABLE);
 I2C_Send7bitAddress(I2C1, 0xa0, I2C_Direction_Receiver);
 while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);

 for (i = 0; i < size - 1; i++) {
 while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
     buffer[i] = I2C_ReceiveData(I2C1);
    }
 I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
    buffer[i] = I2C_ReceiveData(I2C1);
}
