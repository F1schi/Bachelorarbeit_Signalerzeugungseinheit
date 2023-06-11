/*
 * DL_AD9106.c
 *
 * Author: Andreas Fischer
 */

#include <Arduino.h>
#include "DL_AD9106.h"
#include "..\HAL\HAL_modbus.h"
#include "..\AL\AL_boundary_check.h"

extern uint16_t holding_reg_values[36];

const float phase_factor = (float)360.0 / (pow(2, 16) - 1);
const float fDivisor = (float)156250000 / (float)16777216;

/*
 * @brief initial settings for AD9106
 * @param none
 * @return none
 */
void DL_AD9106_INIT(void)
{
  AD9106setFrequency(1000);
  holding_reg_values[1] = 1000;

  for (uint8_t i = 0; i < 4; i++)
  {
    AD9106setAmplitude(i, 1000);
    holding_reg_values[3 + i] = 1000;

    AD9106setPhase(i, 0);

    AD9106disableChannel(i);
  }
}

/*
 * @brief enables each channel independently via SPI
 * @param ch number of channel
 * @return none
 */
void AD9106enableChannel(uint8_t ch)
{
  AD9106_regval[1] &= ~(0x1 << (4 - ch));
}

/*
 * @brief disables each channel independently via SPI
 * @param ch number of channel
 * @return none
 */
void AD9106disableChannel(uint8_t ch)
{
  AD9106_regval[1] |= (0x1 << (4 - ch));
}

/*
 * @brief sets the amplitude via SPI
 * @param ch number of channel
 * @param amp set Amplitude [1 - 1300] mV
 * @return none
 */
void AD9106setAmplitude(uint8_t ch, int16_t amp)
{
  boundaryCheck16(amp, 1, 1300);

  AD9106_regval[37 - ch] = (float)amp * (float)12.46513;
}

/*
 * @brief sets the frequency for all channels via SPI
 * @param freq Frequency
 * @return none
 */
void AD9106setFrequency(uint32_t freq)
{
  uint32_t i32FrequencyTmp = freq / fDivisor;
  AD9106_regval[39] = (i32FrequencyTmp & 0xFFFF00) >> 8; // frequency high
  AD9106_regval[40] = (i32FrequencyTmp & 0x0000FF) << 8; // frequency low
}

/*
 * @brief sets the Phase via SPI
 * @param ch number of channel
 * @param phase set Phase [0 - 360]°
 * @return none
 */
void AD9106setPhase(uint8_t ch, uint16_t phase)
{
  float phaTmp = phase / phase_factor;
  AD9106_regval[45 - ch] = (uint16_t)phaTmp;
}

/*
 * @brief sets the analog gain via SPI
 * @param ch number of channel
 * @param tune Gain-value [-64 - 63]
 * @return none
 */
void AD9106analogGain(uint8_t ch, int16_t tune)
{
  AD9106_regval[8 - ch] = (uint16_t)(~(tune - 1)); // twos complement
}

/*
 * @brief sets the offset via SPI
 * @param ch number of channel
 * @param offset Offset [0 - 20] mV
 * @return none
 */
void AD9106setOffset(uint8_t ch, uint16_t offset)
{
  AD9106_regval[21 - ch] = map(offset, 0, 20, 0xFFF0, 0xC4E0);
}

/*
 * @brief updates all changed SPI registers on the AD9106
 * @param none
 * @return none
 */
void AD9106updateRegister(void)
{
  static uint16_t AD9106_regval_local[66];
  static bool init_finished = false; // first ime the function gets called -> transmits all register values

  for (int i = 0; i < 66; i++)
  {
    if (AD9106_regval_local[i] != AD9106_regval[i] || init_finished == false)
    {
      AD9106_regval_local[i] = AD9106_regval[i];
      AD9106writeRegister(reg_add[i], AD9106_regval[i]);
    }
  }
  AD9106updatePatternRegister();

  init_finished = true;
}

/*
 * @brief updates the SPI pattern register on the AD9106
 * @param none
 * @return none
 */
void AD9106updatePatternRegister(void)
{
  AD9106writeRegister(0x1D, 0x0001); // selfclearing
} // needs to be written directly on the register, because it wount be sent via AD9106updateRegister()

/*
 * @brief Prints register addresses and corresponding data in hexadecimal format (debugging only)
 * @param none
 * @return none
 */
void AD9106printRegisters()
{
  Serial.println("print register array");
  Serial.println("\n\n\tAdress |   Data");
  Serial.println("\t------------------");

  for (int i = 0; i < 66; i++)
  {
    if (i < 15)
    {
      Serial.print("\t0x");
      Serial.print(reg_add[i], HEX);
      Serial.print("    |   0x");
      Serial.println(AD9106_regval[i], HEX);
    }
    else
    {
      Serial.print("\t0x");
      Serial.print(reg_add[i], HEX);
      Serial.print("   |   0x");
      Serial.println(AD9106_regval[i], HEX);
    }
  }
  Serial.println("\n");
}

/*
 * @brief reads the error-register via SPI of the AD9106
 * @param none
 * @return error error code
 */
uint16_t AD9106readError(void)
{
  uint16_t error = AD9106readRegister(0x60);

  return error;
}

/*
 * @brief prints the error of the AD9106 (only debugging)
 * @param error Error code
 * @return none
 */
void AD9106printError(uint16_t error)
{
  if (error != 0)
  {
    Serial.println("\n\n****_____*****_____*****_____*****_____****_____*****_____*****_____*****_____*****_____*****_____*****\n");

    if ((error & 1) > 0 == true)
    {
      Serial.println("ERROR: Bit 0\tMEM_READ_ERR:\t\tmemory read conflict");
    }
    if ((error & 2) > 0 == true)
    {
      Serial.println("ERROR: Bit 1\tODD_ADDR_ERR:\t\tmemory pattern play is not even in length in trigger delay mode");
    }
    if ((error & 4) > 0 == true)
    {
      Serial.println("ERROR: Bit 2\tPERIOD_SHORT_ERR:\tperiod register setting value is smaller than pattern play cycle");
    }
    if ((error & 8) > 0 == true)
    {
      Serial.println("ERROR: Bit 3\tDOUT_START_SHORT_ERR:\tDOUT_START value is smaller than default value");
    }
    if ((error & 16) > 0 == true)
    {
      Serial.println("ERROR: Bit 4\tPAT_DLY_SHORT_ERR:\tpattern delay value is smaller than default value");
    }
    if ((error & 32) > 0 == true)
    {
      Serial.println("ERROR: Bit 5\tDOUT_START_LG_ERR:\tDOUT_START is larger than pattern delay");
    }

    // Serial.println(error, BIN);
    Serial.println("\n****_____*****_____*****_____*****_____****_____*****_____*****_____*****_____*****_____*****_____*****\n\n");
  }
  else
  {
    Serial.println("\nNo ERRORs pending");
  }
}

/*
 * @brief clearing the AD9106 error
 * @param none
 * @return none
 */
void AD9106clearError(void)
{
  AD9106writeRegister(0x60, 0x0000);
}