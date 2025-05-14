#ifndef SLEEP_MODE_H_
#define SLEEP_MODE_H_
#include "timer.h"
#include <stm32g031xx.h>

void GPIO_SleepStatusInit(void);
void RTC_Init(void);
void enter_low_power_standby_mode(uint32_t sleepTime);

#endif
