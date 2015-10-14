#include <Servo.h>

const int MOTOR_MAX = 150;			// for safety
const int FRONT_LEFT_PIN = 5;
const int FRONT_RIGHT_PIN = 4;
const int BACK_LEFT_PIN = 2;
const int BACK_RIGHT_PIN = 3;

int isread = 1;

Servo front_left;
Servo front_right;
Servo back_left;
Servo back_right;

void init_motors() {
	front_left.attach(FRONT_LEFT_PIN);
	front_right.attach(FRONT_RIGHT_PIN);
	back_left.attach(BACK_LEFT_PIN);
	back_right.attach(BACK_RIGHT_PIN);
	set_motors(0, 0, 0, 0);
}

// the setup routine runs once when you press reset:
void setup() {
	init_motors();
	
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start serial");
}

int filter_bound(int value) {
	if (value > MOTOR_MAX) {
		value = MOTOR_MAX;
	} else if (value < 0) {
		value = 0;
	}
	return value;
}

void set_motors(int fl, int fr, int bl, int br) {
	front_left.write(filter_bound(fl));
	front_right.write(filter_bound(fr));
	back_left.write(filter_bound(bl));
	back_right.write(filter_bound(br));
}

double pid_roll(double p, double i, double d, short roll, short desired) {
	int out_left, out_right;
	short in_diff;
	double out_diff;

	in_diff = desired - roll;
	out_diff = in_diff * p;
	return out_diff;
}

// the loop routine runs over and over again forever:
void loop() {
	short roll, yaw, pitch;
	double diff_roll;
	int fl, fr, bl, br;
	int throttle = 0;

	while(!Serial.available());
	throttle = Serial.parseInt();
	Serial.print("input: ");
	Serial.println(throttle, DEC);
	set_motors(throttle, throttle, throttle, throttle);

  delay(100);
}
