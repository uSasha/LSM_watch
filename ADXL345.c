#include "ADXL345.h" 
#include "em_usart.h"
#include "em_gpio.h"
#include "activity_app.h"

char values[10];



/******************************************************************************
 * @brief write accel register of defined addres with defined value
 *
 * 
 ******************************************************************************/
void writeRegister(char registerAddress, char value)
{
  GPIO_PinOutClear(ACCEL_CS_PORT, ACCEL_CS_PIN);

  USART_Tx( ACCEL_USART, registerAddress);
  USART_Tx( ACCEL_USART, value);

  while(!(USART_StatusGet( ACCEL_USART ) & USART_STATUS_TXC));       // wait for end of transmission

  GPIO_PinOutSet(ACCEL_CS_PORT, ACCEL_CS_PIN);
}


/******************************************************************************
 * @brief read accel register of defined addres with defined number of bytes
 * store it in values buffer
 * 
 ******************************************************************************/
void readRegister(char registerAddress, int numBytes, char * values)
{
  char address = 0x80 | registerAddress;
  if(numBytes > 1)address = address | 0x40;
  
  GPIO_PinOutClear(ACCEL_CS_PORT, ACCEL_CS_PIN);

  ACCEL_USART->CMD |= USART_CMD_RXBLOCKEN;  
  USART_Tx( ACCEL_USART, address );
  ACCEL_USART->CMD |= USART_CMD_RXBLOCKDIS;
  
  for( int i=0; i<numBytes; i++ )    // there is an issue somewhere if you try to read more then 1 byte
  {
    USART_Tx( ACCEL_USART, DUMMY );
    while (!(ACCEL_USART->STATUS & USART_STATUS_RXDATAV));
    values[i] = USART_Rx( ACCEL_USART );
  }

  while(!(USART_StatusGet( ACCEL_USART ) & USART_STATUS_TXC));       // wait for end of transmission

  ACCEL_USART->CMD |= USART_CMD_RXBLOCKEN;
  GPIO_PinOutSet(ACCEL_CS_PORT, ACCEL_CS_PIN);
}
