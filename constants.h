#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define MOTOR_IDLE 1000 // 750
#define MOTOR_START	1179	// motor starts to turn when given this value.
#define MOTOR_MAX 2000		// for safety
#define FRONT_LEFT_PIN 5
#define FRONT_RIGHT_PIN 4
#define BACK_LEFT_PIN 2
#define BACK_RIGHT_PIN 3

#define P_GAIN 2.6
#define I_GAIN 0.004
#define D_GAIN 1.0
#define P_MAX 90
#define I_MAX 7

#define YAW_P_GAIN 1000.0
#define YAW_I_GAIN 0.0
#define YAW_D_GAIN 0.0

#define THROTTLE_GAIN 1.0
#define AILERON_GAIN 0.05
#define ELEVATOR_GAIN 0.05
#define RUDDER_GAIN 0.3

#define PID_INTERVAL_IN_MILLIS 20.0

#endif
