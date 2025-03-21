#ifndef SLEEP_MODE_H_
#define SLEEP_MODE_H_
#include "timer.h"
#include <stm32g031xx.h>

void RTC_Init();
void enter_low_power_standby_mode(void);

#endif
