#ifndef UTMGR_H__
#define UTMGR_H__

#define UTMGR_RC_OK                 0
#define UTMGR_RC_TEST_NOT_FOUND     1
#define UTMGR_RC_TEST_ABORT         2
#define UTMGR_RC_TEST_RET_STR_TOO_LONG 3

#define UTMGR_ARG_TYPE_LWORD        0
#define UTMGR_ARG_TYPE_STRING       1

#define UTMGR_MAX_ARGS              4
#define UTMGR_MAX_STRING_SIZE       64
#define UTMGR_MAX_TC                255
#define UTMGR_MAX_MODULES           255
#define UTMGR_MAX_RET_STR_LEN       255

#define UTMGR_COMMA                 ','

typedef struct
{
	uint16_t	num_of_arg;
	uint8_t 	argtype[UTMGR_MAX_ARGS];
} utmgr_test_argtype_t;

typedef struct
{
	char *p_test_id;
	uint8_t (*p_func_tc_prepare)(void);
	void (*p_func_tc_run)(void *p_in[], void *p_out[]);
	void (*p_func_tc_cleanup)(void);
    utmgr_test_argtype_t *p_in_desc;
	utmgr_test_argtype_t *p_out_desc;
} utmgr_test_desc_t;

typedef struct
{
	char *module_desc;
    uint8_t num_of_tc;
    utmgr_test_desc_t *(*p_func_tc_find)(char *p_tc_id);	
} utmgr_module_desc_t;

void utmgr_register(utmgr_module_desc_t *p_module_desc);
uint8_t utmgr_exec_tc(char *p_tc_str, char *p_arg_str, char *p_ret_str);

#endif  /* _ UTMGR_H__ */
