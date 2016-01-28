#include "constants.h"
#include "motors.h"
#include "imu.h"
#include "pid.h"

//#define DEBUG    // uncomment when you need debugging

int isread = 1;
PIDController rollCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);

// the setup routine runs once when you press reset:
void setup() {
	init_motors();
	
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start serial");
  initIMU();
}

void loop() {
	short roll, yaw, pitch;
	double diff_roll;
	int fl, fr, bl, br;
	static int throttle = MOTOR_START+4;

  readIMU();
	getGyro(&roll, &yaw, &pitch);
  Serial.println(roll);
	diff_roll = rollCtrl.pid(roll);

	fl = throttle + diff_roll;
	fr = throttle - diff_roll;
	bl = throttle + diff_roll;
	br = throttle - diff_roll;
	set_motors(fl, fr, bl, br);
#ifdef DEBUG
	Serial.print(fl, DEC);
	Serial.print(" ");
	Serial.print(fr, DEC);
	Serial.print(" ");
	Serial.print(bl, DEC);
	Serial.print(" ");
	Serial.println(br, DEC);
#endif
  delay(10);
}
