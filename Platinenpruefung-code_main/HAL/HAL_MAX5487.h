/*
 * HAL_MAX5487.h
 *
 * Author: Andreas Fischer
 */

#ifndef _HAL_MAX5487_H_
#define _HAL_MAX5487_H_

void MAX5487writeData(uint8_t device, uint8_t ch, uint8_t cmd, uint8_t data);
void MAX5487writeResistance(uint8_t device, uint8_t ch, uint16_t resistance);
void HAL_MAX5487_INIT(void);

// commands
#define WRITE_REGISTER 0x0
#define WITE_NV_REGISTER 0x1
#define RESGSTER_TO_NV_REGISTER 0x2
#define NV_REGISTER_TO_REGISTER 0x3

// resistant values
#define R_MIN 200   // typical for VDD = 5 V
#define R_MAX 10200 // END to END typical 10 kOhm -> 10k + 200

#endif /* _HAL_MAX5487_H_ */