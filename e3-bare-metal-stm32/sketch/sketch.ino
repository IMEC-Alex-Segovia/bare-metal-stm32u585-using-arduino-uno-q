/*
 * e3-bare-metal-stm32 - Arduino UNO Q
 * Sketch principal (corre en el MCU STM32)
 * 
 * In this example, I show how to use a pin
 * as an input using the IDR register and
 * PUPDR register.
 *
 * This program toggle a led when the button
 * is pressed 
 */

#include "stm32u5xx.h"

void setup() {
  /*
   * Board pins D7 and D4 are using in this
   * example.
   * D7 is the B2 MCU pin (output)
   * D4 is the A12 MCU pin (input)
   */

  // enable the clock of GPIO ports
  RCC->AHB2ENR1 |= (0x3U);

  // wait clock 
  for (volatile int i = 0; i < 255; i++) {__asm__ volatile("nop");}

  // configure pins
  GPIOB->MODER &= ~(0x3U << 4);
  GPIOB->MODER |= (0x1U << 4);

  GPIOA->MODER &= ~(0x3U << 24);
  
  // disable pull up resistors in D4
  // in this example, a physical 10k pull-up
  // resistor is in the D4 pin 
  GPIOA->PUPDR &= ~(0x3U << 24);

  // initial state of D7 pin
  GPIOB->BSRR |= (1U << 2);
 
}

void loop() {
  // Tu l¢gica aqu¡
  // when pin is pressed, que D4 pin is
  // low h

  uint8_t last_state = (GPIOA->IDR & (1U << 12)) ? 1 : 0;
  uint8_t current_state = 0;

  while (1) {
    last_state = current_state;
    
    for (volatile int i = 0; i < 20 * 16000U; i++) {__asm__ volatile("nop");}

    current_state = (GPIOA->IDR & (1U << 12)) ? 1 : 0;
        
    if ((last_state == 1) && (current_state == 0)) {
      if ((GPIOB->ODR & (1U << 2))) {
        GPIOB->BSRR |= (1U << 18); 
      } else {
        GPIOB->BSRR |= (1U << 2); 
      }
    }
  }
}
                                                      
