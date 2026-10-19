#line 1 "/home/arduino/ArduinoApps/e4-bare-metal-stm32/sketch/systick.cpp"
#include <stm32u5xx.h>

#define SYSTICK_MS_LOAD 160000 - 1
#define SYSTICK_US_LOAD 160 - 1

#define CTRL_ENABLE (1U << 0)
#define CTRL_CLKSOURCE (1U << 2)
#define CTRL_COUNTFLAG (1U << 16)

void SysTickDelayMs(int delay) {
    SysTick->LOAD = SYSTICK_MS_LOAD;

    SysTick->VAL = 0;

    SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSOURCE;

    for (int i = 0; i < delay; i++) {
    	while ((SysTick->CTRL & CTRL_COUNTFLAG) == 0) {}
    }

    SysTick->CTRL = 0;
}

void SysTickDelayUs(int delay) {
    SysTick->LOAD = SYSTICK_US_LOAD;

    SysTick->VAL = 0;

    SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSOURCE;

    for (int i = 0; i < delay; i++) {
    	while ((SysTick->CTRL & CTRL_COUNTFLAG) == 0) {}
    }

    SysTick->CTRL = 0;
}
