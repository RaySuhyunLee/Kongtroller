#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define MOTOR_IDLE 1000 // 750
#define MOTOR_START	1179	// motor starts to turn when given this value.
#define MOTOR_MAX 1900		// for safety
#define FRONT_LEFT_PIN 5
#define FRONT_RIGHT_PIN 4
#define BACK_LEFT_PIN 2
#define BACK_RIGHT_PIN 3

#define P_GAIN 0.7
#define I_GAIN 0.05
#define D_GAIN 7.0
#define I_MAX 5
#define PID_INTERVAL_IN_MILLIS 1

#endif
