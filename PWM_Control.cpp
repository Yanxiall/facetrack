#include "PWM_Control.hpp"

void  PWM_Control::setPWM()
{
	wiringPiSetup();
	pinMode (PWMPIN_DOWN, PWM_OUTPUT);
	pinMode (PWMPIN_UP, PWM_OUTPUT);
	pwmSetMode (PWM_MODE_MS);
	pwmSetRange (PWM_PERIOD);
	pwmSetClock (CLOCK_DIVIDER);	
}
		    
void PWM_Control::ControlServo(Step remote_input, Step face_input)
{	    
	if(remote_input.control_active)
	{
		duty_down += remote_input.step_down * PWM_DUTY_STEP;
		duty_up += remote_input.step_up * PWM_DUTY_STEP;
	}
	else if(face_input.control_active)
	{
		duty_down += face_input.step_down * PWM_DUTY_STEP;
		duty_up += face_input.step_up * PWM_DUTY_STEP;
	}
	else
	{
		// do nothing
	}						
	if (duty_down > DUTY_MAX) 
		duty_down = DUTY_MAX;
	if (duty_down < DUTY_MIN)
        duty_down = DUTY_MIN;
	if (duty_up > DUTY_MAX)
		duty_up = DUTY_MAX;
	if (duty_up < DUTY_MIN)
		duty_up = DUTY_MIN;
	// write to change the duty of PWM
	pwmWrite(PWMPIN_DOWN, duty_down);
	pwmWrite(PWMPIN_UP, duty_up);	
}







