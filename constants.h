#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define MOTOR_IDLE 750
#define MOTOR_START	1179	// motor starts to turn when given this value.
#define MOTOR_MAX 1500 //1700		// for safety
#define FRONT_LEFT_PIN 5
#define FRONT_RIGHT_PIN 4
#define BACK_LEFT_PIN 2
#define BACK_RIGHT_PIN 3

#define P_GAIN 0.4 //1.2
#define I_GAIN 0.01 // 0.025
#define D_GAIN 8.0 //80.0
#define I_MAX 20
#define PID_INTERVAL_IN_MILLIS 2

#endif
