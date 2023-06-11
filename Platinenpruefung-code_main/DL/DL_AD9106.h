/*
 * DL_AD9106.h
 *
 * Author: Andreas Fischer
 */

#ifndef _DL_AD9106_H_
#define _DL_AD9106_H_

void DL_AD9106_INIT(void);
void AD9106enableChannel(uint8_t ch);
void AD9106disableChannel(uint8_t ch);
void AD9106setAmplitude(uint8_t ch, int16_t amp);
void AD9106setFrequency(uint32_t freq);
void AD9106setPhase(uint8_t ch, uint16_t phase);
void AD9106analogGain(uint8_t ch, int16_t tune);
void AD9106setOffset(uint8_t ch, uint16_t offset);
void AD9106updateRegister(void);
void AD9106updatePatternRegister(void);
void AD9106printRegisters();
uint16_t AD9106readError(void);
void AD9106printError(uint16_t error);
void AD9106clearError(void);

#endif /* _DL_AD9106_H_ */
