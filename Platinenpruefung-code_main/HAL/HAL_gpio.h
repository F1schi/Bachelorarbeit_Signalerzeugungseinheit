/*
 * HAL_gpio.h
 *
 * Author: Andreas Fischer
 */

#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

void HAL_GPIO_INIT(void);
void GPIOrelayTest(void);

// SPI
#define SPI_MISO 1
#define SPI_SCK 3
#define SPI_MOSI 4

// AD9106
#define EN_CVDDX 2
#define SHDN_N_LT3472 4
#define TRIGGER 7
#define RESETB 8
#define SPI_CSB_ALT 9

#define SELECT_AD9106 digitalWrite(SPI_CSB_ALT, LOW);
#define UNSELECT_AD9106 digitalWrite(SPI_CSB_ALT, HIGH);

// MAX5487
#define CS0_MAX5487 10
#define CS1_MAX5487 12
#define CS2_MAX5487 13
#define CS3_MAX5487 23

// AD5672
#define CS0_AD5672 3
#define GAIN0 11
#define LDAC0 5
#define RESET0 6

#define CS1_AD5672 48
#define GAIN1 46
#define LDAC1 48
#define RESET1 47

#endif /* _HAL_GPIO_H_ */
