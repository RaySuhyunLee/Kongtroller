/* Receiver.cpp
 * 
 * read pwm signals from the RC receiver. 
 */

#include <Arduino.h>
#include <EnableInterrupt.h>
#include "Constants.h"

#define CHANNEL_MAX 4
int value[CHANNEL_MAX];

#define INDEX_THROTTLE 0
#define INDEX_AILERON 1
#define INDEX_ELEVATOR 2
#define INDEX_RUDDER 3

#define PIN_THROTTLE A8
#define PIN_AILERON A9
#define PIN_ELEVATOR A10
#define PIN_RUDDER A11

volatile long timeStamp[CHANNEL_MAX];

#define READ_SIGNAL(pin, channel) \
  if (digitalRead((pin)) == HIGH) { \
    timeStamp[channel] = micros(); \
  } else { \
    value[channel] = micros() - timeStamp[channel]; \
  }

#define NEUTRALIZE(value) \
  (1500 - (value))

double expo(int value, float exponent) {
  int abs = value > 0 ? value : -value;
  double powed = pow(abs, exponent);
  if (value < 0) {
    powed = powed * -1;
  }
  return powed;
}

int cut(int value, int start, int end) {
  if (value >= start && value <= end) {
    value = 0;
  }
  return value;
}

void readThrottle() { READ_SIGNAL(PIN_THROTTLE, INDEX_THROTTLE) }
void readAileron() { READ_SIGNAL(PIN_AILERON, INDEX_AILERON) }
void readElevator() { READ_SIGNAL(PIN_ELEVATOR, INDEX_ELEVATOR) }
void readRudder() { READ_SIGNAL(PIN_RUDDER, INDEX_RUDDER) }

void initReceiver(void) {
  pinMode(PIN_THROTTLE, INPUT);
  pinMode(PIN_AILERON, INPUT);
  pinMode(PIN_ELEVATOR, INPUT);
  pinMode(PIN_RUDDER, INPUT);
  enableInterrupt(PIN_THROTTLE, &readThrottle, CHANGE);
  enableInterrupt(PIN_AILERON, &readAileron, CHANGE);
  enableInterrupt(PIN_ELEVATOR, &readElevator, CHANGE);
  enableInterrupt(PIN_RUDDER, &readRudder, CHANGE);
}

void readReceiver(int *throttle, int *aileron, int *elevator, int *rudder) {
  *throttle = value[INDEX_THROTTLE];
  *aileron = (int)(expo(NEUTRALIZE(value[INDEX_AILERON]), 1.6) / 37.4);
  *elevator = (int)(expo(NEUTRALIZE(value[INDEX_ELEVATOR]), 1.6) / 37.4);
  *rudder = cut(NEUTRALIZE(value[INDEX_RUDDER]), -20, 20);
}
