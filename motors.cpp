#include "Arduino.h"
#include "constants.h"
#include <Servo.h>

Servo front_left;
Servo front_right;
Servo back_left;
Servo back_right;

#define BACK_RIGHT_OFFSET 3

void set_motors(int, int, int, int);
int filter_bound(int);

void init_motors() {
	front_left.attach(FRONT_LEFT_PIN);
	front_right.attach(FRONT_RIGHT_PIN);
	back_left.attach(BACK_LEFT_PIN);
	back_right.attach(BACK_RIGHT_PIN);
  
  front_left.writeMicroseconds(MOTOR_IDLE);
  front_right.writeMicroseconds(MOTOR_IDLE);
  back_left.writeMicroseconds(MOTOR_IDLE);
  back_right.writeMicroseconds(MOTOR_IDLE);
}

void set_motors(int fl, int fr, int bl, int br) {
	front_left.writeMicroseconds(filter_bound(fl));
	//front_right.writeMicroseconds(filter_bound(fr));
	//back_left.writeMicroseconds(filter_bound(bl));
	back_right.writeMicroseconds(filter_bound(br));
}

int filter_bound(int value) {
  return constrain(value, MOTOR_START, MOTOR_MAX);
}


