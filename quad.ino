#include "constants.h"
#include "motors.h"
#include "imu.h"
#include "pid.h"
#include "receiver.h"
#include "starter.h"

/* uncomment below macros when you need debugging */
//#define DEBUG_MOTORS
//#define DEBUG_RECEIVER
//#define DEBUG_PID
//#define DEBUG_IMU

PIDController rollLevelCtrl(LEVEL_P_GAIN, LEVEL_I_GAIN, LEVEL_D_GAIN);
PIDController pitchLevelCtrl(LEVEL_P_GAIN, LEVEL_I_GAIN, LEVEL_D_GAIN);
PIDController rollRateCtrl(RATE_P_GAIN, RATE_I_GAIN, RATE_D_GAIN);
PIDController pitchRateCtrl(RATE_P_GAIN, RATE_I_GAIN, RATE_D_GAIN);
PIDController yawRateCtrl(YAW_P_GAIN, YAW_I_GAIN, YAW_D_GAIN);
PIDController altitudeCtrl(ALTITUDE_P_GAIN, ALTITUDE_I_GAIN, ALTITUDE_D_GAIN);

unsigned long testCnt=0;

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

void initPID();

// the setup routine runs once when you press reset:
void setup() {
  initStarter(2000, DISARM_TIME_IN_MILLIS);
	init_motors();
  delay(3000);
	
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start serial");
  initIMU();
  initReceiver();

  //rollLevelCtrl.setIMax(LEVEL_I_MAX);
  //pitchLevelCtrl.setIMax(LEVEL_I_MAX);
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
   
    if (!isArmed()) {
      if (rudder > 400) {
        startArmCount(); 
      } else {
        cancelArmCount();
      }
    }

    if (throttle < MOTOR_START) {
      init_motors();
      initPID();
      startDisarmCount();
    } else if (isArmed()){
      cancelDisarmCount();

      prev_time = current_time;

      testCnt++;
      //Serial.println(testCnt);
      
      static double roll_prev, pitch_prev, yaw_prev;

      double out_level_roll, out_level_pitch;
      double out_rate_roll, out_rate_yaw, out_rate_pitch, out_altitude;
      int fl, fr, bl, br;
      /* Level Control Logic */
#ifdef LEVEL_CONTROL
      out_level_roll = rollLevelCtrl.pid(aileron * LEVEL_AILERON_GAIN - roll_current, elapsed_time);
      out_level_pitch = pitchLevelCtrl.pid(elevator * LEVEL_ELEVATOR_GAIN - pitch_current, elapsed_time);
#else
      out_level_roll = aileron * RATE_AILERON_GAIN;
      out_level_pitch = elevator * RATE_ELEVATOR_GAIN;
#endif

      /* Rate Control Logic */
#ifdef RATE_CONTROL
      double roll_p, roll_i, roll_d, pitch_p, pitch_i, pitch_d;
      double roll_omega, pitch_omega;
      pitch_omega = getRate(pitch_current-pitch_prev, elapsed_time);
      roll_omega = getRate(roll_current-roll_prev, elapsed_time);

      out_rate_roll = rollRateCtrl.pid(roll_omega - out_level_roll, elapsed_time, &roll_p, &roll_i, &roll_d);
      out_rate_pitch = pitchRateCtrl.pid(pitch_omega - out_level_pitch, elapsed_time, &pitch_p, &pitch_i, &pitch_d);
#else
      out_rate_roll = 0;
      out_rate_pitch = 0;
#endif

      /* Yaw Control Logic */

      double yaw_p, yaw_i, yaw_d;
      double yaw_omega;
      yaw_omega = getRate(yaw_current-yaw_prev, elapsed_time);
      out_rate_yaw = yawRateCtrl.pid(yaw_omega, elapsed_time) - rudder * RUDDER_GAIN;

      /* Altitude Control Logic */

      double acc_climb = acc_z - GRAVITIONAL_ACC;
      // TODO implement acc_climb calculation logic
      out_altitude = altitudeCtrl.pid(acc_climb, elapsed_time);

#ifdef DEBUG_PID
      Serial.print(out_level_roll);
      Serial.print(" ");
      Serial.print(out_level_pitch);
      Serial.print(" ");
      Serial.println(out_rate_yaw);
#endif

      fl = throttle - out_altitude;
      fr = throttle - out_altitude;
      bl = throttle - out_altitude;
      br = throttle - out_altitude;

      fl += -out_rate_roll * 0.7 - out_rate_pitch + out_rate_yaw * 0.7;
      fr += out_rate_roll * 0.7 - out_rate_pitch - out_rate_yaw * 0.7;
      bl += -out_rate_roll + out_rate_pitch - out_rate_yaw;
      br += out_rate_roll + out_rate_pitch + out_rate_yaw;
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

      roll_prev = roll_current;
      pitch_prev = pitch_current;
      yaw_prev = yaw_current;
    }
  }
}

void initPID() {
  rollLevelCtrl.initMemory();
  pitchLevelCtrl.initMemory();
  rollRateCtrl.initMemory();
  pitchRateCtrl.initMemory();
  yawRateCtrl.initMemory();
  altitudeCtrl.initMemory();
}
