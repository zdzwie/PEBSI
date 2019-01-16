/**
 ******************************Copyright(c)*************************
 *****************Jakub Brzezowski&Bart³omiej Banaœ*****************
 **
 **----------------------------File info----------------------------
 **
 **
 ** File name:               I2C_Driver.c
 ** Descriptions:            I2C configuration.
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

#include "I2C_Driver.h"

//-------------------------Functions Code------------------------------

/**
 * @brief Port initialization.
 * @author Jakub Brzezowski
 * @version 1.2 15-01-2019
 * @param none
 * @retval none
 */
void gpio_init(void){

	GPIO_InitTypeDef gpio;

// Distribution clock to port.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

// Port Configuration.
	 GPIO_StructInit(&gpio);
	 gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // SCL, SDA
	 gpio.GPIO_Mode = GPIO_Mode_AF_OD;
	 gpio.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &gpio);

}
/**
 * @brief I2C_configuration.
 * @author Jakub Brzezowski
 * @version 1.2 15-01-2019
 * @param none
 * @retval none
 */
void i2c_config(void){

	I2C_InitTypeDef i2c;

	// Distribution clock to port.
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	 //Transmission configuration.
	  I2C_StructInit(&i2c);
	  i2c.I2C_Mode = I2C_Mode_I2C;
	  i2c.I2C_Ack = I2C_Ack_Enable;
	  i2c.I2C_ClockSpeed = 100000;
	  I2C_Init(I2C1, &i2c);
	  I2C_Cmd(I2C1, ENABLE);

}
/**
 * @brief I2C transmission_initialization.
 * @author Jakub Brzezowski
 * @version 1.2 15-01-2019
 * @param none
 * @retval none
 */
void transmission_init(void){

	gpio_init();
	i2c_init();
}
