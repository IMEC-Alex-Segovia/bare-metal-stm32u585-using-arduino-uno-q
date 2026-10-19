#include <Arduino.h>
#line 1 "/home/arduino/ArduinoApps/e4-bare-metal-stm32/sketch/sketch.ino"
/*
 * e4-bare-metal-stm32 - Arduino UNO Q
 * Sketch principal (corre en el MCU STM32)
 *
 * In this example, a stopwatch is implemented
 * using a 7 segment display and 3 buttons
 * the button A switch between count up and pause
 * the button B switch between count down and pause
 * the button C restart the count to 0 if count up 
 * was selected, else restart the count to 99 if
 * cout down was selected. If counter reach the 
 * limit, automaticaly stop and wait for the 
 * button for the contra count mode
 *
 * pins used for 7 seg display:
 * a: D3 -> GPIOB0
 * b: D6 -> GPIOB1
 * c: D7 -> GPIOB2
 * d: D2 -> GPIOB3
 * e: D8 -> GPIOB4
 * f: D1 -> GPIOB6
 * g: D0 -> GPIOB7
 *
 * an1: D9 -> GPIOB8
 * an2: D10 -> GPIOB9
 *
 * pins used for buttons
 *
 * A: D11 -> GPIOB15
 * B: D12 -> GPIOB14
 * C: D13 -> GPIOB13
 */



#include "stm32u5xx.h"
#include "systick.h"

#define SEG_A_ON  (1U << 0)
#define SEG_A_OFF (1U << 16)

#define SEG_B_ON  (1U << 1)
#define SEG_B_OFF (1U << 17)

#define SEG_C_ON  (1U << 2)
#define SEG_C_OFF (1U << 18)

#define SEG_D_ON  (1U << 3)
#define SEG_D_OFF (1U << 19)

#define SEG_E_ON  (1U << 4)
#define SEG_E_OFF (1U << 20)

#define SEG_F_ON  (1U << 6)
#define SEG_F_OFF (1U << 22)

#define SEG_G_ON  (1U << 7)
#define SEG_G_OFF (1U << 23)

static const uint32_t digits[] = {
  // a, b, c, d, e, f, g
  (SEG_A_ON  | SEG_B_ON  | SEG_C_ON  | SEG_D_ON  | SEG_E_ON  | SEG_F_ON  | SEG_G_OFF), // 0
  (SEG_A_OFF | SEG_B_ON  | SEG_C_ON  | SEG_D_OFF | SEG_E_OFF | SEG_F_OFF | SEG_G_OFF), // 1
  (SEG_A_ON  | SEG_B_ON  | SEG_C_OFF | SEG_D_ON  | SEG_E_ON  | SEG_F_OFF | SEG_G_ON ), // 2
  (SEG_A_ON  | SEG_B_ON  | SEG_C_ON  | SEG_D_ON  | SEG_E_OFF | SEG_F_OFF | SEG_G_ON ), // 3
  (SEG_A_OFF | SEG_B_ON  | SEG_C_ON  | SEG_D_OFF | SEG_E_OFF | SEG_F_ON  | SEG_G_ON ), // 4
  (SEG_A_ON  | SEG_B_OFF | SEG_C_ON  | SEG_D_ON  | SEG_E_OFF | SEG_F_ON  | SEG_G_ON ), // 5
  (SEG_A_ON  | SEG_B_OFF | SEG_C_ON  | SEG_D_ON  | SEG_E_ON  | SEG_F_ON  | SEG_G_ON ), // 6
  (SEG_A_ON  | SEG_B_ON  | SEG_C_ON  | SEG_D_OFF | SEG_E_OFF | SEG_F_OFF | SEG_G_OFF), // 7
  (SEG_A_ON  | SEG_B_ON  | SEG_C_ON  | SEG_D_ON  | SEG_E_ON  | SEG_F_ON  | SEG_G_ON ), // 8
  (SEG_A_ON  | SEG_B_ON  | SEG_C_ON  | SEG_D_ON  | SEG_E_OFF | SEG_F_ON  | SEG_G_ON ), // 9
};

#line 74 "/home/arduino/ArduinoApps/e4-bare-metal-stm32/sketch/sketch.ino"
void setup();
#line 88 "/home/arduino/ArduinoApps/e4-bare-metal-stm32/sketch/sketch.ino"
void loop();
#line 74 "/home/arduino/ArduinoApps/e4-bare-metal-stm32/sketch/sketch.ino"
void setup() {
  RCC->AHB2ENR1 |= (1U << 1);
  
  SysTickDelayUs(10);

  GPIOB->MODER &= ~((0x3U << 0*2) | (0x3U << 1*2) | (0x3U << 2*2) | (0x3U << 3*2)  | (0x3U << 4*2)  | (0x3U << 6*2) | 
                    (0x3U << 7*2) | (0x3U << 8*2) | (0x3U << 9*2) | (0x3U << 15*2) | (0x3U << 14*2) | (0x3U << 13*2)); 
 
  GPIOB->MODER |= ((0x1U << 0*2) | (0x1U << 1*2) | (0x1U << 2*2) | (0x1U << 3*2) | (0x1U << 4*2) | 
                   (0x1U << 6*2) | (0x1U << 7*2) | (0x1U << 8*2) | (0x1U << 9*2));

  GPIOB->PUPDR &= ~((0x3U << 15*2) | (0x3U << 14*2) | (0x3U << 13*2));  
}

void loop() {
  // Tu lógica aquí

  for (int i = 0; i < 10; i++) {
    GPIOB->BSRR = digits[i];
    SysTickDelayMs(1000);
  }
}

