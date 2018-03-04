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
 * @defgroup ble_sdk_app_template_main main.c
 * @{
 * @ingroup ble_sdk_app_template
 * @brief Template project main file.
 *
 * This file contains a template for creating a new application. It has the code necessary to wakeup
 * from button, advertise, get a connection restart advertising on disconnect and if no new
 * connection created go back to system-off mode.
 * It can easily be used as a starting point for creating a new application, the comments identified
 * with 'YOUR_JOB' indicates where and how you can customize.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "util.h"

static uint8_t m_lookup[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                             'A', 'B', 'C', 'D', 'E', 'F'};
					 
uint8_t byte_to_ascii (uint8_t *p_dest, uint8_t value)
{
    uint8_t quotient;
    uint8_t rc = 0;
	uint8_t divisor = DIVISOR_BYTE;
	bool check_leading_zero = true;
	
	if (!value)
	{
		*p_dest = ZERO;
		return (1);
	}
	
    for (uint8_t i = 0; i < CONFIG_BYTE_DIGITS_MAX; i++)
    {
		quotient = value / divisor;
		value = value % divisor;
		if (check_leading_zero)
		{
			if (quotient)
			{
				*p_dest++ = quotient + ZERO;
				rc++;
				check_leading_zero = false;
			}
		}
		else
		{
			*p_dest++ = quotient + ZERO;
			rc++;
		}			
		divisor = divisor / DIVISOR;
    }
    return (rc);
}

uint8_t signed_byte_to_ascii (uint8_t *p_dest, int8_t signed_value)
{
	uint8_t value;
    uint8_t quotient;
    uint8_t rc = 0;
	uint8_t divisor = DIVISOR_BYTE;
	bool check_leading_zero = true;
	
	if (!signed_value)
	{
		*p_dest = ZERO;
		return (1);
	}
	
	if (signed_value < 0)
	{
		*p_dest++ = MINUS;
		signed_value *= -1;
		rc++;
	}
	value = (uint8_t)signed_value;
	
    for (uint8_t i = 0; i < CONFIG_BYTE_DIGITS_MAX; i++)
    {
		quotient = value / divisor;
		value = value % divisor;
		if (check_leading_zero)
		{
			if (quotient)
			{
				*p_dest++ = quotient + ZERO;
				rc++;
				check_leading_zero = false;
			}
		}
		else
		{
			*p_dest++ = quotient + ZERO;
			rc++;
		}			
		divisor = divisor / DIVISOR;
    }
    return (rc);
}

uint8_t word_to_ascii (uint8_t *p_dest, uint16_t value)
{
    uint16_t divisor = DIVISOR_WORD;
    uint8_t quotient;
    uint8_t rc = 0;
	bool check_leading_zero = true;
	
	if (!value)
	{
		*p_dest = ZERO;
		return (1);
	}
	
    for (uint8_t i = 0; i < CONFIG_WORD_DIGITS_MAX; i++)
    {
		quotient = value / divisor;
		value = value % divisor;
		if (check_leading_zero)
		{
			if (quotient)
			{
				*p_dest++ = quotient + ZERO;
				rc++;
				check_leading_zero = false;
			}
		}
		else
		{
			*p_dest++ = quotient + ZERO;
			rc++;
		}
		divisor = divisor / DIVISOR;
    }
    return (rc);
}

uint8_t longword_to_ascii (uint8_t *p_dest, uint32_t value)
{
    uint32_t divisor = DIVISOR_LONGWORD;
    uint8_t quotient;
    uint8_t rc = 0;
	bool check_leading_zero = true;
	
	if (!value)
	{
		*p_dest = ZERO;
		return (1);
	}
	
    for (uint8_t i = 0; i < CONFIG_LONGWORD_DIGITS_MAX; i++)
    {
		quotient = value / divisor;
		value = value % divisor;
		if (check_leading_zero)
		{
			if (quotient)
			{
				*p_dest++ = quotient + ZERO;
				rc++;
				check_leading_zero = false;
			}
		}
		else
		{
			*p_dest++ = quotient + ZERO;
			rc++;
		}
		divisor = divisor / DIVISOR;
    }
    return (rc);
}

uint8_t ascii_to_byte (uint8_t *p_data, uint8_t len)
{
    uint8_t i;
    uint8_t rc = 0;

    for (i = 0; i < len; i++)
    {
		rc = (rc * SCALER) + *(p_data + i) - ZERO;
    }
    return (rc);
}

uint16_t ascii_to_word (uint8_t *p_data, uint8_t len)
{
    uint8_t i;
    uint16_t rc = 0;

    for (i = 0; i < len; i++)
    {
		rc = (rc * SCALER) + *(p_data + i) - ZERO;
    }
    return (rc);
}

uint32_t ascii_to_longword (uint8_t *p_data, uint8_t len)
{
    uint8_t i;
    uint32_t rc = 0;

    for (i = 0; i < len; i++)
    {
		rc = (rc * SCALER) + *(p_data + i) - ZERO;
    }
    return (rc);
}

static uint8_t bcd_convert (char nibble)
{
	if (nibble >= '0' && nibble <= '9')
		return (nibble - ZERO);
	
	if (nibble == 'A' || nibble == 'a') 
		return (10);
	else if (nibble == 'B' || nibble == 'b')
		return (11);
	else if (nibble == 'C' || nibble == 'c')
		return (12);
	else if (nibble == 'D' || nibble == 'd')
		return (13);
	else if (nibble == 'E' || nibble == 'e')
		return (14);	
	else if (nibble == 'F' || nibble == 'f')
		return (15);
	else
		return (0);
}

uint8_t ascii_to_bcd (char msn, char lsn)
{
    return (BCD_MULTIPLIER * bcd_convert(msn) + bcd_convert(lsn));
	
}

void big_to_small_endian(uint8_t *p_data, uint8_t len)
{
    uint8_t val;
	uint8_t i;
	uint8_t halfpoint;
	
	halfpoint = len >> 1;
	for (i = 0; i < halfpoint; i++)
	{
		val = *(p_data + i);
		*(p_data + i) = *(p_data + len - i - 1);
		*(p_data + len - i - 1) = val;
	}
}

void byte_to_hex(char *p_msnibble, char *p_lsnibble, uint8_t value)
{
    *p_msnibble = m_lookup[value >> 4];
    *p_lsnibble = m_lookup[(value & 0x0f)];	
}

void string_to_bcd(char *p_src, uint16_t len, char *p_dest)
{
	uint16_t i, j;

	j = 0;
	for (i = 0; i < len; i = i + 2)
	{
		*(p_dest + j) = ascii_to_bcd(*(p_src + i), *(p_src + i + 1));
		j++;			
	}
}

uint8_t calc_checksum(uint8_t *s)
{
	uint8_t result = 0;
	s++; // Skip dollar signal 
	while ((*s != '*') && (*s != '\0'))
		result ^= *s++;
	
	return (result);
}