#include "constants.h"
#include "motors.h"
#include "imu.h"
#include "pid.h"
#include "receiver.h"

//#define DEBUG    // uncomment when you need debugging
//#define DEBUG_RECEIVER
//#define DEBUG_PID
//#define DEBUG_IMU

PIDController rollCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);
PIDController pitchCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);
PIDController yawCtrl(YAW_P_GAIN, YAW_I_GAIN, YAW_D_GAIN, PID_INTERVAL_IN_MILLIS);

double initialYaw;

long testCnt=0;

// the setup routine runs once when you press reset:
void setup() {
	init_motors();
  delay(3000);
	
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start serial");
  initIMU();
  initReceiver();
  
  // initizlize yaw value
  readIMU();
  double tmp;
  getGyro(&tmp, &tmp, &initialYaw);
}

unsigned long pref_time=0;

void loop() {
  unsigned long current_time = millis();
  static int throttle, aileron, elevator, rudder;
  static double roll, pitch, yaw_prev, yaw_current;
  
  if (Serial.available()) {
    throttle = Serial.parseInt();
  }
  if ((current_time >= pref_time + PID_INTERVAL_IN_MILLIS)) {
    /* read values from RC receiver */
    readReceiver(&throttle, &aileron, &elevator, &rudder);

#ifdef DEBUG_RECEIVER
    Serial.print(throttle);
    Serial.print(" | ");
    Serial.print(aileron);
    Serial.print(" | ");
    Serial.print(elevator);
    Serial.print(" | ");
    Serial.println(rudder);
#endif

    readIMU();
    yaw_prev = yaw_current;
    getGyro(&roll, &pitch, &yaw_current);

#ifdef DEBUG_IMU
    Serial.print("roll: ");
    Serial.print(roll);
    Serial.print(" | pitch: ");
    Serial.print(pitch);
    Serial.print(" | yaw: ");
    Serial.println(yaw_current);
#endif

    if (throttle < MOTOR_START) {
      init_motors();
    } else {
      pref_time = current_time;

      double diff_roll, diff_yaw, diff_pitch;
      int fl, fr, bl, br;

      testCnt++;
      //Serial.println(testCnt);

      double roll_p, roll_i, roll_d, pitch_p, pitch_i, pitch_d, yaw_p, yaw_i, yaw_d;
      diff_roll = rollCtrl.pid(roll + (aileron - 1500) * AILERON_GAIN, &roll_p, &roll_i, &roll_d);
      diff_pitch = pitchCtrl.pid(pitch + (elevator - 1500) * ELEVATOR_GAIN, &pitch_p, &pitch_i, &pitch_d);
      double yaw_omega; // TODO move logics below to other module
      yaw_omega = yaw_current - yaw_prev;
      if (yaw_omega < -180)  yaw_omega += 360;
      else if (yaw_omega > 180) yaw_omega -= 360;
      diff_yaw = yawCtrl.pid(yaw_omega / PID_INTERVAL_IN_MILLIS);
      diff_yaw += (rudder - 1500) * 0.3;

#ifdef DEBUG_PID
      Serial.print(diff_roll);
      Serial.print(" ");
      Serial.print(diff_pitch);
      Serial.print(" ");
      Serial.println(diff_yaw);
#endif

      fl = throttle - diff_roll - diff_pitch + diff_yaw;
      fr = throttle + diff_roll - diff_pitch - diff_yaw;
      bl = throttle - diff_roll + diff_pitch - diff_yaw;
      br = throttle + diff_roll + diff_pitch + diff_yaw;
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
