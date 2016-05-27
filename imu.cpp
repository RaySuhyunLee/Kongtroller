#include "Arduino.h"

#define BUFFER_LENGTH 20
byte imu_buffer[BUFFER_LENGTH];

void initIMU() {
  Serial3.begin(19200);
}

void readIMU() {
  Serial3.write('*');
  int bytes = Serial3.readBytes(imu_buffer, 16);
	if (Serial3.available() > 0) {
    int cnt = 0;
  }
  /*
  for(int i=0; i<18; i++) {
    Serial.print(imu_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");*/
}

void getGyro(double* roll, double* pitch, double* yaw) {
  *roll = (imu_buffer[2] << 8 | imu_buffer[3]) / 100.0;
  *pitch = (imu_buffer[4] << 8 | imu_buffer[5]) / 100.0;
  *yaw = (imu_buffer[6] << 8 | imu_buffer[7]) / 100.0;
}

void getAcc(double* x, double* y, double* z) {
  *x = (imu_buffer[8] << 8 | imu_buffer[9]);
  *y = (imu_buffer[10] << 8 | imu_buffer[11]);
  *z = (imu_buffer[12] << 8 | imu_buffer[13]);
}
