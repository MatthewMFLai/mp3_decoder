/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */

#define COMMA                   ','
#define LIMITER                 ';'
#define TERM0                   '\0'

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#include "SEGGER_RTT.h"

#include "utmgr.h"
#include "Adafruit_MP3.h"

/**
 * @brief Function for application main entry.
 */

int main(void)
{	
    uint8_t i, idx = 0;
	char c;
	char inbuf[128];
	char tc_id[8];
	char tc_arg[64];
	char tc_ret[64];
	SEGGER_RTT_Init();
	SEGGER_RTT_printf(0, "Hello World!\n");
		
	utmgr_register(mp3_get_module_desc());
	for (;;)
	{
		c = SEGGER_RTT_WaitKey(); // will block until data is available
		if(c == LIMITER)
		{
			// input buffer should look like
			// rbf001,10,20,test string 1,test string 2
			
			// Look for the first ","
			for (i = 0; i < idx; i++)
				if (inbuf[i] == COMMA)
					break;
				
			if (i == idx)
			{
				// reset idx
				idx = 0;
				continue;
			}
			memcpy(tc_id, inbuf, i);
			tc_id[i] = TERM0;
			i++;
			memcpy(tc_arg, &inbuf[i], idx - i);
			tc_arg[idx - i] = TERM0;
			SEGGER_RTT_printf(0, "tc id: %s arg: %s\n", tc_id, tc_arg);
			utmgr_exec_tc(tc_id, tc_arg, tc_ret);
			SEGGER_RTT_printf(0, "Result: %s\n", tc_ret);
			
			// reset buffer index
			idx = 0;
		}
		else
			inbuf[idx++] = c;
		//power_manage();
	}
}
/**
 *@}
 **/
