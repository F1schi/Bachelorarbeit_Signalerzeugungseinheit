/*
 * HAL_AD5672.c
 *
 * Author: Andreas Fischer
 */

#include <Arduino.h>
#include <SPI.h>
#include "HAL_AD5672.h"
#include "HAL_gpio.h"
#include "HAL_SPI.h"
#include "../AL/AL_boundary_check.h"
#include "HAL_modbus.h"

extern uint16_t holding_reg_values[36];

const float ADC12Bit = (float)5000.0 / (pow(2, 12) - 1); // V / LSB | 12 Bit

uint16_t powerModes[2] = {0}; // powerModes0 = 0, powerModes1 = 0;
extern const uint8_t CS_AD5672[2];
extern const uint8_t LDAC_AD5672[2];

/*
 * @brief initialization of the Digital to Analog Converter
 * @param none
 * @return none
 */
void HAL_AD5672_INIT(void)
{
    uint8_t cnt = 19;

    for (uint8_t i = 0; i < 8; i++)
    {
        AD5672setPowerDown(0, i, onekOhm); // Set power down mode, device 0
        AD5672setPowerDown(1, i, onekOhm); // Set power down mode, device 1

        AD5672writeDAC(0, WRITE_AND_UPDATE_DAC, i, 2500); // Set inital value = 2500 mV, device 0
        AD5672writeDAC(1, WRITE_AND_UPDATE_DAC, i, 0);    // Set inital value = 0 mV, device 1

        holding_reg_values[cnt++] = 2500;
        holding_reg_values[cnt + 8] = 0;
    }
    // values for Channel 0: needs to be resend - do not know why
    AD5672writeDAC(0, WRITE_AND_UPDATE_DAC, 0, 2500); // Set inital value = 2500 mV, device 0
    AD5672writeDAC(1, WRITE_AND_UPDATE_DAC, 0, 1000); // DAC_Ref
    holding_reg_values[27] = 1000;

    AD5672writePowerDown(0); // write PowerDowm mode to device 0
    AD5672writePowerDown(1); // write PowerDowm mode to device 1

    AD5672SetAllValue(0);
    AD5672SetAllValue(1);
}

/*
 * @brief SPI-write function of the DAC
 * @param device device number [0 - 1]
 * @param cmd Execution command
 * @param ch DAC channel number [0 - 7]
 * @param mvoltage register value
 * @return none
 */
void AD5672writeDAC(bool device, uint8_t cmd, uint8_t ch, uint16_t mvoltage)
{                                                                      // Structure: 4 Bit Command + 4 Bit Address + 16 Bit Data
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0)); // change SPI Mode

    uint8_t command = cmd << 4 | ch;
    uint16_t data = (uint16_t)((float)mvoltage / (float)ADC12Bit);

    digitalWrite(CS_AD5672[device], LOW);

    SPI.transfer(command);
    SPI.transfer16(data << 4);

    digitalWrite(CS_AD5672[device], HIGH);

    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3)); // return to default SPI Mode
}

/*
 * @brief set all DAC values at ones (from input- to output-register)
 * @param device device number
 * @return none
 */
void AD5672SetAllValue(bool device)
{
    digitalWrite(LDAC_AD5672[device], LOW);
    delayMicroseconds(10);
    digitalWrite(LDAC_AD5672[device], HIGH);
}

/*
 * @brief sets the specific power-mode of the DAC channel
 * @param device device number [0 - 1]
 * @param ch DAC channel number [0 - 7]
 * @param mode modes are defined in typdef "powerMode" (normal = 0, onekOhm = 1, tristate = 3)
 * @return none
 */
void AD5672setPowerDown(bool device, uint8_t ch, powerMode mode)
{
    static uint8_t modes[2][DAC7 + 1] = {0};
    modes[device][ch] = mode;

    for (uint8_t i = 0; i < 8; i++)
    {
        powerModes[device] |= (modes[device][i] << (i * 2));
    }
}

/*
 * @brief writes the specified power mode to the DAC
 * @param device device number
 */
void AD5672writePowerDown(bool device)
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0)); // change SPI Mode

    digitalWrite(CS_AD5672[device], LOW);

    SPI.transfer(POWER_UP_DOWN); // command

    SPI.transfer16(powerModes[device]);

    digitalWrite(CS_AD5672[device], HIGH);

    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3)); // return to default SPI Mode
}
