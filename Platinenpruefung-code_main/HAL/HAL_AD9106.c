/*
 * HAL_AD9106.c
 *
 * Author: Andreas Fischer
 */

#include <Arduino.h>
#include "HAL_AD9106.h"
#include "HAL_gpio.h"

// default settings for AD9106
uint16_t AD9106_regval[66] = {0x0000, 0x0e0F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000e, 0x0000, 0x0000, 0x0000, 0x0000, 0x3232, 0x3232, 0x0111, 0xffff, 0x0101, 0x0101, 0x0003, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xfff0, 0x0000, 0x0000, 0x0000, 0xfff0, 0x0000, 0x0000, 0x0000, 0xfff0, 0x0000, 0x0000, 0x0000, 0xfff0, 0x0000, 0x0001, 0x0001};
// array which contains the register address
uint16_t reg_add[66] /* */ = {0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x001f, 0x0020, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x003e, 0x003f, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0047, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f, 0x001e, 0x001d};

/*
 * @brief reads a specific register on the AD9106
 * @param reg register address
 * @return value responds with the value of the  register
 */
uint16_t AD9106readRegister(uint8_t reg)
{
  SELECT_AD9106

  SPI.transfer16(READ + reg);
  uint16_t value = SPI.transfer16(DUMMY0);

  UNSELECT_AD9106

  return value;
}

/*
 * @brief writes a specific register on the AD9106
 * @param address register address
 * @param value value of the adress
 * @return none
 */
void AD9106writeRegister(uint8_t reg, uint16_t value)
{
  SELECT_AD9106

  SPI.transfer16(WRITE + reg);
  SPI.transfer16(value);

  UNSELECT_AD9106
}

/*
 * @brief reads and prints all registers of the AD9106 (only for debugging)
 * @param none
 * @return none
 */
void AD9106readAllRegisters(void)
{
  Serial.println("\n\n\tAdress |   Data");
  Serial.println("\t------------------");

  for (int i = 0; i < 66; i++)
  {
    Serial.print("\t0x");
    Serial.print(reg_add[i], HEX);
    Serial.print("    |   0x");
    Serial.println(AD9106readRegister(reg_add[i]), HEX);
  }
}

/*
 * @brief harware reset of the AD9106
 * @param none
 * @return none
 */
void AD9106resetRegister(void)
{
  digitalWrite(RESETB, LOW);
  delayMicroseconds(10);
  digitalWrite(RESETB, HIGH);
}

/*
 * @brief harware enables the output of the AD9106
 * @param none
 * @return none
 */
void AD9106enableOutput(void)
{
  digitalWrite(TRIGGER, LOW);
}

/*
 * @brief harware disables the output of the AD9106
 * @param none
 * @return none
 */
void AD9106disableOutput(void)
{
  digitalWrite(TRIGGER, HIGH);
}
