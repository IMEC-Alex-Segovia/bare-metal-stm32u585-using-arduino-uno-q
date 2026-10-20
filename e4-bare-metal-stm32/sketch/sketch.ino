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
 * cat1: D9 -> GPIOB8
 * cat2: D10 -> GPIOB9
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

#define SHOW_TENS  (1U << 8 | 1U << 25)
#define SHOW_UNITS (1U << 24 | 1U << 9)

#define MAX_DELAY_COUNT 50 - 1

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

uint8_t count_units = 0;
uint8_t count_tens = 0;
uint8_t count_delay = MAX_DELAY_COUNT;

uint8_t btn_A_ls = 0;
uint8_t btn_B_ls = 0;
uint8_t btn_C_ls = 0;

uint8_t btn_A_cs = 0;
uint8_t btn_B_cs = 0;
uint8_t btn_C_cs = 0;

uint8_t pause = 0;

uint8_t count_up = 1;

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

  btn_A_ls = btn_A_cs;
  btn_B_ls = btn_B_cs;
  btn_C_ls = btn_C_cs;

  GPIOB->BSRR = (digits[count_units] | SHOW_UNITS);
  SysTickDelayMs(10);

  GPIOB->BSRR = (digits[count_tens] | SHOW_TENS);
  SysTickDelayMs(10);

  btn_A_cs = (GPIOB->IDR & (1U << 15)) ? 1 : 0;
  btn_B_cs = (GPIOB->IDR & (1U << 14)) ? 1 : 0;
  btn_C_cs = (GPIOB->IDR & (1U << 13)) ? 1 : 0;

  if ((btn_A_ls == 1) && (btn_A_cs == 0)) {
    pause ^= 1U;
    count_up = 1;  
  }
  if ((btn_B_ls == 1) && (btn_B_cs == 0)) {
    pause ^= 1U; 
    count_up = 0;
  }

  if (pause == 0) {
    
    if (count_delay == 0) {
      count_delay = MAX_DELAY_COUNT;
      if (count_up == 1) {  
        if (count_units == 9) {
          // count_units = 0;
          if (count_tens == 9) {
            // count_tens = 0;
            pause = 1; 
          } else {
            count_tens += 1;
            count_units = 0;
          } 
        } else {
          count_units += 1;
        }
      } else {
        if (count_units == 0) {
          // count_units = 9;
          if (count_tens == 0) {
            // count_tens = 0;
            pause = 1; 
          } else {
            count_tens -= 1;
            count_units = 9;
          } 
        } else {
          count_units -= 1;
        }
      }
    } else {
      count_delay -= 1;
    }

  } else {
    if ((btn_C_ls == 1) && (btn_C_cs == 0)) {
      count_units = (count_up == 1) ? 0 : 9;
      count_tens = (count_up == 1) ? 0 : 9;
    }
  }
}
