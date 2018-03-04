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

#include "utmgr.h"
#include "util.h"

static char m_in_string0[UTMGR_MAX_STRING_SIZE];
static char m_in_string1[UTMGR_MAX_STRING_SIZE];
static char m_in_string2[UTMGR_MAX_STRING_SIZE];
static char m_in_string3[UTMGR_MAX_STRING_SIZE];
static char *m_in_strings[UTMGR_MAX_ARGS] = {m_in_string0,
                                             m_in_string1,
											 m_in_string2,
											 m_in_string3};
											 
static char m_out_string0[UTMGR_MAX_STRING_SIZE];
static char m_out_string1[UTMGR_MAX_STRING_SIZE];
static char m_out_string2[UTMGR_MAX_STRING_SIZE];
static char m_out_string3[UTMGR_MAX_STRING_SIZE];
static char *m_out_strings[UTMGR_MAX_ARGS] = {m_out_string0,
                                              m_out_string1,
											  m_out_string2,
											  m_out_string3};

static uint32_t m_in_long0;
static uint32_t m_in_long1;
static uint32_t m_in_long2;
static uint32_t m_in_long3;
static uint32_t *m_in_longs[UTMGR_MAX_ARGS] = {&m_in_long0,
                                           &m_in_long1,
										   &m_in_long2,
										   &m_in_long3};

static uint32_t m_out_long0;
static uint32_t m_out_long1;
static uint32_t m_out_long2;
static uint32_t m_out_long3;
static uint32_t *m_out_longs[UTMGR_MAX_ARGS] = {&m_out_long0,
                                            &m_out_long1,
										    &m_out_long2,
										    &m_out_long3};
											
static uint8_t m_next_in_string;
static uint8_t m_next_in_long;
static uint8_t m_next_out_string;
static uint8_t m_next_out_long;

static void *m_in_args[UTMGR_MAX_ARGS];
static void *m_out_args[UTMGR_MAX_ARGS];
 
static utmgr_module_desc_t *m_modules[UTMGR_MAX_MODULES] = {0};
static uint8_t m_next_module = 0;

static char m_ret_str[UTMGR_MAX_RET_STR_LEN];

static void utmgr_clean_arg(void)
{
	uint8_t i;
	for (i = 0; i < UTMGR_MAX_ARGS; i++)
	{
		memset(m_in_strings[i], 0, UTMGR_MAX_STRING_SIZE);
		memset(m_out_strings[i], 0, UTMGR_MAX_STRING_SIZE);
		*(m_in_longs[i]) = 0;
		*(m_out_longs[i]) = 0;
		m_in_args[i] = 0;
		m_out_args[i] = 0;
    }
	
    m_next_in_string = 0;
    m_next_in_long = 0;
    m_next_out_string = 0;
    m_next_out_long = 0;
}

static utmgr_test_desc_t *utmgr_get_tc(char *p_tc_str)
{
	uint8_t i;
	utmgr_test_desc_t *p_rc = 0;
	
	for (i = 0; i < m_next_module; i++)
	{
		p_rc = m_modules[i]->p_func_tc_find(p_tc_str);
		if (!p_rc)
			break;
	}	
	return (p_rc);
}
										   
void utmgr_register(utmgr_module_desc_t *p_module_desc)
{
	m_modules[m_next_module++] = p_module_desc;
}

static void utmgr_parse_args (uint8_t *p_msg, uint16_t len, utmgr_test_argtype_t *p_arg)
{
	uint16_t i = 0;
	uint16_t j = 1;
	uint8_t field_length[UTMGR_MAX_ARGS] = {0};
	uint8_t field_start[UTMGR_MAX_ARGS] = {0};
	
	// Clear the descriptor first.
    field_start[0] = 0;
	
	while (i < len && j < p_arg->num_of_arg)
	{
		if (*(p_msg + i) == UTMGR_COMMA)
		{
			field_length[j - 1] = i - field_start[j - 1];
			field_start[j++] = i + 1;
		}
		i++;
	}
	
	// Handle the last field.
	field_length[j - 1] = len - field_start[j - 1];
	
	for (i = 0; i < p_arg->num_of_arg; i++)
    {
		switch (p_arg->argtype[i])
		{
			case UTMGR_ARG_TYPE_LWORD:
				m_in_args[i] = (void *)&m_in_longs[m_next_in_long++];
			    *(uint32_t *)m_in_args[i] = ascii_to_longword(p_msg + field_start[i], field_length[i]);
			    break;
				
			case UTMGR_ARG_TYPE_STRING:
			    m_in_args[i] = (void *)m_in_strings[m_next_in_string];
				memcpy(m_in_strings[m_next_in_string++], p_msg + field_start[i], field_length[i]);
			    break;
				
			default:
			    break;
		}
	}
}

static void utmgr_set_return_args (utmgr_test_argtype_t *p_arg)
{
	uint16_t i;
	
	for (i = 0; i < p_arg->num_of_arg; i++)
    {
		switch (p_arg->argtype[i])
		{
			case UTMGR_ARG_TYPE_LWORD:
				m_out_args[i] = (void *)&m_out_longs[m_next_out_long];
                m_next_out_long++;
			    break;
				
			case UTMGR_ARG_TYPE_STRING:
			    m_out_args[i] = (void *)m_out_strings[m_next_out_string];
                m_next_out_string++;
			    break;
				
			default:
			    break;
		}
	}	
}

static uint16_t utmgr_build_args (uint8_t *p_msg, utmgr_test_argtype_t *p_arg)
{
	uint16_t i = 0;
    uint16_t len = 0;
	uint16_t tmplen = 0;
	
	for (i = 0; i < p_arg->num_of_arg; i++)
    {
		switch (p_arg->argtype[i])
		{
			case UTMGR_ARG_TYPE_LWORD:
                tmplen = longword_to_ascii (p_msg, *(uint32_t *)m_out_args[i]);
			    break;
				
			case UTMGR_ARG_TYPE_STRING:
				tmplen = strlen((char *)m_out_args[i]);
				memcpy(p_msg, (char *)m_out_args[i], tmplen);
			    break;
				
			default:
			    break;
		}
		len += tmplen;
		p_msg += tmplen;
		
		if (len > UTMGR_MAX_RET_STR_LEN)
			return (len);
		
		if ((i + 1) < p_arg->num_of_arg)
		{
		    *p_msg++ = UTMGR_COMMA;
            len++;			
		}
	}
	return (len);
}

uint8_t utmgr_exec_tc(char *p_tc_str, char *p_arg_str, char *p_ret_str)
{
	uint8_t rc;
	uint16_t len;
	utmgr_test_desc_t *p_tc_desc = utmgr_get_tc(p_tc_str);
	
	if (!p_tc_desc)
		return (UTMGR_RC_TEST_NOT_FOUND);
	
	utmgr_clean_arg();
	utmgr_parse_args((uint8_t *)p_arg_str, strlen(p_arg_str), p_tc_desc->p_in_desc);
	utmgr_set_return_args(p_tc_desc->p_out_desc);
	rc = p_tc_desc->p_func_tc_prepare();
	if (rc != UTMGR_RC_OK)
		return (rc);
	p_tc_desc->p_func_tc_run(m_in_args, m_out_args);
	p_tc_desc->p_func_tc_cleanup();
	len = utmgr_build_args((uint8_t *)m_ret_str, p_tc_desc->p_out_desc);
	if (len <= UTMGR_MAX_RET_STR_LEN)
		strcpy(p_ret_str, m_ret_str);
	else
	{
		memcpy(p_ret_str, m_ret_str, len);
		return (UTMGR_RC_TEST_RET_STR_TOO_LONG);
	}
	return (UTMGR_RC_OK);
}