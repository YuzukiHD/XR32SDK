/*
 * Copyright (C) 2017 XRADIO TECHNOLOGY CO., LTD. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the
 *       distribution.
 *    3. Neither the name of XRADIO TECHNOLOGY CO., LTD. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>

#include "kernel/os/os.h"
#include "driver/chip/hal_wakeup.h"
#include "pm/pm.h"

#include "common/framework/platform_init.h"


#define TEST_SLEEP
#define TEST_STANDBY
#define TEST_POWEROFF


#define BUTTON_WAKEUP_PORT_DEF GPIO_PORT_A
#define BUTTON_WAKEUP_PIN_DEF  GPIO_PIN_6

#define WAKEUP_IO_PIN_DEF  2    /* PA6 */
#define WAKEUP_IO_MODE_DEF  0
#define WAKEUP_IO_PULL_DEF  1

static void button_irq_cb(void *arg)
{
	printf("button event!\r\n");
}

static void button_init(void)
{
	GPIO_InitParam param;
	GPIO_IrqParam Irq_param;

	param.driving = GPIO_DRIVING_LEVEL_1;
	param.pull = GPIO_PULL_UP;
	param.mode = GPIOx_Pn_F6_EINT;
	HAL_GPIO_Init(BUTTON_WAKEUP_PORT_DEF, BUTTON_WAKEUP_PIN_DEF, &param);

	Irq_param.event = GPIO_IRQ_EVT_FALLING_EDGE;
	Irq_param.callback = button_irq_cb;
	Irq_param.arg = (void *)NULL;
	HAL_GPIO_EnableIRQ(BUTTON_WAKEUP_PORT_DEF, BUTTON_WAKEUP_PIN_DEF, &Irq_param);
}

static void button_deinit(void)
{
	HAL_GPIO_DisableIRQ(BUTTON_WAKEUP_PORT_DEF, BUTTON_WAKEUP_PIN_DEF);
	HAL_GPIO_DeInit(BUTTON_WAKEUP_PORT_DEF, BUTTON_WAKEUP_PIN_DEF);
}

int main(void)
{
	platform_init();

	button_init();

	OS_MSleep(2000);

#ifdef TEST_SLEEP
	printf("System will go to Sleep and wakeup after 10S.\n"
	       " Prepare to test power consumption now.\n");
	HAL_Wakeup_SetTimer_mS(10000);
	//HAL_Wakeup_SetIO(WAKEUP_IO_PIN_DEF, WAKEUP_IO_MODE_DEF, WAKEUP_IO_PULL_DEF);
	pm_enter_mode(PM_MODE_SLEEP);
	printf("wakeup event:%d\n", HAL_Wakeup_GetEvent());
#endif

#ifdef TEST_STANDBY
	OS_MSleep(2000);
	printf("System will go to Standby and wakeup after 10S.\n"
	       " Prepare to test power consumption now.\n");
	HAL_Wakeup_SetTimer_mS(10000);
	//HAL_Wakeup_SetIO(WAKEUP_IO_PIN_DEF, WAKEUP_IO_MODE_DEF, WAKEUP_IO_PULL_DEF);
	pm_enter_mode(PM_MODE_STANDBY);
	printf("wakeup event:%d\n", HAL_Wakeup_GetEvent());
#endif

#ifdef TEST_POWEROFF
	OS_MSleep(2000);
	printf("System will go to Poweroff Mode.\n"
	       " Prepare to test power consumption now.\n");
	//HAL_Wakeup_SetTimer_mS(10000);
	//HAL_Wakeup_SetIO(WAKEUP_IO_PIN_DEF, WAKEUP_IO_MODE_DEF, WAKEUP_IO_PULL_DEF);
	pm_enter_mode(PM_MODE_POWEROFF);
#endif

	button_deinit();

	return 0;
}
