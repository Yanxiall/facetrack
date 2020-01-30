// Summary: control PWM 
// Author:  Yanxia
// Date:    2020-01-21

#ifndef PWM_Control_hpp_
#define PWM_Control_hpp_

#include <wiringPi.h>
#include <iostream>
#include "DetectTrackFace.hpp"
#include "commonInterface.hpp"

#define PWMPIN_DOWN 24 //GPIO.24 as the output of PWM 
#define PWMPIN_UP 1    //GPIO.1 as the output of PWM
#define DUTY_MAX 400 // duty = 400 / 4000 = 10%
#define DUTY_MIN 200 // duty = 200 / 4000 = 5%
#define PWM_PERIOD 4000 //period = PWM_PERIOD * (1/(19.2Mhz / CLOCK_DIVIDER)) = 20ms
#define CLOCK_DIVIDER 96
#define DELAY_TIME_MS 10 // 50ms
#define PWM_DUTY_STEP 3 // each step of the movement of servo
#define DEBUG

#define FACE_POSITION_MIDDLE_X 110.0 //  x coordinate of the middle point of the camera
#define FACE_POSITION_MIDDLE_Y 80.0 // y coordinate of the middle point of the camera
#define FACE_POSITION_THRESHOLD_X 26 //within the threshold the servo can not move
#define FACE_POSITION_THRESHOLD_Y 20

class PWM_Control
{	
	public:
        
	  PWM_Control()
	  {
           duty_down = (DUTY_MAX + DUTY_MIN) / 2;
           duty_up = (DUTY_MAX + DUTY_MIN) / 2;
		  		  
	  };
	  void setPWM();
	  void ControlServo(Step remote_input, Step face_input);
	  	  
	private:
	  int duty_down;
	  int duty_up;
	  
};
#endif
