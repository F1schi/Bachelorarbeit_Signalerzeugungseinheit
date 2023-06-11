/*
 * HAL_modbus.h
 *
 * Author: Andreas Fischer
 */

#ifndef _HAL_MODBUS_H_
#define _HAL_MODBUS_H_

#define NUM_OF_COILS 149
#define NUM_OF_INPUT_REGISTERS 16
#define NUM_OF_HOLDING_REGISTERS 105

#define BUFFER_SIZE 256
#define CLIENT_ID 1

void HAL_MODBUS_INIT(void);
char coilRead(unsigned int address);
boolean coilWrite(unsigned int address, boolean value);
long inputRegisterRead(word address);
long holdingRegisterRead(unsigned int address);
boolean holdingRegisterWrite(word address, word value);
void sendResponse(uint8_t *response, uint8_t cnt);
int getPinMode(uint8_t pin);

// Modbus Fuktion-Codes
#define MODBUS_READ_COIL 0x01
#define MODBUS_READ_HOLDING 0x03
#define MODBUS_READ_INPUT 0x04
#define MODBUS_WRITE_COIL 0x05
#define MODBUS_WRITE_HOLDING 0x06

#endif /* _HAL_MODBUS_H_ */
