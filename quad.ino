#include "constants.h"
#include "motors.h"
#include "imu.h"
#include "pid.h"

//#define DEBUG    // uncomment when you need debugging

PIDController rollCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);

// the setup routine runs once when you press reset:
void setup() {
	init_motors();
  delay(5000);
	
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start serial");
  initIMU();
}

unsigned long pref_time=0;

void loop() {
  unsigned long current_time = millis();
  static int throttle = MOTOR_IDLE;
  if (Serial.available()) {
    throttle = Serial.parseInt();
  }
  if ((current_time >= pref_time + PID_INTERVAL_IN_MILLIS)) {
    if (throttle < MOTOR_START) {
      init_motors();
    }
    pref_time = current_time;

    double roll, yaw, pitch;
    double diff_roll, diff_yaw, diff_pitch;
    int fl, fr, bl, br;

    readIMU();
    getGyro(&roll, &yaw, &pitch);
    //Serial.println(roll);
    diff_roll = rollCtrl.pid(roll - 0);

    fl = throttle - diff_roll;
    fr = throttle + diff_roll;
    bl = throttle - diff_roll;
    br = throttle + diff_roll;
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
  }
}
