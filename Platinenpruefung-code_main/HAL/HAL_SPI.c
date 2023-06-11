/*
 * HAL_SPI.c
 *
 * Author: Andreas Fischer
 */

#include <SPI.h>
#include <Arduino.h>
#include "HAL_SPI.h"

/*
 * @brief initialization of the SPI interface
 * @param none
 * @return none
 */
void HAL_SPI_INIT(void)
{
  SPI.begin();
  SPISettings settings(SPI_SPEED, MSBFIRST, SPI_MODE3);
  SPI.beginTransaction(settings);
}

/*
SPI Mode:
  0:  AD5672 (DAC)
  3:  AD9106 (FreqG), MAX5487 (ePoti)
*/
