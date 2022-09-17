#include "App.h"

#define DEFAULT_STK_SIZE		2048
void DefaultTask(void *pdata);
static void Delay(uint32_t Ms);
osThreadDef(DefaultTask, osPriorityNormal, 1, DEFAULT_STK_SIZE);

void RunApp(void)
{
		lcd_init();
    lcd_clear(BLACK);
    AMG883xInit();
    osKernelInitialize();
    osThreadCreate(osThread(DefaultTask), NULL);
    osKernelStart();
}

void DefaultTask(void *pdata)
{
    while(1)
    {
      AMG883xMatrixDisplay();
			Delay(10);
    }
}

static void Delay(uint32_t Ms)
{
    osDelay(Ms);
}
