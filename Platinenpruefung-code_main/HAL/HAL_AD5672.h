/*
 * HAL_AD5672.h
 *
 * Author: Andreas Fischer
 */

#ifndef _HAL_AD5672_H_
#define _HAL_AD5672_H_

typedef enum
{
    normal = 0,
    onekOhm = 1,
    tristate = 3
} powerMode;

void HAL_AD5672_INIT(void);
void AD5672writeDAC(bool device, uint8_t cmd, uint8_t ch, uint16_t mvoltage);
void AD5672SetAllValue(bool device);
void AD5672setPowerDown(bool device, uint8_t ch, powerMode mode);
void AD5672writePowerDown(bool device);

// maximum ratings
#define DAC_MIN 0
#define DAC_MAX 5000

// Channel adresses (4 Bits)
#define DAC0 0x0
#define DAC1 0x1
#define DAC2 0x2
#define DAC3 0x3
#define DAC4 0x4
#define DAC5 0x5
#define DAC6 0x6
#define DAC7 0x7

// Commands (4 Bits)
#define NO_OPERATION 0x0
#define WRITE_INPUT 0x1                         // write only to one specific DAC input register
#define UPDATE_DAC 0x2                          // update only one specific DAC channel
#define WRITE_AND_UPDATE_DAC 0x3                // writes and updates only the specified DAC-channel
#define POWER_UP_DOWN 0x4                       // set power mode individually: normal = 0,  onekOhm = 1, tristate = 3
#define HARDWARE_LDAC 0x5                       // possible to to select which channels respond to the LDAC pin
#define SOFTWARE_RESET 0x6                      // Reset to RstSel state therefore address = 0x0 & data = 0x1234
#define GAIN_REFERENCE 0x7                      // select internal or external Gain - internal ref is fine
#define SET_UP_DCEN 0x8                         // Daisy-chain enable
#define SET_UP_READBACK 0x9                     // Setup readback via SPI
#define TRANSPARENT_MODE 0xa                    // update alle channels at once, simular to UPDATE_DAC
#define UPDATE_INPUT_AND_DAC_SIMULTANEOUSLY 0xB // update simultaneously
// ... 0xC to 0xE Reserved
#define DAISY_CHAIN 0xF // no operation for dasy-chain-mode

#endif /* _HAL_AD5672_H_ */