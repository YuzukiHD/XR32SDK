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
#include <stdlib.h>
#include <string.h>

#include "driver/chip/hal_csi.h"
#include "driver/chip/hal_dma.h"
#include "driver/chip/hal_i2c.h"
#include "driver/chip/hal_uart.h"
#include "kernel/os/os.h"

#include "driver/component/csi_camera/camera_csi.h"
#include "driver/component/csi_camera/gc0308/drv_gc0308.h"

#define IMAGE_BUFFSIZE 153600

#define CAM_RESET_PIN GPIO_PIN_9
#define CAM_RESET_PORT GPIO_PORT_B

uint8_t* image_buff;

void Cam_Hardware_Reset(void) {
    printf("--%s---%d----\n", __func__, __LINE__);

    Drv_GC0308_Pwdn_Pin_Ctrl(GPIO_PIN_LOW);
    Drv_GC0308_Reset_Pin_Ctrl(GPIO_PIN_LOW);
    OS_MSleep(3);
    Drv_GC0308_Reset_Pin_Ctrl(GPIO_PIN_HIGH);
    OS_MSleep(100);
}

void Cam_PowerInit(void) {
    Cam_PowerCtrlCfg PowerCtrlcfg;
//    PowerCtrlcfg.Cam_Pwdn_Port = CAM_POWERDOWN_PORT;
    PowerCtrlcfg.Cam_Reset_Port = CAM_RESET_PORT;

//    PowerCtrlcfg.Cam_Pwdn_Pin = CAM_POWERDOWN_PIN;
    PowerCtrlcfg.Cam_Reset_Pin = CAM_RESET_PIN;

    Drv_GC0308_PowerInit(&PowerCtrlcfg);
    Drv_GC0308_EnvironmentInit();
}

int Cam_Init(uint8_t* imagebuf) {
    HAL_CSI_Moudle_Enalbe(CSI_DISABLE);
    if (Drv_GC0308_Init() == COMP_ERROR)
        return COMP_ERROR;
    else
        OS_MSleep(500);

    Drv_GC0308_Set_SaveImage_Buff(( uint32_t )imagebuf);
    HAL_CSI_Moudle_Enalbe(CSI_ENABLE);
    return COMP_OK;
}

int main(void) {
    uint32_t image_size = 0;

    image_buff = ( uint8_t* )malloc(IMAGE_BUFFSIZE);
    if (image_buff == NULL) {
        COMPONENT_WARN("image buff malloc error \r\n");
        return COMP_ERROR;
    }
    memset(image_buff, 0, IMAGE_BUFFSIZE);

    Cam_PowerInit();
    Cam_Hardware_Reset();
	
    if(Cam_Init(image_buff) == COMP_ERROR){
		printf("[GC0308] CAM INIT ERROR\n");
	}

    Drv_GC0308_Capture_Enable(CSI_STILL_MODE, CSI_ENABLE);
    image_size = Drv_GC0308_Capture_Componemt(10000);
    printf("[GC0308] image_size %u\n", image_size);

    Drv_GC0308_DeInit();
    free(image_buff);

    return COMP_OK;
}
