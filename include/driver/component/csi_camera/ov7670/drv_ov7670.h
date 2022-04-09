/**
  * @file  drv_ov7670.h
  * @author  XRADIO IOT WLAN Team
  */

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

#ifndef __OV7670_H__
#define __OV7670_H__

#include "driver/component/component_def.h"
#include "driver/chip/hal_gpio.h"
#include "driver/chip/hal_csi.h"

#ifdef __cplusplus
	 extern "C" {
#endif

#define OV7670_PICTURE_SEND_UART UART0_ID

void Drv_Ov7670_PowerInit(Cam_PowerCtrlCfg *cfg);
void Drv_Ov7670_Reset_Pin_Ctrl(GPIO_PinState state);
void Drv_Ov7670_Pwdn_Pin_Ctrl(GPIO_PinState state);
Component_Status Drv_Ov7670_Init();
Component_Status Drv_Ov7670_Capture_Enable(CSI_CAPTURE_MODE mode , CSI_CTRL ctrl);
void Drv_Ov7670_Set_SaveImage_Buff(uint32_t image_buff_addr);
uint32_t Drv_Ov7670_Capture_Componemt(uint32_t timeout_ms);
void Drv_Ov7670_Uart_Send_Picture(void *buf, uint32_t image_size);
void Drv_Ov7670_DeInit();

void Drv_OV7670_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void Drv_OV7670_Special_Effects(CAM_SPECAIL_EFFECTS eft);
void Drv_OV7670_Contrast(CAM_CONTARST contrast);
void Drv_CAM_LIGHT_MODE(CAM_LIGHT_MODE light_mode);
void Drv_CAM_COLOR_SATURATION(CAM_COLOR_SATURATION sat);
void Drv_CAM_BRIGHTNESS(CAM_BRIGHTNESS bright);

Component_Status Ov7670_Demo();

#ifdef __cplusplus
	 }
#endif

#endif /* _OV7670_H_ */
