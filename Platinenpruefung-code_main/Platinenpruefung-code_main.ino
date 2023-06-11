/*
\  Author: Andreas Fischer
 | in cooperation with RSF Elektronik Ges.m.b.H
*/

#include "HAL/HAL_general.c"
#include "HAL/HAL_gpio.c"
#include "HAL/HAL_SPI.c"
#include "HAL/HAL_AD9106.c"
#include "HAL/HAL_serial.c"
#include "HAL/HAL_MAX5487.c"
#include "HAL/HAL_AD5672.c"
#include "HAL/HAL_modbus.c"
#include "HAL/HAL_modbus.h"

#include "DL/DL_AD9106.c"
#include "DL/DL_general.c"

#include "AL/AL_boundary_check.c"

extern ModbusRTUSlave modbus;

void setup()
{
  HAL_INIT();
  DL_INIT();
}

void loop()
{
  modbus.poll();
}
