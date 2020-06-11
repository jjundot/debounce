#ifndef __DBC_H__
#define __DBC_H__
/*
 * Debounce lib
 *
 *
 */

/*--------------Macro define-------------*/
//#define DBC_TEST

#define  DBC_STATE_UNKNOWN      0
#define  DBC_STATE_STAY_NORMAL  1
#define  DBC_STATE_TRIG_NORMAL  2
#define  DBC_STATE_STAY_UPPER   3
#define  DBC_STATE_TRIG_UPPER   4
#define  DBC_STATE_STAY_LOWER   5
#define  DBC_STATE_TRIG_LOWER   6

#define DBC_ERR_NONE   0
#define DBC_ERR_INIT   1
#define DBC_ERR_PARAM  2

/*--------------typedef-----------------*/
typedef uint8 dbc_state_t;
typedef uint8 dbc_ret_t;
typedef uint16 dbc_times_t;
typedef uint16 dbc_value_t;

typedef dbc_ret_t (*dbc_cbk_t)(dbc_state_t state);

//#pragma pack(push, 4)
typedef struct
{
	/* manual init. */
	dbc_cbk_t cbk;
	dbc_times_t thres_upper_times;
	dbc_times_t thres_lower_times;
	dbc_value_t thres_upper_value;
	dbc_value_t thres_lower_value;
	dbc_times_t thres_normal_times;

	/* auto init.*/
	dbc_times_t times_upper_cnt;
	dbc_times_t times_lower_cnt;
	dbc_times_t times_normal_cnt;
	dbc_state_t state;
	uint8  inited;
}dbc_t;
//#pragma pack(pop)

/*--------------function for out----------*/
dbc_ret_t dbc_init(dbc_t *d);
dbc_ret_t dbc_deinit(dbc_t *d);
dbc_ret_t dbc_deal(dbc_t *d, dbc_value_t input);
dbc_ret_t dbc_get_state(dbc_t *d, dbc_state_t *state);



#ifdef DBC_TEST
void dbc_test_init(void);
void dbc_test(void);

#endif

#endif


