//#include <Std_Types.h>
#include "dbc.h"



dbc_ret_t dbc_init(dbc_t *d)
{
	if(d == NULL)
	{
		return DBC_ERR_INIT;
	}
	
	if(d->inited)
	{
		return DBC_ERR_INIT;
	}

	if( ((d->thres_lower_value) >= (d->thres_upper_value)) ||
	    (d->thres_lower_times == 0) ||
	    (d->thres_upper_times == 0) )
	{
		return DBC_ERR_PARAM;
	}

	d->state = DBC_STATE_UNKNOWN;
	d->times_upper_cnt = 0;
	d->times_lower_cnt = 0;
	d->times_normal_cnt = 0;
	d->inited = 1;

	return DBC_ERR_NONE;
}

dbc_ret_t dbc_deinit(dbc_t *d)
{
	if(d == NULL)
	{
		return DBC_ERR_INIT;
	}
		
	d->inited = 0;
	d->cbk = NULL;
	d->state = DBC_STATE_STAY_NORMAL;

	return DBC_ERR_NONE;
}

dbc_ret_t dbc_deal(dbc_t *d, dbc_value_t input)
{
	if(d == NULL)
	{
		return DBC_ERR_INIT;
	}
		
	if(d->inited==0)
	{
		return DBC_ERR_INIT;
	}

	if(input >= d->thres_upper_value)
	{
		d->times_lower_cnt = 0;
		d->times_normal_cnt = 0;

		d->times_upper_cnt++;
		if(d->times_upper_cnt >= d->thres_upper_times)
		{
			d->times_upper_cnt = 0;
			if( (d->state == DBC_STATE_TRIG_UPPER) || (d->state == DBC_STATE_STAY_UPPER) )
			{
				d->state = DBC_STATE_STAY_UPPER;
			}
			else
			{
				d->state = DBC_STATE_TRIG_UPPER;
			}
		}
	}
	else if(input <= d->thres_lower_value)
	{
		d->times_upper_cnt = 0;
		d->times_normal_cnt = 0;
		
		d->times_lower_cnt++;
		if(d->times_lower_cnt >= d->thres_lower_times)
		{
			d->times_lower_cnt = 0;
			if( (d->state == DBC_STATE_TRIG_LOWER) || (d->state == DBC_STATE_STAY_LOWER) )
			{
				d->state = DBC_STATE_STAY_LOWER;
			}
			else
			{
				d->state = DBC_STATE_TRIG_LOWER;
			}
		}
	}
	else
	{
		d->times_lower_cnt = 0;
		d->times_upper_cnt = 0;

		d->times_normal_cnt++;
		if(d->times_normal_cnt >= d->thres_normal_times)
		{
			d->times_normal_cnt = 0;
			if( (d->state == DBC_STATE_TRIG_NORMAL) || (d->state == DBC_STATE_STAY_NORMAL) )
			{
				d->state = DBC_STATE_STAY_NORMAL;
			}
			else
			{
				d->state = DBC_STATE_TRIG_NORMAL;
			}
		}
	}

//	if( (d->cbk != NULL) && 
//		(d->state ==DBC_STATE_TRIG_LOWER) &&
//		(d->state == DBC_STATE_TRIG_NORMAL)
//		(d->state == DBC_STATE_TRIG_UPPER) ) // Only call back at Triggered.
	if(d->cbk != NULL && d->state != DBC_STATE_UNKNOWN) // Call back anytime.
	{
		d->cbk(d->state);
	}

	if(d->state == DBC_STATE_TRIG_LOWER) d->state = DBC_STATE_STAY_LOWER;
	if(d->state == DBC_STATE_TRIG_UPPER) d->state = DBC_STATE_STAY_UPPER;
	if(d->state == DBC_STATE_TRIG_NORMAL) d->state = DBC_STATE_STAY_NORMAL;

	return DBC_ERR_NONE;
}

dbc_ret_t dbc_get_state(dbc_t *d, dbc_state_t *state)
{
	if(d == NULL)
	{
		return DBC_ERR_INIT;
	}
	
	*state =  d->state;
	
	return DBC_ERR_NONE;
}




#ifdef DBC_TEST

dbc_t dbc_ch1, dbc_ch2;

dbc_ret_t dbc_test_ch1_cbk(dbc_state_t state)
{
	//LOG("%d, ", state);

	return 0;
}

dbc_ret_t dbc_test_ch2_cbk(dbc_state_t state)
{
	//LOG("%d\r\n", state);

	return 0;
}


void dbc_test_init(void) // Call at initial.
{
	dbc_ch1.cbk = dbc_test_ch1_cbk;
	dbc_ch1.thres_lower_times = 5;
	dbc_ch1.thres_upper_times = 8;
	dbc_ch1.thres_normal_times = 10;
	dbc_ch1.thres_lower_value = 0;
	dbc_ch1.thres_upper_value = 1;
	
	(void)dbc_init(&dbc_ch1);


	dbc_ch2.cbk = dbc_test_ch2_cbk;
	dbc_ch2.thres_lower_times = 5;
	dbc_ch2.thres_upper_times = 6;
	dbc_ch2.thres_normal_times = 8;
	dbc_ch2.thres_lower_value = 20;
	dbc_ch2.thres_upper_value = 66;
	
	(void)dbc_init(&dbc_ch2);
}

void dbc_test(void) // Call at equal intervals.
{
 	uint16 tmp1 = 0, tmp2 = 0;

	/* get tmp1, tmp2 real value here. */
	
	(void)dbc_deal(&dbc_ch1, (dbc_value_t)tmp1);
	(void)dbc_deal(&dbc_ch2, (dbc_value_t)tmp2);
}



#endif










