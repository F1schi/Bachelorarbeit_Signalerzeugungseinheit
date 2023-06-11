/*
 * HAL_SPI.c
 *
 * Author: Andreas Fischer
 */

#include <Arduino.h>
#include "HAL_serial.h"

/*
 * @brief initialization of the Serial interface
 * @param none
 * @return none
 */
void HAL_SERIAL_INIT(void)
{
  Serial.begin(BAUDRATE);
}