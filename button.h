#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

#define __FAIL(__ERROR_CODE__)   (void)(__ERROR_CODE__)            //Implementation of this macro/function is system dependent


#define BUTTON_MAXIMUM_PRESSED_TIME_THRESHOLD_IN_MS          5000
#define BUTTON_MAXIMUM_RELEASED_TIME_THRESHOLD_IN_MS         6000

#define BUTTON_PRESSED_VALUE                               	1
#define BUTTON_RELEASED_VALUE                             	0


#define BUTTON_TASK_PERIOD_IN_MS                     		10  // assumption: switch bouncing time is equal to 10 ms
#define BUTTON_TASK_OFFSET_IN_MS                     		0   //design dependent

typedef enum
{
	BUTTON_STATUS_ERROR = 0,
	BUTTON_STATUS_OK
}button_status_t;

typedef enum
{
    BUTTON_ERROR_NONE = 0,
	BUTTON_ERROR_INVALID_TAG,
    BUTTON_ERROR_BUTTON_INSTANCE_EQUAL_NULL_POINTER,
	BUTTON_ERROR_INVALID_BUTTON_TRANSITION,
	BUTTON_ERROR_BUTTON_INIT_PINSTATE_ROUTINE_EQUAL_NULL_POINTER,
	BUTTON_ERROR_BUTTON_GET_PINSTATE_ROUTINE_EQUAL_NULL_POINTER
}button_errorcode_t;


typedef enum
{
	BUTTON_STATE_RELEASED = 0,
	BUTTON_STATE_PRE_PRESSED,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_PRE_RELEASED
}button_state_t;

typedef struct
{
	uint32_t tag;
	button_state_t state;
#define BUTTON_MAXIMUM_SAMPLES_NUMBER   5
    uint8_t samples[BUTTON_MAXIMUM_SAMPLES_NUMBER];
	uint32_t pressed_time;
	uint32_t released_time;
	/*
	 * Function pointers should be initialized before calling button_init() function
	*/
	void (*button_init_pin)(void);
    uint8_t (*button_get_pinstate)(void); //assumption: when the function is called which this pointer points to, it should return 1 if the button is pressed otherwise it returns 0
	button_status_t status;
	button_errorcode_t error_code;
}button_t;


void button_set_initialization_pin_routine(button_t* button , void(button_init_pin)(void));
void button_set_getting_pin_state_routine(button_t* button, uint8_t (*button_get_pinstate)(void));

void button_init(button_t *button);
button_state_t button_get_state(button_t* button);
void button_update(button_t* button);
uint32_t button_get_pressed_time(button_t* button);
uint32_t button_get_released_time(button_t* button);



#endif // __BUTTON_H__
