/*
 * HAL_gpio.c
 *
 * Author: Andreas Fischer
 */

#include <Arduino.h>
#include "HAL_gpio.h"

const uint8_t GPIOs[48] = { // 99 = Dummy data
    99, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
    14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45};

const uint8_t CS_MAX5487[4] = {
    CS0_MAX5487, CS1_MAX5487, CS2_MAX5487, CS3_MAX5487};

const uint8_t CS_AD5672[2] = {CS0_AD5672, CS1_AD5672};
const uint8_t LDAC_AD5672[2] = {LDAC0, LDAC1};

/*
 * @brief initialisation of the inputs and outputs of the Arduino MEGA 2560
 * @param none
 * @return none
 */
void HAL_GPIO_INIT(void)
{
  // SPI
  pinMode(SPI_MISO, INPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_SCK, OUTPUT);

  // AD9106
  pinMode(EN_CVDDX, OUTPUT);
  pinMode(SHDN_N_LT3472, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(RESETB, OUTPUT);
  pinMode(SPI_CSB_ALT, OUTPUT);

  digitalWrite(EN_CVDDX, HIGH);      // on-board oscillator is chosen (150 MHz)
  digitalWrite(SHDN_N_LT3472, HIGH); // on-board amplifiers powered
  digitalWrite(TRIGGER, HIGH);       // LOW = output enabled; HIGH = output disabled
  digitalWrite(RESETB, HIGH);        // pull low for 10 us to reset registers
  digitalWrite(SPI_CSB_ALT, HIGH);   // CS (active low)

  // MAX5487
  pinMode(CS0_MAX5487, OUTPUT);
  pinMode(CS1_MAX5487, OUTPUT);
  pinMode(CS2_MAX5487, OUTPUT);
  pinMode(CS3_MAX5487, OUTPUT);

  digitalWrite(CS0_MAX5487, HIGH); // CS (active low)
  digitalWrite(CS1_MAX5487, HIGH); // CS (active low)
  digitalWrite(CS2_MAX5487, HIGH); // CS (active low)
  digitalWrite(CS3_MAX5487, HIGH); // CS (active low)

  // AD5672
  pinMode(CS0_AD5672, OUTPUT);
  pinMode(GAIN0, OUTPUT);
  pinMode(LDAC0, OUTPUT);
  pinMode(RESET0, OUTPUT);

  pinMode(CS1_AD5672, OUTPUT);
  pinMode(GAIN1, OUTPUT);
  pinMode(LDAC1, OUTPUT);
  pinMode(RESET1, OUTPUT);

  digitalWrite(CS0_AD5672, HIGH); // CS (active low)
  digitalWrite(GAIN0, HIGH);      // GAIN = x2
  digitalWrite(LDAC0, HIGH);      // active low; LOW = DAC values are pushed in DAC reg from innput reg
  digitalWrite(RESET0, HIGH);     // active low; reset on falling edge to 0V (depends on RSTSEL = LOW)

  digitalWrite(CS1_AD5672, HIGH); // CS (active low)
  digitalWrite(GAIN1, HIGH);      // GAIN = x2
  digitalWrite(LDAC1, HIGH);      // active low; LOW = DAC values are pushed in DAC reg from innput reg
  digitalWrite(RESET1, HIGH);     // active low; reset on falling edge to 0V (depends on RSTSEL = LOW)

  // GPIOs
  for (int i = 0; i <= (sizeof(GPIOs) / sizeof(GPIOs[0])); i++)
  {
    pinMode(GPIOs[i], OUTPUT);   // set all GPIOs as Output
    digitalWrite(GPIOs[i], LOW); // Pull every pin to LOW
  }
}

/*
 * @brief turn on and off again each relay at the program start
 * @param none
 * @return none
 */
void GPIOrelayTest(void)
{
  uint8_t relays[10] = {29, 31, 45, 32, 43, 33, 44, 34, 30, 41};

  for (int i = 0; i < 10; i++)
  {
    digitalWrite(relays[i], HIGH);
    delay(100);
  }

  delay(300);

  for (int i = 0; i < 10; i++)
  {
    digitalWrite(relays[i], LOW);
    delay(50);
  }

  for (int i = 0; i < 10; i++)
  {
    digitalWrite(relays[i], HIGH);
    delay(30);
  }

  delay(300);

  for (int i = 0; i < 10; i++)
  {
    digitalWrite(relays[i], LOW);
    delay(7);
  }
}