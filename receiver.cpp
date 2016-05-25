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

#define PIN_THROTTLE 10
#define PIN_AILERON 11
#define PIN_ELEVATOR 12
#define PIN_RUDDER 13

volatile long timeStamp[CHANNEL_MAX];

#define READ_SIGNAL(pin, channel) \
  if (digitalRead((pin)) == HIGH) { \
    timeStamp[channel] = micros(); \
  } else { \
    value[channel] = micros() - timeStamp[channel]; \
  }

#define NEUTRALIZE(value) \
  (1500 - (value))

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
  *throttle = value[INDEX_THROTTLE] * THROTTLE_GAIN;
  *aileron = NEUTRALIZE(value[INDEX_AILERON]) * AILERON_GAIN;
  *elevator = NEUTRALIZE(value[INDEX_ELEVATOR]) * ELEVATOR_GAIN;
  *rudder = NEUTRALIZE(value[INDEX_RUDDER]) * RUDDER_GAIN;
}
