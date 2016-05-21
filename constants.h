#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define MOTOR_IDLE 1000 // 750
#define MOTOR_START	1179	// motor starts to turn when given this value.
#define MOTOR_MAX 1900		// for safety
#define FRONT_LEFT_PIN 5
#define FRONT_RIGHT_PIN 4
#define BACK_LEFT_PIN 2
#define BACK_RIGHT_PIN 3

#define P_GAIN 1.2
#define I_GAIN 0.02
#define D_GAIN 5.0//9.0
#define I_MAX 10
#define PID_INTERVAL_IN_MILLIS 2

#endif
