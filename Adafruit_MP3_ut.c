/*****************************************************************************
 * Static data
 *****************************************************************************/
static char module_desc[] = "mp3 unit test driver";

static utmgr_test_argtype_t tc_0_args_desc = {1, {UTMGR_ARG_TYPE_LWORD}};
static utmgr_test_argtype_t tc_0_ret_desc = {1, {UTMGR_ARG_TYPE_LWORD}};

static utmgr_test_argtype_t tc_1_args_desc = {1, {UTMGR_ARG_TYPE_LWORD}};
static utmgr_test_argtype_t tc_1_ret_desc = {1, {UTMGR_ARG_TYPE_LWORD}};

static utmgr_test_argtype_t tc_2_args_desc = {1, {UTMGR_ARG_TYPE_LWORD}};
static utmgr_test_argtype_t tc_2_ret_desc = {1, {UTMGR_ARG_TYPE_LWORD}}; 

Adafruit_MP3 mp3_obj;

/*****************************************************************************
 * Static functions
 *****************************************************************************/
static uint8_t tc_0_prepare(void)
{
	SEGGER_RTT_printf(0, "tc 0 prepare\n");
	return (0);
}

static void tc_0_run(void *p_in[], void *p_out[])
{     
   SEGGER_RTT_printf(0, "tc 0 run\n");
   SEGGER_RTT_printf(0, "arg 1: %d\n", *(uint32_t *)p_in[0]); 

   Adafruit_MP3_construct(&mp3_obj);
   Adafruit_MP3_begin(&mp3_obj);
   *(uint32_t *)p_out[0] = 100;
}

static void tc_0_cleanup(void)
{
    SEGGER_RTT_printf(0, "tc 0 cleanup\n");	
}

static utmgr_test_desc_t tc_0_desc = {"mp3000",
                                      tc_0_prepare,
									  tc_0_run,
									  tc_0_cleanup,
									  &tc_0_args_desc,
									  &tc_0_ret_desc};
									  
static uint8_t tc_1_prepare(void)
{
	SEGGER_RTT_printf(0, "tc 1 prepare\n");
	return (0);
}

static void tc_1_run(void *p_in[], void *p_out[])
{     
   SEGGER_RTT_printf(0, "tc 1 run\n");
   SEGGER_RTT_printf(0, "arg 1: %d\n", *(uint32_t *)p_in[0]); 

   Adafruit_MP3_tick(&mp3_obj);
   *(uint32_t *)p_out[0] = 100;
}

static void tc_1_cleanup(void)
{
    SEGGER_RTT_printf(0, "tc 1 cleanup\n");	
}

static utmgr_test_desc_t tc_1_desc = {"mp3001",
                                      tc_1_prepare,
									  tc_1_run,
									  tc_1_cleanup,
									  &tc_1_args_desc,
									  &tc_1_ret_desc};

static uint8_t tc_2_prepare(void)
{
	SEGGER_RTT_printf(0, "tc 2 prepare\n");
	return (0);
}

static void tc_2_run(void *p_in[], void *p_out[])
{
    SEGGER_RTT_printf(0, "tc 2 run\n");
    SEGGER_RTT_printf(0, "arg 1: %d\n", *(uint32_t *)p_in[0]);
    MP3_Handler();
    *(uint32_t *)p_out[0] = 100;
}

static void tc_2_cleanup(void)
{
    SEGGER_RTT_printf(0, "tc 2 cleanup\n");	
}

static utmgr_test_desc_t tc_2_desc = {"mp3002",
                                      tc_2_prepare,
									  tc_2_run,
									  tc_2_cleanup,
									  &tc_2_args_desc,
									  &tc_2_ret_desc};									  
static utmgr_test_desc_t *tc_array[] = {&tc_0_desc, &tc_1_desc, &tc_2_desc};

static utmgr_test_desc_t *tc_find(char *p_tc_id)
{
	for (uint8_t i = 0; i < sizeof(tc_array)/sizeof(utmgr_test_desc_t *); i++)
		if (!strcmp(tc_array[i]->p_test_id, p_tc_id))
			return (tc_array[i]);
		
    return (0);
}

static utmgr_module_desc_t module_tc_desc = {module_desc, 3, tc_find};
/*****************************************************************************
 * Interface functions
 *****************************************************************************/

 utmgr_module_desc_t *mp3_get_module_desc(void)
 {
	 return (&module_tc_desc);
 }