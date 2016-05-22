#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define MOTOR_IDLE 1000 // 750
#define MOTOR_START	1179	// motor starts to turn when given this value.
#define MOTOR_MAX 2000		// for safety
#define FRONT_LEFT_PIN 5
#define FRONT_RIGHT_PIN 4
#define BACK_LEFT_PIN 2
#define BACK_RIGHT_PIN 3

#define P_GAIN 3.0
#define I_GAIN 0.025
#define D_GAIN 40.0
#define P_MAX 100
#define I_MAX 50

#define YAW_P_GAIN 10.0
#define YAW_I_GAIN 0.0
#define YAW_D_GAIN 5.0


#define PID_INTERVAL_IN_MILLIS 2.0

#endif
