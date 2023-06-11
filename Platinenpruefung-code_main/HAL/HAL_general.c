/*
 * HAL_general.c
 *
 * Author: Andreas Fischer
 */

#include "HAL_general.h"
#include "HAL_gpio.h"
#include "HAL_SPI.h"
#include "HAL_serial.h"
#include "HAL_modbus.h"
#include "HAL_AD5672.h"
#include "HAL_MAX5487.h"

/*
 * @brief initialization of the harware
 * @param none
 * @return none
 */
void HAL_INIT(void)
{
  HAL_GPIO_INIT();
  HAL_SPI_INIT();
  HAL_SERIAL_INIT();
  HAL_MODBUS_INIT();

  HAL_MAX5487_INIT();
  HAL_AD5672_INIT();

  GPIOrelayTest();
}
