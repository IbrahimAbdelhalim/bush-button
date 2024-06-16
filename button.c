#include "button.h"

#define NULL                      (void*)(0)

#define BUTTON_MODULE_TAG         0X00010000


void button_init(button_t* button)
{
   if (button == NULL)
    {
    __FAIL(BUTTON_ERROR_BUTTON_INSTANCE_EQUAL_NULL_POINTER);
    }
   else if (button->button_init_pin == NULL)
    {
    button->status = BUTTON_STATUS_ERROR;
    button->error_code == BUTTON_ERROR_BUTTON_INIT_PINSTATE_ROUTINE_EQUAL_NULL_POINTER;
    }
   else
    {
    button->button_init_pin();

    button->tag = BUTTON_MODULE_TAG;
    button->state = BUTTON_STATE_RELEASED;
    button->pressed_time = 0;
    button->released_time = 0;

    for (uint8_t sample_index = 0; sample_index < BUTTON_MAXIMUM_SAMPLES_NUMBER; ++sample_index)
        {
        button->samples[sample_index] = BUTTON_RELEASED_VALUE;
        }

    button->status = BUTTON_STATUS_OK;
    button->error_code = BUTTON_ERROR_NONE;
    }
}


button_state_t button_get_state(button_t* button)
{
    button_state_t state = BUTTON_STATE_RELEASED;

	if (button == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_INSTANCE_EQUAL_NULL_POINTER);
	}
	else if (button->tag != BUTTON_MODULE_TAG)
	{
	button->status = BUTTON_STATUS_ERROR;
	button->error_code = BUTTON_ERROR_INVALID_TAG;
	}
	else
	{
	state = button->state;
	}
	return state;
}


uint32_t button_get_pressed_time(button_t* button)
{
    uint32_t pressed_time = 0;

	if(button == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_INSTANCE_EQUAL_NULL_POINTER);
	}
	else if (button->tag != BUTTON_MODULE_TAG)
	{
	button->status = BUTTON_STATUS_ERROR;
	button->error_code = BUTTON_ERROR_INVALID_TAG;
	}
	else
	{
	pressed_time = button->pressed_time;
	}
	return pressed_time;
}

uint32_t button_get_released_time(button_t* button)
{
	uint32_t released_time = 0;

	if(button == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_INSTANCE_EQUAL_NULL_POINTER);
	}
	else if (button->tag != BUTTON_MODULE_TAG)
	{
	button->status = BUTTON_STATUS_ERROR;
	button->error_code = BUTTON_ERROR_INVALID_TAG;
	}
	else
	{
	released_time = button->released_time;
	}
	return released_time;
}

void button_set_initialization_pin_routine(button_t* button , void(*button_init_pin)(void))
{
	if (button == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_INSTANCE_EQUAL_NULL_POINTER);
	}
	else if (button_init_pin == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_INIT_PINSTATE_ROUTINE_EQUAL_NULL_POINTER);
	}
	else
	{
	button->button_init_pin = button_init_pin;
	}
}

void button_set_getting_pin_state_routine(button_t* button, uint8_t (*button_get_pinstate)(void))
{
	if (button == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_INSTANCE_EQUAL_NULL_POINTER);
	}
	else if (button_get_pinstate == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_GET_PINSTATE_ROUTINE_EQUAL_NULL_POINTER);
	}
	else
	{
	button->button_get_pinstate = button_get_pinstate;
	}
}

void button_update(button_t* button)
{
    if (button == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_INSTANCE_EQUAL_NULL_POINTER);
	}
	else if (button->tag != BUTTON_MODULE_TAG)
	{
	button->status = BUTTON_STATUS_ERROR;
	button->error_code = BUTTON_ERROR_INVALID_TAG;
	}
	else if (button->button_get_pinstate == NULL)
	{
	__FAIL(BUTTON_ERROR_BUTTON_GET_PINSTATE_ROUTINE_EQUAL_NULL_POINTER);
	}
	else
	{
    uint8_t sample_index;

	for (sample_index = 0; sample_index < BUTTON_MAXIMUM_SAMPLES_NUMBER - 1; ++sample_index)
		{
		button->samples[sample_index] = button->samples[sample_index + 1];
		}

    button->samples[sample_index] = button->button_get_pinstate();

	switch(button->state)
		{
		case BUTTON_STATE_RELEASED:
			{
			if ((button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 1] == BUTTON_PRESSED_VALUE) &&
			    (button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 2] == BUTTON_PRESSED_VALUE) &&
				(button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 4] == BUTTON_RELEASED_VALUE) &&
				(button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 5] == BUTTON_RELEASED_VALUE))
				{
				button->released_time = 0;
				button->state = BUTTON_STATE_PRE_PRESSED;
				}
			else
			{
			if (button->released_time < BUTTON_MAXIMUM_RELEASED_TIME_THRESHOLD_IN_MS)
				{
				button->released_time += BUTTON_TASK_PERIOD_IN_MS;
				}
		    else
				{
				//do nothing here
				}
			}
			break;
			}
	    case BUTTON_STATE_PRE_PRESSED:
			{
			if ((button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 1] == BUTTON_PRESSED_VALUE) &&
			    (button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 2] == BUTTON_PRESSED_VALUE) &&
				(button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 3] == BUTTON_PRESSED_VALUE))
				{
				button->state = BUTTON_STATE_PRESSED;
				}
		    else
				{
				//do nothing here
				}
			break;
			}
	    case BUTTON_STATE_PRESSED:
			{
			if ((button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 1] == BUTTON_RELEASED_VALUE) &&
			    (button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 2] == BUTTON_RELEASED_VALUE) &&
				(button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 4] == BUTTON_PRESSED_VALUE) &&
				(button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 5] == BUTTON_PRESSED_VALUE))
				{
				button->pressed_time = 0;
				button->state = BUTTON_STATE_PRE_RELEASED;
				}
			else
			{
			if (button->pressed_time < BUTTON_MAXIMUM_PRESSED_TIME_THRESHOLD_IN_MS)
				{
				button->pressed_time += BUTTON_TASK_PERIOD_IN_MS;
				}
		    else
				{
				//do nothing here
				}
			}
			break;
			}
	    case BUTTON_STATE_PRE_RELEASED:
			{
			if ((button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 1] == BUTTON_RELEASED_VALUE) &&
			    (button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 2] == BUTTON_RELEASED_VALUE) &&
				(button->samples[BUTTON_MAXIMUM_SAMPLES_NUMBER - 3] == BUTTON_RELEASED_VALUE))
				{
				button->state = BUTTON_STATE_RELEASED;
				}
		    else
				{
				//do nothing here
				}
			break;
			}
		default:
			{
            //put the module at defined state until the boss/supervisor takes it's decision to define what the suitable recovery is
			button->state = BUTTON_STATE_RELEASED;
			button->pressed_time = 0;
			button->released_time = 0;

			for(uint8_t sample_index = 0; sample_index < BUTTON_MAXIMUM_SAMPLES_NUMBER; ++sample_index)
				{
				button->samples[sample_index] = BUTTON_RELEASED_VALUE;
				}

			button->status = BUTTON_STATUS_ERROR;
			button->error_code = BUTTON_ERROR_INVALID_BUTTON_TRANSITION;
			break;
			}
		}
	}
}
