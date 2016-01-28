#ifndef IMU_H_
#define IMU_H_

void initIMU();
void readIMU();
void getGyro(short*, short*, short*);
void getAcc(short*, short*, short*);

#endif
