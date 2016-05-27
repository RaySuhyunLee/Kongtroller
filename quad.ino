#include "constants.h"
#include "motors.h"
#include "imu.h"
#include "pid.h"
#include "receiver.h"

/* uncomment below macros when you need debugging */
//#define DEBUG_MOTORS
//#define DEBUG_RECEIVER
//#define DEBUG_PID
//#define DEBUG_IMU

PIDController rollRateCtrl(RATE_P_GAIN, RATE_I_GAIN, RATE_D_GAIN, PID_INTERVAL_IN_MILLIS);
PIDController pitchRateCtrl(RATE_P_GAIN, RATE_I_GAIN, RATE_D_GAIN, PID_INTERVAL_IN_MILLIS);
PIDController yawRateCtrl(YAW_P_GAIN, YAW_I_GAIN, YAW_D_GAIN, PID_INTERVAL_IN_MILLIS);
PIDController altitudeCtrl(ALTITUDE_P_GAIN, ALTITUDE_I_GAIN, ALTITUDE_D_GAIN, PID_INTERVAL_IN_MILLIS);

unsigned long testCnt=0;

#define GET_RATE(value_diff, interval, rate) \
  if ((value_diff) < -180)  (rate) = (value_diff) + 360; \
  else if ((value_diff) > 180) (rate) = (value_diff) - 360; \
  (rate) = (rate) / interval;

double getRate(double value_diff, long interval) {
  double rate;
  if (value_diff < -180) {
    rate = value_diff + 360;
  }
  else if (value_diff > 180) {
    rate = value_diff - 360;
  } else {
    rate = value_diff;
  }
  rate = rate * 1000 / interval;
  return rate;
}

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
}

unsigned long prev_time=0;

void loop() {
  unsigned long elapsed_time, current_time;
  current_time = millis();
  elapsed_time = current_time - prev_time;
  static int throttle, aileron, elevator, rudder;
  static double roll_current, roll_prev, pitch_current, pitch_prev, yaw_prev, yaw_current;
  static double acc_x, acc_y, acc_z;
  
  if (Serial.available()) {
    throttle = Serial.parseInt();
  }
  if ((elapsed_time >= PID_INTERVAL_IN_MILLIS)) {
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
    roll_prev = roll_current;
    pitch_prev = pitch_current;
    yaw_prev = yaw_current;
    getGyro(&roll_current, &pitch_current, &yaw_current);
    getAcc(&acc_x, &acc_y, &acc_z);

#ifdef DEBUG_IMU
    Serial.print("roll: ");
    Serial.print(roll_current);
    Serial.print(" | pitch: ");
    Serial.print(pitch_current);
    Serial.print(" | yaw: ");
    Serial.println(yaw_current);
#endif

    if (throttle < MOTOR_START) {
      init_motors();
    } else {
      prev_time = current_time;

      double out_roll, out_yaw, out_pitch, out_altitude;
      int fl, fr, bl, br;

      testCnt++;
      //Serial.println(testCnt);

      /* Rate Control Logic */

      double roll_p, roll_i, roll_d, pitch_p, pitch_i, pitch_d, yaw_p, yaw_i, yaw_d;
      double roll_omega, pitch_omega, yaw_omega;
      pitch_omega = getRate(pitch_current-pitch_prev, elapsed_time);
      roll_omega = getRate(roll_current-roll_prev, elapsed_time);
      yaw_omega = getRate(yaw_current-yaw_prev, elapsed_time);

      out_roll = rollRateCtrl.pid(roll_omega, elapsed_time, &roll_p, &roll_i, &roll_d) - aileron;
      out_pitch = pitchRateCtrl.pid(pitch_omega, elapsed_time, &pitch_p, &pitch_i, &pitch_d) - elevator;
      out_yaw = yawRateCtrl.pid(yaw_omega, elapsed_time) - rudder;

      double acc_climb = acc_z - GRAVITIONAL_ACC;
      // TODO implement acc_climb calculation logic
      out_altitude = altitudeCtrl.pid(acc_climb, elapsed_time);

#ifdef DEBUG_PID
      Serial.print(out_roll);
      Serial.print(" ");
      Serial.print(out_pitch);
      Serial.print(" ");
      Serial.println(out_yaw);
#endif

      fl = throttle - out_roll - out_pitch + out_yaw - out_altitude;
      fr = throttle + out_roll - out_pitch - out_yaw - out_altitude;
      bl = throttle - out_roll + out_pitch - out_yaw - out_altitude;
      br = throttle + out_roll + out_pitch + out_yaw - out_altitude;
      set_motors(fl, fr, bl, br);
#ifdef DEBUG_MOTORS
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
