#ifndef _APP_H_
#define _APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"
#include "spi.h"
#include "adc.h"
#include "gpio.h"
#include "usart.h"

#include "Lcd.h"
#include "AMG883x.h"
#include "cmsis_os.h"

void RunApp(void);

#ifdef __cplusplus
}
#endif
#endif  //ifndef _APP_H_
