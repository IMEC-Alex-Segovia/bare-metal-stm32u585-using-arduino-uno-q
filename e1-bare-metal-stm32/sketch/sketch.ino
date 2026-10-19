/*
 * e1-bare-metal-stm32 - Arduino UNO Q
 * Sketch principal (corre en el MCU STM32)
 *
 * En este ejemplo, se enseña como configurar un pin
 * como salida y ponerlo en HIGH o LOW en el STM32
 */

#include "stm32u5xx.h"
#include "systick.h"

void delay_500ms() {
  for (volatile int i = 0; i < 500 * 16000U; i++) {__asm__ volatile("nop");}
}

void setup() {
  /*
   * Habilitar el reloj del puerto GPIO que se va a
   * utilizar. En este caso, se utiliza el pin D7 de
   * la placa, que corresponde al pin B2 del MCU.
   * Tambien el pin D6 que es el B1 del MCU.
   * Igualmente se usa el pn D5 que es el A11 del MCU.
   * El registro RCC_AHB2ENR1 sirve para habilitar
   * el reloj, y al poner el bit 1 y 0 de este registro
   * en HIGH, se habilitará el reloj del GPIOB y GPIOA 
   */ 

  RCC->AHB2ENR1 |= (3U);

  // se espera a que el reloj se estabilice
  for (volatile int i = 0; i < 255; i++) {__asm__ volatile("nop");}

  /*
   * Configurar el pin B2 y B1 del MCU como salida
   * digital con el registro GPIOB_MODER. Los bits
   * correspondientes al modo son [4:5] para B2 y
   * [2:3] para B1. Se escribe 01 para configurar 
   * como salida. El pin A11 también se configura
   * como salida
   */

  // mascara para limpiar la configuracion acutal
  GPIOB->MODER &= ~(0x3U << 4 | 0x3U << 2); 
  // configuración de B2 y B1 como salida
  GPIOB->MODER |= (0x1U << 4 | 0x1U << 2);

  // configurar el GPIOA
  GPIOA->MODER &= ~(0x3U << 22);
  GPIOA->MODER |= (0x1U << 22);

  /*
   * Poner en LOW como estado inicial el pin B2
   * usando el registro GPIOB_BSRR. El bit 2 pone
   * en HIGH el pin, mientras que el bit 18 lo
   * pone el LOW
   */

  GPIOB->BSRR |= (1U << 18 | 1U << 17);
  GPIOA->BSRR |= (1U << 27); 

}

void loop() {
  // Tu lógica aquí
  GPIOB->BSRR |= (1U << 2 | 1U << 17); 
  GPIOA->BSRR |= (1U << 27);

  // delay_500ms();
  SysTickDelayMs(1000);

  GPIOB->BSRR |= (1U << 18 | 1U << 1);
  GPIOA->BSRR |= (1U << 27);
    
  // delay_500ms();
  SysTickDelayMs(1000);

  GPIOA->BSRR |= (1U << 11);
  GPIOB->BSRR |= (1U << 17 | 1U << 18);

  // delay_500ms();
  SysTickDelayMs(1000);
}
