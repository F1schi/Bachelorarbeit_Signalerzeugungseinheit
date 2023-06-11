/*
 * HAL_modbus.c
 *
 * Author: Andreas Fischer
 */

#include <Arduino.h>
#include <ModbusRTUSlave.h>
#include <util/crc16.h>
#include "HAL_modbus.h"
#include "..\DL\DL_AD9106.h"
#include "HAL_AD5672.h"
#include "HAL_MAX5487.h"

extern uint16_t powerModes[2];
extern uint16_t AD9106_regval[66];

bool Coils[NUM_OF_COILS] = {0};
byte buffer[BUFFER_SIZE] = {0};
uint8_t pinOrder[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
                      14, 15, 16, 17, 18, 19, 20, 21, 22, 25, 24, 27, 26, 29, 28, 31,
                      30, 33, 32, 35, 34, 37, 36, 39, 38, 41, 40, 43, 42, 45, 44};
uint16_t min_reg_values[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 360, 360, 360, 360, 360,
                             360, 360, 360, 360, 360, 360, 360, 360, 0};
uint16_t max_reg_values[] = {32783, 65535, 65635, 1300, 1300, 1300, 1300, 65535, 65535, 65535, 65535,
                             10360, 10360, 10360, 10360, 10360, 10360, 10360, 10360, 5000, 5000, 5000,
                             5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
                             5000, 65535, 1};
uint16_t holding_reg_values[36] = {0};

ModbusRTUSlave modbus(Serial, buffer, BUFFER_SIZE);

void (*resetFunc)(void) = 0; // pointer to address 0

/*
 * @brief initialization of the modbus interface
 * @param none
 * @return none
 */
void HAL_MODBUS_INIT(void)
{
    modbus.begin(CLIENT_ID, 9600);
    modbus.configureCoils(NUM_OF_COILS, coilRead, coilWrite);
    modbus.configureInputRegisters(NUM_OF_INPUT_REGISTERS, inputRegisterRead);
    modbus.configureHoldingRegisters(NUM_OF_HOLDING_REGISTERS, holdingRegisterRead, holdingRegisterWrite);
}

/*
 * @brief function which will be called if a client wants to read a coil
 * @param address address of the coil
 * @return the value of the coil
 */
char coilRead(unsigned int address)
{
    uint8_t tmp = 0;

    if (address >= 0 && address <= NUM_OF_COILS)
    {
        if (address < 50) // pinMode is also needed for further output calculation
        {
            // function returns: 0=OUTPUT; 1=INPUT; 2=INPUT_PULLUP
            tmp = getPinMode(pinOrder[address - 1]);
        }

        if (address > 50 && address < 100)
        {
            tmp = getPinMode(pinOrder[address - 50 - 1]);
            if (tmp == true) // if PinMode == INPUT
            {
                tmp = digitalRead(pinOrder[address - 50 - 1]);
            }
        }
        else if (address > 100)
        {
            tmp--; // reassigns: -1=OUTPUT; 0=INPUT; 1=INPUT_PULLUP
        }

        byte response[6] = {
            CLIENT_ID,        // Modbus client ID
            MODBUS_READ_COIL, // Modbus function code
            0x01,             // Number of data bytes
            tmp};             // value of the Coil

        sendResponse(response, 6); // perform CR-Check and sends the data

        return tmp;
    }
    else
    {
        return -1;
    }
}

/*
 * @brief function which will be called if a Client wants to write a coil
 * @param address address of the coil
 * @para value boolean value for the specific coil
 * @return true = everything okey; false = something went wrong
 */
boolean coilWrite(unsigned int address, boolean value)
{
    if (address >= 0 && address <= NUM_OF_COILS) // address boundary check
    {
        if (address > 0 && address < 50) // change pinMode
        {
            pinMode(pinOrder[address - 1], (value ? INPUT : OUTPUT));
        }
        else if (address > 50 && address < 100)
        {
            if (getPinMode(address - 50 - 1) == 0) // wire the digital value if the pin == OUTPUT
            {
                digitalWrite(pinOrder[address - 50 - 1], value == 1 ? HIGH : LOW);
            }
        }
        else if (address > 100 && address < 150) // enable / disable PULLUP resistor
        {
            if (getPinMode(address - 100 - 1) > 0)
            {
                pinMode(pinOrder[address - 100 - 1], (value ? INPUT : INPUT_PULLUP));
            }
        }
        // special case for address {0, 50, 100} not implemented

        byte response[8] = {
            CLIENT_ID,         // Modbus client ID
            MODBUS_WRITE_COIL, // Modbus function code
            value,             // value of the coil
            address,           // address of the coil
            0,                 // don't care byte
        };

        sendResponse(response, 8); // perform CR-Check and sends the data

        return true;
    }
    else
    {
        return false;
    }
}

/*
 * @brief function which will be called if a Client wants to read a input register
 * @param address address of the input register
 * @return value of the register
 */
long inputRegisterRead(word address)
{
    if (address >= 0 && address <= NUM_OF_COILS) // address boundary check
    {
        uint16_t value = analogRead(pinOrder[address - 1]);

        byte response[7] = {
            CLIENT_ID,         // Modbus client ID
            MODBUS_READ_INPUT, // Modbus function code
            0x02,              // Number of data bytes
            highByte(value),   // High byte of the analog value
            lowByte(value)};   // Low byte of the analog value

        sendResponse(response, 7); // perform CR-Check and sends the data

        return value;
    }
    else
    {
        return -1;
    }
}

/*
 * @brief function which will be called if a Client wants to read a holding register
 * @param address address of the coil
 * @return value of the register
 */
long holdingRegisterRead(unsigned int address)
{
    if (address >= 0 && address <= NUM_OF_HOLDING_REGISTERS) // address boundary check
    {
        uint16_t tmp_val = 0;

        if (address == 0) // Vorrichtungscodierung, can't be read by python GUI
        {
            tmp_val |= digitalRead(23); // * 1
            tmp_val |= (digitalRead(24) << 1);
            tmp_val |= (digitalRead(25) << 2);
            tmp_val |= (digitalRead(26) << 3);
        }
        if (address > 0 && address < 38)
        {
            tmp_val = holding_reg_values[address - 1];
        }
        else if (address == 39)
        {
            tmp_val = AD9106readError();
        }
        else if (address > 39 && address <= NUM_OF_HOLDING_REGISTERS)
        {
            tmp_val = AD9106_regval[address - 40];
        }

        byte response[7] = {
            CLIENT_ID,           // Modbus client ID
            MODBUS_READ_HOLDING, // Modbus function code
            0x02,                // Number of data bytes
            highByte(tmp_val),   // High byte of the analog value
            lowByte(tmp_val)};   // Low byte of the analog value

        sendResponse(response, 7); // perform CR-Check and sends the data

        return tmp_val;
    }
    else
    {
        return -1;
    }
}

/*
 * @brief function which will be called if a Client wants to write a holding register
 * @param address address of the coil
 * @return true = everything okey; false = something went wrong
 */
boolean holdingRegisterWrite(word address, word value)
{
    // address 0 = "Vorrichtungscodierung" read only!
    if (address > 0 && address <= NUM_OF_HOLDING_REGISTERS) // valid register address
    {
        if (address < 37)
        {
            holding_reg_values[address - 1] = value;
        }

        if (address == 1) // Reset system & Channel on/off
        {
            uint16_t tmp_value = uint16_t(value);

            for (uint8_t i = 0; i < 4; i++) // enable/disable channels
            {
                if ((tmp_value & 0x0001) == 1)
                {
                    AD9106enableChannel(i + 1);
                }
                else
                {
                    AD9106disableChannel(i + 1);
                }

                tmp_value = tmp_value >> 1;
            }

            if ((tmp_value & 0x0001) == 1) // apply new AD9106 register values
            {
                AD9106updateRegister();
                AD9106enableOutput();
            }

            tmp_value = tmp_value >> 11;

            if ((tmp_value & 0x0001) == true) // Software reset
            {
                resetFunc();
            }
        }
        else if (address == 2 || address == 3) // set frequency
        {
            static uint16_t freq[2] = {0};
            freq[address - 2] = value;

            uint32_t freq_tmp = ((uint32_t)freq[1] << 16) | freq[0];

            AD9106setFrequency(freq_tmp);
        }
        else if (address > 3 && address < 8) // set Amplitude
        {
            value = boundaryCheck16(value, 0, 1300);
            AD9106setAmplitude(address - 4 + 1, value);
        }
        else if (address > 7 && address < 12) // set phase
        {
            value = boundaryCheck16(value, 0, 360);
            AD9106setPhase(address - 8 + 1, value);
        }
        else if (address > 11 && address < 20) // set poti value
        {
            value = boundaryCheck16(value, R_MIN, R_MAX);
            if (address == 12 || address == 13)
            {
                MAX5487writeResistance(0, address - 12, value);
            }
            else if (address == 14 || address == 15)
            {
                MAX5487writeResistance(1, address - 14, value);
            }
            else if (address == 16 || address == 17)
            {
                MAX5487writeResistance(2, address - 16, value);
            }
            else if (address == 18 || address == 19)
            {
                MAX5487writeResistance(3, address - 18, value);
            }
        }
        else if (address > 19 && address < 28) // set DAC offset
        {
            value = boundaryCheck16(value, 0, 5000);
            AD5672writeDAC(0, WRITE_AND_UPDATE_DAC, address - 20, value);
        }
        else if (address > 27 && address < 36) // set DAC-Ref + reserved
        {
            if (address == 28)
            {
                value = boundaryCheck16(value, 1000, 5000); // DAC-Ref
            }
            else
            {
                value = boundaryCheck16(value, 0, 5000);
            }

            AD5672writeDAC(1, WRITE_AND_UPDATE_DAC, address - 12, value);
        }
        else if (address > 35 && address < 38) // DAC PowerDown modi
        {
            powerModes[address - 36] = value;
            AD5672writePowerDown(address - 36);
        }
        else if (address == 38) // reset AD9106
        {
            if (value == 1)
            {
                AD9106resetRegister();
            }
        }
        else if (address == 39) // clear AD9106 error
        {
            AD9106clearError();
        }
        // read AD9106 register values
        else if (address > 39 && address <= NUM_OF_HOLDING_REGISTERS)
        {
            AD9106_regval[address - 40] = value;
        }

        return true;
    }
    else
    {
        return false;
    }
}

/*
 * @brief function which will calculate the CR-Checksum and sends the response to the client
 * @param response array which consists of the data for the client
 * @param cnt number of array elements
 * @return none
 */
void sendResponse(uint8_t *response, uint8_t cnt)
{
    uint16_t crc = 0xFFFF;

    for (uint8_t i = 0; i < cnt - 2; i++) // calculate CRC
    {
        crc = _crc16_update(crc, response[i]);
    }

    response[cnt - 2] = lowByte(crc); // split CRC in HIGH- and LOW-Byte
    response[cnt - 1] = highByte(crc);

    for (uint8_t i = 0; i < cnt; i++) // Send acknowledgement message
    {
        Serial.write(response[i]);
    }
}

/*
 * @brief function which returns the current pinMode
 * @param pin pin number
 * @return current pinMode (0=OUTPUT; 1=INPUT; 2=INPUT_PULLUP)
 */
int getPinMode(uint8_t pin)
{
    if (pin >= NUM_DIGITAL_PINS) // check if pin exists
    {
        return -1;
    }

    uint8_t mask = digitalPinToBitMask(pin); // get bit mask of the pin
    uint8_t port = digitalPinToPort(pin);    // get the port of the pin

    return ((*portModeRegister(port) & mask) ? false : ((*portOutputRegister(port) & mask) ? INPUT_PULLUP : true));
}
