/**
 ******************************Copyright(c)*************************
 *****************Jakub Brzezowski&Bartłomiej Banaś*****************
 **
 **----------------------------File info----------------------------
 **
 **
 ** File name:               vs1003.c
 ** Descriptions:            .
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

/* Includes ------------------------------------------------------------------*/
#include "vs1003.h"
#include "stm32f10x.h"

#define SPI_SPEED_HIGH    1
#define SPI_SPEED_LOW     0

#define MP3_Reset(x)	  x ? GPIO_SetBits(GPIOA,GPIO_Pin_5) : GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define MP3_CCS(x)	  x ? GPIO_SetBits(GPIOB,GPIO_Pin_12) : GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define MP3_DCS(x)	  x ? GPIO_SetBits(GPIOA,GPIO_Pin_4) : GPIO_ResetBits(GPIOA,GPIO_Pin_4)


#define MP3_DREQ          GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)


#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03

#define SPI_MODE        	0x00
#define SPI_STATUS      	0x01
#define SPI_BASS        	0x02
#define SPI_CLOCKF      	0x03
#define SPI_DECODE_TIME 	0x04
#define SPI_AUDATA      	0x05
#define SPI_WRAM        	0x06
#define SPI_WRAMADDR    	0x07
#define SPI_HDAT0       	0x08
#define SPI_HDAT1       	0x09
#define SPI_AIADDR      	0x0a
#define SPI_VOL         	0x0b
#define SPI_AICTRL0     	0x0c
#define SPI_AICTRL1     	0x0d
#define SPI_AICTRL2     	0x0e
#define SPI_AICTRL3     	0x0f
#define SM_DIFF         	0x01
#define SM_JUMP         	0x02
#define SM_RESET        	0x04
#define SM_OUTOFWAV     	0x08
#define SM_PDOWN        	0x10
#define SM_TESTS        	0x20
#define SM_STREAM       	0x40
#define SM_PLUSV        	0x80
#define SM_DACT         	0x100
#define SM_SDIORD       	0x200
#define SM_SDISHARE     	0x400
#define SM_SDINEW       	0x800
#define SM_ADPCM        	0x1000
#define SM_ADPCM_HP     	0x2000

/* Private variables ---------------------------------------------------------*/

/* VS1003ÉèÖÃ²ÎÊý */
/* 0 , henh.1 , hfreq.2 , lenh.3 , lfreq 5 ,Ö÷ÒôÁ¿ */
uint8_t vs1003ram[5] = { 0 , 0 , 0 , 0 , 250 };

/*******************************************************************************
* Function Name  : delay_ms
* Description    : delay time
* Input          : - ms: time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint32_t ms)
{
    uint32_t len;
    for (;ms > 0; ms --)
        for (len = 0; len < 100; len++ );
}

/*******************************************************************************
* Function Name  : VS1003_SPI_Init
* Description    : VS1003 SPI Init
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void VS1003_SPI_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE);
  /* SPI2 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);
}

/*******************************************************************************
* Function Name  : VS1003_SPI_SetSpeed
* Description    : None
* Input          : - SpeedSet: SPI_SPEED_HIGH 1  SPI_SPEED_LOW 0
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void VS1003_SPI_SetSpeed( uint8_t SpeedSet)
{
  SPI_InitTypeDef SPI_InitStructure ;

  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex ;
  SPI_InitStructure.SPI_Mode=SPI_Mode_Master ;
  SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b ;
  SPI_InitStructure.SPI_CPOL=SPI_CPOL_High ;
  SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge ;
  SPI_InitStructure.SPI_NSS=SPI_NSS_Soft ;

  if( SpeedSet == SPI_SPEED_LOW )
  {
      SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;
  }
  else
  {
      SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
  }
  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB ;
  SPI_InitStructure.SPI_CRCPolynomial=7 ;
  SPI_Init(SPI2,&SPI_InitStructure);
}

/*******************************************************************************
* Function Name  : VS1003_SPI_ReadWriteByte
* Description    : None
* Input          : - TxData: Write Byte
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static uint8_t VS1003_SPI_ReadWriteByte( uint8_t TxData )
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI2, TxData);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/*******************************************************************************
* Function Name  : VS1003_Init
* Description    : VS1003¶Ë¿Ú³õÊ¼»¯
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
  /* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13  | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* XCS */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* XDCS */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* DREQ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Reset */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  MP3_Reset(0);
  delay_ms(1);
  MP3_Reset(1);

  MP3_DCS(1);
  MP3_CCS(1);

  VS1003_SPI_Init();
}


/*******************************************************************************
* Function Name  : VS1003_WriteReg
* Description    : VS1003Ð´¼Ä´æÆ÷
* Input          : - reg: ÃüÁîµØÖ·
*				   - value: ÃüÁîÊý¾Ý
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_WriteReg( uint8_t reg,uint16_t value )
{
   while(  MP3_DREQ ==0 );           /* µÈ´ý¿ÕÏÐ */

   VS1003_SPI_SetSpeed( SPI_SPEED_LOW );
   MP3_DCS(1);
   MP3_CCS(0);
   VS1003_SPI_ReadWriteByte(VS_WRITE_COMMAND); /* ·¢ËÍVS1003µÄÐ´ÃüÁî */
   VS1003_SPI_ReadWriteByte(reg);
   VS1003_SPI_ReadWriteByte(value>>8);
   VS1003_SPI_ReadWriteByte(value);
   MP3_CCS(1);
   VS1003_SPI_SetSpeed( SPI_SPEED_HIGH );
}

/*******************************************************************************
* Function Name  : VS1003_ReadReg
* Description    : VS1003¶Á¼Ä´æÆ÷
* Input          : - reg: ÃüÁîµØÖ·
* Output         : None
* Return         : - value: Êý¾Ý
* Attention		 : None
*******************************************************************************/
uint16_t VS1003_ReadReg( uint8_t reg)
{
   uint16_t value;

   while(  MP3_DREQ ==0 );           /* µÈ´ý¿ÕÏÐ */
   VS1003_SPI_SetSpeed( SPI_SPEED_LOW );
   MP3_DCS(1);
   MP3_CCS(0);
   VS1003_SPI_ReadWriteByte(VS_READ_COMMAND);/* ·¢ËÍVS1003µÄ¶ÁÃüÁî */
   VS1003_SPI_ReadWriteByte( reg );
   value = VS1003_SPI_ReadWriteByte(0xff);
   value = value << 8;
   value |= VS1003_SPI_ReadWriteByte(0xff);
   MP3_CCS(1);
   VS1003_SPI_SetSpeed( SPI_SPEED_HIGH );
   return value;
}

/*******************************************************************************
* Function Name  : VS1003_ResetDecodeTime
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_ResetDecodeTime(void)
{
   VS1003_WriteReg(SPI_DECODE_TIME,0x0000);
   VS1003_WriteReg(SPI_DECODE_TIME,0x0000); /* ²Ù×÷Á½´Î */
}

/*******************************************************************************
* Function Name  : VS1003_GetDecodeTime
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint16_t VS1003_GetDecodeTime(void)
{
   return VS1003_ReadReg(SPI_DECODE_TIME);
}

/*******************************************************************************
* Function Name  : VS1003_SoftReset
* Description    : VS1003Èí¸´Î»
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_SoftReset(void)
{
   uint8_t retry;

   while(  MP3_DREQ ==0 );   /* µÈ´ýÈí¼þ¸´Î»½áÊø */
   VS1003_SPI_ReadWriteByte(0xff);  /* Æô¶¯´«Êä */
   retry = 0;
   while( VS1003_ReadReg(0) != 0x0804 )   /* Èí¼þ¸´Î»,ÐÂÄ£Ê½ */
   {
	   VS1003_WriteReg(SPI_MODE,0x0804);  /* Èí¼þ¸´Î»,ÐÂÄ£Ê½ */
	   delay_ms(2);                        /* µÈ´ýÖÁÉÙ1.35ms */
	   if( retry++ > 100 )
	   {
	      break;
	   }
   }

   while(  MP3_DREQ ==0 );   /* µÈ´ýÈí¼þ¸´Î»½áÊø */

   retry = 0;

   while( VS1003_ReadReg(SPI_CLOCKF) != 0X9800 ) /* ÉèÖÃvs1003µÄÊ±ÖÓ,3±¶Æµ ,1.5xADD */
   {
	   VS1003_WriteReg(SPI_CLOCKF,0X9800);       /* ÉèÖÃvs1003µÄÊ±ÖÓ,3±¶Æµ ,1.5xADD */
	   if( retry++ > 100 )
	   {
	      break;
	   }
   }
   retry = 0;

   while( VS1003_ReadReg(SPI_AUDATA) != 0XBB81 ) /* ÉèÖÃvs1003µÄÊ±ÖÓ,3±¶Æµ ,1.5xADD */
   {
	   VS1003_WriteReg(SPI_AUDATA,0XBB81);
	   if( retry++ > 100 )
	   {
	      break;
	   }
   }

   VS1003_WriteReg(11,0x2020);	 /* ÒôÁ¿ */
   VS1003_ResetDecodeTime();     /* ¸´Î»½âÂëÊ±¼ä */
   /* Ïòvs1003·¢ËÍ4¸ö×Ö½ÚÎÞÐ§Êý¾Ý£¬ÓÃÒÔÆô¶¯SPI·¢ËÍ */
   MP3_DCS(0);  /* Ñ¡ÖÐÊý¾Ý´«Êä */
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   MP3_DCS(1);  /* È¡ÏûÊý¾Ý´«Êä */
   delay_ms(20);
}

/*******************************************************************************
* Function Name  : VS1003_Reset
* Description    : VS1003¸´Î»
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_Reset(void)
{
   delay_ms(20);
   VS1003_SPI_ReadWriteByte(0XFF);
   MP3_DCS(1);              /* È¡ÏûÊý¾Ý´«Êä */
   MP3_CCS(1);              /* È¡ÏûÊý¾Ý´«Êä */

   while(  MP3_DREQ ==0 );  /* µÈ´ýDREQÎª¸ß */
   delay_ms(20);
}

/*******************************************************************************
* Function Name  : VS1003_SineTest
* Description    : VS1003ÕýÏÒ²âÊÔ
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_SineTest(void)
{
   VS1003_WriteReg(0x0b,0X2020);	  /* ÉèÖÃÒôÁ¿ */
   VS1003_WriteReg(SPI_MODE,0x0820); /* ½øÈëvs1003µÄ²âÊÔÄ£Ê½ */
   while(  MP3_DREQ ==0 );  /* µÈ´ýDREQÎª¸ß */

   /* Ïòvs1003·¢ËÍÕýÏÒ²âÊÔÃüÁî£º0x53 0xef 0x6e n 0x00 0x00 0x00 0x00 */
   MP3_DCS(0);/* Ñ¡ÖÐÊý¾Ý´«Êä */
   VS1003_SPI_ReadWriteByte(0x53);
   VS1003_SPI_ReadWriteByte(0xef);
   VS1003_SPI_ReadWriteByte(0x6e);
   VS1003_SPI_ReadWriteByte(0x24);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);
   /* ÍË³öÕýÏÒ²âÊÔ */
   MP3_DCS(0);/* Ñ¡ÖÐÊý¾Ý´«Êä */
   VS1003_SPI_ReadWriteByte(0x45);
   VS1003_SPI_ReadWriteByte(0x78);
   VS1003_SPI_ReadWriteByte(0x69);
   VS1003_SPI_ReadWriteByte(0x74);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);

   /* ÔÙ´Î½øÈëÕýÏÒ²âÊÔ²¢ÉèÖÃnÖµÎª0x44£¬¼´½«ÕýÏÒ²¨µÄÆµÂÊÉèÖÃÎªÁíÍâµÄÖµ */
   MP3_DCS(0);/* Ñ¡ÖÐÊý¾Ý´«Êä */
   VS1003_SPI_ReadWriteByte(0x53);
   VS1003_SPI_ReadWriteByte(0xef);
   VS1003_SPI_ReadWriteByte(0x6e);
   VS1003_SPI_ReadWriteByte(0x44);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);
   /* ÍË³öÕýÏÒ²âÊÔ */
   MP3_DCS(0);/* Ñ¡ÖÐÊý¾Ý´«Êä */
   VS1003_SPI_ReadWriteByte(0x45);
   VS1003_SPI_ReadWriteByte(0x78);
   VS1003_SPI_ReadWriteByte(0x69);
   VS1003_SPI_ReadWriteByte(0x74);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);
}

/*******************************************************************************
* Function Name  : VS1003_RamTest
* Description    : VS1003 RAM²âÊÔ
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_RamTest(void)
{
   volatile uint16_t value;

   VS1003_Reset();
   VS1003_WriteReg(SPI_MODE,0x0820);/* ½øÈëvs1003µÄ²âÊÔÄ£Ê½ */
   while(  MP3_DREQ ==0 );           /* µÈ´ýDREQÎª¸ß */
   MP3_DCS(0);	       			     /* xDCS = 1£¬Ñ¡Ôñvs1003µÄÊý¾Ý½Ó¿Ú */
   VS1003_SPI_ReadWriteByte(0x4d);
   VS1003_SPI_ReadWriteByte(0xea);
   VS1003_SPI_ReadWriteByte(0x6d);
   VS1003_SPI_ReadWriteByte(0x54);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(50);
   MP3_DCS(1);
   value = VS1003_ReadReg(SPI_HDAT0); /* Èç¹ûµÃµ½µÄÖµÎª0x807F£¬Ôò±íÃ÷ÍêºÃ */
}

/*******************************************************************************
* Function Name  : VS1003_SetVol
* Description    : Éè¶¨vs1003²¥·ÅµÄÒôÁ¿ºÍ¸ßµÍÒô
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_SetVol(void)
{
   uint8_t i;
   uint16_t bass=0;  /* ÔÝ´æÒôµ÷¼Ä´æÆ÷Öµ */
   uint16_t volt=0;  /* ÔÝ´æÒôÁ¿Öµ */
   uint8_t  vset=0;  /* ÔÝ´æÒôÁ¿Öµ */

   vset = 255 - vs1003ram[4]; /* È¡·´µÃµ½×î´óÖµ */
   volt =vset;
   volt <<= 8;
   volt += vset;/* µÃµ½ÒôÁ¿ÉèÖÃºó´óÐ¡ */
   /* 0,henh.1,hfreq.2,lenh.3,lfreq */
   for( i = 0; i < 4; i++ )
   {
       bass <<= 4;
       bass += vs1003ram[i];
   }
   VS1003_WriteReg(SPI_BASS,bass);/* BASS */
   VS1003_WriteReg(SPI_VOL,volt); /* ÉèÒôÁ¿ */
}

/*******************************************************************************
* Function Name  : VS1003_WriteData
* Description    : VS1003Ð´Êý¾Ý
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_WriteData( uint8_t * buf)
{
   uint8_t  count = 32;

   MP3_DCS(0);
   while( count-- )
   {
	  VS1003_SPI_ReadWriteByte( *buf++ );
   }
   MP3_DCS(1);
   MP3_CCS(1);
}

/*******************************************************************************
* Function Name  : VS1003_Record_Init
* Description    : VS1003Â¼Òô³õÊ¼»¯
* Input          : None
* Output         : None
* Return         : None
* Attention		 : VS1003Â¼Òô³õÊ¼»¯³É¹¦Ö®ºó¿ÉÒÔÌýµ½¶ú»úÓÐ»Ø·Å
*                  ´ËÊ±²»ÄÜÓÃÀ®°ÈÈÝÒ×²úÉú×Ô¼¤
*******************************************************************************/
void VS1003_Record_Init(void)
{
  uint8_t retry;

  /* ÅäÖÃÊ±ÖÓ */
  while( VS1003_ReadReg(SPI_CLOCKF) != 0x9800 )
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x9800);
	  delay_ms(2);                        /* µÈ´ýÖÁÉÙ1.35ms */
	  if( retry++ > 100 )
	  {
	      break;
	  }
  }

  while( VS1003_ReadReg(SPI_BASS) != 0x0000 )
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x0000);
	  delay_ms(2);                        /* µÈ´ýÖÁÉÙ1.35ms */
	  if( retry++ > 100 )
	  {
	      break;
	  }
  }

  /* Set sample rate divider=12 */
  while( VS1003_ReadReg(SPI_AICTRL0) != 0x0012 )
  {
	  VS1003_WriteReg(SPI_AICTRL0,0x0012);
	  delay_ms(2);                        /* µÈ´ýÖÁÉÙ1.35ms */
	  if( retry++ > 100 )
	  {
	      break;
	  }
  }

  /* AutoGain OFF, reclevel 0x1000 */
  while( VS1003_ReadReg(SPI_AICTRL1) != 0x1000 )
  {
	  VS1003_WriteReg(SPI_AICTRL1,0x1000);
	  delay_ms(2);                        /* µÈ´ýÖÁÉÙ1.35ms */
	  if( retry++ > 100 )
	  {
	      break;
	  }
  }

  /* RECORD,NEWMODE,RESET */
  while( VS1003_ReadReg(SPI_MODE) != 0x1804 )
  {
	  VS1003_WriteReg(SPI_MODE,0x1804);
	  delay_ms(2);                        /* µÈ´ýÖÁÉÙ1.35ms */
	  if( retry++ > 100 )
	  {
	      break;
	  }
  }

  while( VS1003_ReadReg(SPI_CLOCKF) != 0x9800 )
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x9800);
	  delay_ms(2);                        /* µÈ´ýÖÁÉÙ1.35ms */
	  if( retry++ > 100 )
	  {
	      break;
	  }
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

/* vs1003.c
 *
 *  Created on: 16.12.2018
 *      Author: Jakub
 */


