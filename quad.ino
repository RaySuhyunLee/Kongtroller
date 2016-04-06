#include "constants.h"
#include "motors.h"
#include "imu.h"
#include "pid.h"

//#define DEBUG    // uncomment when you need debugging

PIDController rollCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);
PIDController pitchCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);

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
    } else {
      pref_time = current_time;

      double roll, yaw, pitch;
      double diff_roll, diff_yaw, diff_pitch;
      int fl, fr, bl, br;

      readIMU();
      getGyro(&roll, &pitch, &yaw);
      diff_roll = rollCtrl.pid(roll - 0);
      diff_pitch = pitchCtrl.pid(pitch - 0);
      Serial.print("diff_roll: ");
      Serial.print(diff_roll);
      Serial.print(" | diff_pitch: ");
      Serial.println(diff_pitch);

      fl = throttle - diff_roll - diff_pitch;
      fr = throttle + diff_roll - diff_pitch;
      bl = throttle - diff_roll + diff_pitch;
      br = throttle + diff_roll + diff_pitch;
      set_motors(fl, fr, bl, br);
#ifdef DEBUG
      Serial.print("motors: [");
      Serial.print(fl, DEC);
      Serial.print(" ");
      Serial.print(fr, DEC);
      Serial.print(" ");
      Serial.print(bl, DEC);
      Serial.print(" ");
      Serial.print(br, DEC);
      Serial.println("]");
#endif
    }
  }
}
