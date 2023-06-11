/*
 * HAL_MAX5487.c
 *
 * Author: Andreas Fischer
 */
#include <Arduino.h>
#include "HAL_MAX5487.h"
#include "HAL_gpio.h"
#include "HAL_modbus.h"

extern const uint8_t CS_MAX5487[4];
extern uint16_t holding_reg_values[36];

const float RperWiper = (float)10000.0 / (float)R_MIN;

/*
 * @brief initialization of the digital potentiometer
 * @param none
 * @return none
 */
void HAL_MAX5487_INIT(void)
{
    uint8_t cnt = 11;
    for (int i = 0; i <= sizeof(CS_MAX5487); i++)
    {
        holding_reg_values[cnt++] = 1000;
        holding_reg_values[cnt++] = 1000;
        MAX5487writeResistance(i, 0, 1000); // inital value -> Gain = 1
        MAX5487writeResistance(i, 1, 1000); // inital value -> Gain = 1
    }
}

/*
 * @brief write resistance value of the digital poentiometer
 * @param device device number [0 - 3]
 * @param ch Number of poti channel [0 - 1]
 * @param resistance value of the resistance [0 - 10k]
 * @return none
 */
void MAX5487writeResistance(uint8_t device, uint8_t ch, uint16_t resistance)
{
    uint8_t data = (uint8_t)round(((float)(resistance - R_MIN) / (float)RperWiper));

    MAX5487writeData(device, ch, WRITE_REGISTER, data);
}

/*
 * @brief SPI-write function of the digital poentiometer
 * @param device device number [0 - 3]
 * @param ch Number of poti channel [0 - 1]
 * @param cmd Execution command
 * @param data register value
 * @return none
 */
void MAX5487writeData(uint8_t device, uint8_t ch, uint8_t cmd, uint8_t data)
{
    digitalWrite(CS_MAX5487[device], LOW);

    SPI.transfer((cmd << 4) | (ch + 1));
    SPI.transfer(data);

    digitalWrite(CS_MAX5487[device], HIGH);
}