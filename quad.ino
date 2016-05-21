#include "constants.h"
#include "motors.h"
#include "imu.h"
#include "pid.h"
#include "receiver.h"

//#define DEBUG    // uncomment when you need debugging
//#define DEBUG_RECEIVER
#define DEBUG_PID
//#define DEBUG_IMU

PIDController rollCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);
PIDController pitchCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);
PIDController yawCtrl(P_GAIN, I_GAIN, D_GAIN, PID_INTERVAL_IN_MILLIS);

double initialYaw;

// the setup routine runs once when you press reset:
void setup() {
	init_motors();
  delay(5000);
	
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
  double roll, yaw, pitch;
  
  if (Serial.available()) {
    throttle = Serial.parseInt();
  }
  if ((current_time >= pref_time + PID_INTERVAL_IN_MILLIS)) {
    /* read values from RC receiver */
    readReceiver(&throttle, &aileron, &elevator, &rudder);
    //aileron = 1500;
    //elevator = 1500;
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
    getGyro(&roll, &pitch, &yaw);
#ifdef DEBUG_IMU
    Serial.print("roll: ");
    Serial.print(roll);
    Serial.print(" | pitch: ");
    Serial.print(pitch);
    Serial.print(" | yaw: ");
    Serial.println(yaw);
#endif

    if (throttle < MOTOR_START) {
      init_motors();
    } else {
      pref_time = current_time;

      double diff_roll, diff_yaw, diff_pitch;
      int fl, fr, bl, br;

      diff_roll = rollCtrl.pid(roll + (aileron - 1500) / 30.0);
      diff_pitch = pitchCtrl.pid(pitch + (elevator - 1500) / 30.0);
      //diff_yaw = yawCtrl.pid(yaw - initialYaw - (-rudder + 1500) / 20.0);
      diff_yaw = 0;
#ifdef DEBUG_PID
      Serial.print("diff_roll: ");
      Serial.print(diff_roll);
      Serial.print(" | diff_pitch: ");
      Serial.println(diff_pitch);
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
