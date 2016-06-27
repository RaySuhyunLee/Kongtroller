#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define DISARM_TIME_IN_MILLIS 7000

#define GREEN_LIGHT_PIN 12
#define RED_LIGHT_PIN 13

#define MOTOR_IDLE 1000 // 750
#define MOTOR_START	1179	// motor starts to turn when given this value.
#define MOTOR_MAX 2000		// for safety
#define FRONT_LEFT_PIN 5
#define FRONT_RIGHT_PIN 4
#define BACK_LEFT_PIN 2
#define BACK_RIGHT_PIN 3

//#define THREE_CELL
#define FOUR_CELL

#define LEVEL_CONTROL

#define RATE_CONTROL

#ifdef THREE_CELL
#define LEVEL_P_GAIN 0.5
#define LEVEL_I_GAIN 0.0 //2.0
#define LEVEL_D_GAIN 0.0 //0.8
#define LEVEL_I_MAX 10

#define RATE_P_GAIN 1.2
#define RATE_I_GAIN 0.1
#define RATE_D_GAIN 0.032
#endif

#ifdef FOUR_CELL
#define LEVEL_P_GAIN 2.5
#define LEVEL_I_GAIN 0.0
#define LEVEL_D_GAIN 0.0
#define LEVEL_I_MAX 10

#define RATE_P_GAIN 0.65
#define RATE_I_GAIN 7.0
#define RATE_D_GAIN 0.057
#endif

#define YAW_P_GAIN 3.0
#define YAW_I_GAIN 0.5
#define YAW_D_GAIN 0.0

#define ALTITUDE_P_GAIN 0.0//0.05
#define ALTITUDE_I_GAIN 0.0
#define ALTITUDE_D_GAIN 0.0
#define GRAVITIONAL_ACC 1000

#define LEVEL_AILERON_GAIN 0.05
#define LEVEL_ELEVATOR_GAIN 0.05
#define RATE_AILERON_GAIN 0.2
#define RATE_ELEVATOR_GAIN 0.2
#define RUDDER_GAIN 1.5

#define PID_INTERVAL_IN_MILLIS 20.0

#endif
