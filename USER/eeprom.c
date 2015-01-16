#include "eeprom.h"

DMA_InitTypeDef   sEEDMA_InitStructure; 

/** @defgroup STM32_EVAL_I2C_EE_Private_Variables
  * @{
  */
__IO uint16_t  sEEAddress = 0;   
__IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;   
__IO uint16_t* sEEDataReadPointer;   
__IO uint8_t*  sEEDataWritePointer;  
__IO uint8_t   sEEDataNum;


/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;    
   
  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(sEE_I2C, DISABLE);
 
  /* sEE_I2C DeInit */
  I2C_DeInit(sEE_I2C);

  /*!< sEE_I2C Periph clock disable */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, DISABLE);
    
  /*!< GPIO configuration */  
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure and enable I2C DMA TX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);   
  
  /* Disable and Deinitialize the DMA channels */
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, DISABLE);
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX, DISABLE);
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX);
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX);
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;  
    
  /*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_APB2PeriphClockCmd(sEE_I2C_SCL_GPIO_CLK | sEE_I2C_SDA_GPIO_CLK, ENABLE);

  /*!< sEE_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, ENABLE);
    
  /*!< GPIO configuration */  
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 

  /* Configure and enable I2C DMA TX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);  
  
  /*!< I2C DMA TX and RX channels configuration */
  /* Enable the DMA clock */
  RCC_AHBPeriphClockCmd(sEE_I2C_DMA_CLK, ENABLE);

  /* I2C TX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX);
  sEEDMA_InitStructure.DMA_PeripheralBaseAddr = (u32)sEE_I2C_DR_Address;
  sEEDMA_InitStructure.DMA_MemoryBaseAddr = (u32)0;   /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_BufferSize = 0xFFFF;            /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  sEEDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  sEEDMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  sEEDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  sEEDMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(sEE_I2C_DMA_CHANNEL_TX, &sEEDMA_InitStructure);  
  
  /* I2C RX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX);
  DMA_Init(sEE_I2C_DMA_CHANNEL_RX, &sEEDMA_InitStructure);  
  
  /* Enable the DMA Channels Interrupts */
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_RX, DMA_IT_TC, ENABLE);    
}


/**
  * @brief  Initializes DMA channel used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DMAConfig(u32 pBuffer, u32 BufferSize, u32 Direction)
{ 
  /* Initialize the DMA with the new parameters */
  if (Direction == sEE_DIRECTION_TX)
  {
    /* Configure the DMA Tx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure.DMA_MemoryBaseAddr = (u32)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    
    sEEDMA_InitStructure.DMA_BufferSize = (u32)BufferSize;  
    DMA_Init(sEE_I2C_DMA_CHANNEL_TX, &sEEDMA_InitStructure);  
  }
  else
  { 
    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure.DMA_MemoryBaseAddr = (u32)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    sEEDMA_InitStructure.DMA_BufferSize = (u32)BufferSize;      
    DMA_Init(sEE_I2C_DMA_CHANNEL_RX, &sEEDMA_InitStructure);    
  }
}


/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_DeInit(void)
{
  sEE_LowLevel_DeInit(); 
}


/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_Init(void)
{ 
  I2C_InitTypeDef  I2C_InitStructure;
  
  sEE_LowLevel_Init();
  
  /*!< I2C configuration */
  /* sEE_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = sEE_HW_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
  /* sEE_I2C Peripheral Enable */
  I2C_Cmd(sEE_I2C, ENABLE);
  /* Apply sEE_I2C configuration after enabling it */
  I2C_Init(sEE_I2C, &I2C_InitStructure);

  /* Enable the sEE_I2C peripheral DMA requests */
  I2C_DMACmd(sEE_I2C, ENABLE);
  
  /*!< Select the EEPROM address according to the state of E0, E1, E2 pins */
  sEEAddress = sEE_HW_ADDRESS;  

}






