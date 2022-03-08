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

#include "board_debug.h"
#include "board.h"
#include "driver/chip/hal_codec.h"
#include <string.h>

/* uart */
#if PRJCONF_UART_EN
__nonxip_text
HAL_Status board_uart_init(UART_ID uart_id)
{
	__nonxip_data static const UART_InitParam board_uart_param = {
		.baudRate		  = BOARD_UART_BAUD_RATE,
		.parity 		  = BOARD_UART_PARITY,
		.stopBits		  = BOARD_UART_STOP_BITS,
		.dataBits		  = BOARD_UART_DATA_BITS,
		.isAutoHwFlowCtrl = BOARD_UART_HW_FLOW_CTRL
	};

	return HAL_UART_Init(uart_id, &board_uart_param);
}
#endif /* PRJCONF_UART_EN */

/* spi */
#if PRJCONF_SPI_EN
__weak HAL_Status board_spi_init(SPI_Port spi)
{
	static const SPI_Global_Config board_spi_param = {
		.mclk	  = BOARD_SPI_MCLK,
		.cs_level = BOARD_SPI_CS_LEVEL
	};

	return HAL_SPI_Init(spi, &board_spi_param);
}

__weak HAL_Status board_spi_deinit(SPI_Port spi)
{
	return HAL_SPI_Deinit(spi);
}
#endif /* PRJCONF_SPI_EN */

/* sound card0 */
#if PRJCONF_SOUNDCARD0_EN
__weak HAL_Status board_soundcard0_init(codec_detect_cb cb)
{
	static const I2C_InitParam i2c_param = {
		.addrMode	= BOARD_SOUNDCARD0_I2C_ADDR_MODE,
		.clockFreq	= BOARD_SOUNDCARD0_I2C_CLK
	};

	HAL_Status ret;

	ret = HAL_I2C_Init(BOARD_SOUNDCARD0_I2C_ID, &i2c_param);
	if (ret != HAL_OK) {
		BOARD_ERR("I2C %d init failed\n", BOARD_SOUNDCARD0_I2C_ID);
		return ret;
	}

	CODEC_InitParam codec_param;
	codec_param.cb = cb;
	ret = HAL_CODEC_Init(&codec_param);
	if (ret != HAL_OK) {
		BOARD_ERR("acodec init failed\n");
		HAL_I2C_DeInit(BOARD_SOUNDCARD0_I2C_ID);
		return ret;
	}

	uint8_t mclkDiv = 1;
	CODEC_DetectParam detect_param;
	HAL_CODEC_TYPE_Get(0, &detect_param, 0);
	if (detect_param.type == AUDIO_CODEC_AC101S)
		mclkDiv = 2;

	I2S_Param i2s_param;
	memset(&i2s_param, 0, sizeof(i2s_param));
	i2s_param.mclkDiv = mclkDiv;

	ret = HAL_I2S_Init(&i2s_param);
	if (ret != HAL_OK) {
		BOARD_ERR("I2S init failed\n");
		HAL_I2C_DeInit(BOARD_SOUNDCARD0_I2C_ID);
		HAL_CODEC_DeInit();
		return ret;
	}

	return ret;
}

__weak HAL_Status board_soundcard0_deinit(void)
{
	HAL_CODEC_DeInit();
	HAL_I2S_DeInit();
	return HAL_I2C_DeInit(BOARD_SOUNDCARD0_I2C_ID);
}
#endif /* PRJCONF_SOUNDCARD0_EN */

/* sound card1 */
#if PRJCONF_SOUNDCARD1_EN
__weak HAL_Status board_soundcard1_init(void)
{
	DMIC_Param dmic_param;
	memset(&dmic_param, 0, sizeof(dmic_param));

	return HAL_DMIC_Init(&dmic_param);
}

__weak HAL_Status board_soundcard1_deinit(void)
{
	HAL_DMIC_DeInit();
	return HAL_OK;
}
#endif /* PRJCONF_SOUNDCARD1_EN */

/* mmc card */
#if PRJCONF_MMC_EN
__weak HAL_Status board_sdcard_init(card_detect_cb cb)
{
	SDC_InitTypeDef sdc_param;
#ifdef CONFIG_DETECT_CARD
	sdc_param.cd_mode = PRJCONF_MMC_DETECT_MODE;
	sdc_param.cd_cb = cb;
#endif
	if (HAL_SDC_Init(0, &sdc_param) == NULL) {
		return HAL_ERROR;
	}
	return HAL_OK;
}
#endif
