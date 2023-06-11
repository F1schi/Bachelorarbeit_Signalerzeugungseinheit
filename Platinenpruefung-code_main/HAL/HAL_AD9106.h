/*
 * HAL_AD9106.h
 *
 * Author: Andreas Fischer
 */

#ifndef _HAL_AD9106_H_
#define _HAL_AD9106_H_

#define READ 0x8000
#define WRITE 0x0000
#define DUMMY0 0x0000

uint16_t AD9106readRegister(uint8_t reg);
void AD9106writeRegister(uint8_t reg, uint16_t value);
void AD9106readAllRegisters(void);
void AD9106resetRegister(void);
void AD9106enableOutput(void);
void AD9106disableOutput(void);

#endif /* _HAL_AD9106_H_ */
