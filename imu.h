#ifndef IMU_H_
#define IMU_H_

void initIMU();
void readIMU();
void getGyro(double*, double*, double*);
void getAcc(short*, short*, short*);

#endif
