#include <Servo.h>

#define FRONT_LEFT_PIN 5
#define FRONT_RIGHT_PIN 4
#define BACK_LEFT_PIN 2
#define BACK_RIGHT_PIN 3

#define MOTOR_IDLE 750
#define MOTOR_MAX 1500

Servo front_left;
Servo front_right;
Servo back_left;
Servo back_right;

int filter_bound(int value) {
  return constrain(value, MOTOR_IDLE, MOTOR_MAX);
}

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
	front_right.writeMicroseconds(filter_bound(fr));
	back_left.writeMicroseconds(filter_bound(bl));
	back_right.writeMicroseconds(filter_bound(br));
}

void setup() {
  init_motors();
  delay(5000);
  Serial.begin(9600);
}

void loop() {
  static int throttle;
  if (Serial.available() > 0) {
    throttle = Serial.parseInt();
    set_motors(throttle, throttle, throttle, throttle);
  }
}
