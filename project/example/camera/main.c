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

#define IMAGE_BUFFSIZE 1000

#define CAM_RESET_PIN GPIO_PIN_13
#define CAM_RESET_PORT GPIO_PORT_A

#define CAM_POWERDOWN_PIN GPIO_PIN_12
#define CAM_POWERDOWN_PORT GPIO_PORT_A

uint8_t* image_buff;

void Cam_Hardware_Reset(void) {
    printf("--%s---%d----\n", __func__, __LINE__);

    Drv_GC0308_Pwdn_Pin_Ctrl(GPIO_PIN_LOW);
    Drv_GC0308_Reset_Pin_Ctrl(GPIO_PIN_LOW);
    OS_MSleep(30);
    Drv_GC0308_Reset_Pin_Ctrl(GPIO_PIN_HIGH);
    OS_MSleep(100);
}

void Cam_PowerInit(void) {
    Cam_PowerCtrlCfg PowerCtrlcfg;
    PowerCtrlcfg.Cam_Pwdn_Port = CAM_POWERDOWN_PORT;
    PowerCtrlcfg.Cam_Reset_Port = CAM_RESET_PORT;

    PowerCtrlcfg.Cam_Pwdn_Pin = CAM_POWERDOWN_PIN;  //开发板
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
    uint32_t image_size = 0, i;

    image_buff = ( uint8_t* )malloc(IMAGE_BUFFSIZE);
    if (image_buff == NULL) {
        COMPONENT_WARN("image buff malloc error \r\n");
        return COMP_ERROR;
    }
    memset(image_buff, 0, IMAGE_BUFFSIZE);

    while (1) {
        Cam_PowerInit();
        Cam_Hardware_Reset();
        Cam_Init(image_buff);
        Drv_GC0308_Capture_Enable(CSI_STILL_MODE, CSI_ENABLE);
        image_size = Drv_GC0308_Capture_Componemt(100000);
        printf("[GC0308] image_size %u\n", image_size);
        if (image_size == 320 * 240 * 2) {
            printf("[GC0308] image capture done\n");
            break;
        } else {
            for(i = 0; i < IMAGE_BUFFSIZE; i++)
                printf(" %u ", image_buff[i]);
            Drv_GC0308_DeInit();
        }
    }

    Drv_GC0308_DeInit();
    free(image_buff);

    return COMP_OK;
}