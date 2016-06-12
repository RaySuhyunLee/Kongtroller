#include <Arduino.h>
#include "constants.h"

uint8_t is_armed = 0;
unsigned long disarm_count;
unsigned long arm_count;
uint8_t is_disarm_count_enabled;
uint8_t is_arm_count_enabled;
unsigned long DISARM_TIME;
unsigned long ARM_TIME;

void disarm();

void initStarter(long arm_time_in_millis, long disarm_time_in_millis) {
  DISARM_TIME = disarm_time_in_millis;
  ARM_TIME = arm_time_in_millis;
  pinMode(RED_LIGHT_PIN, OUTPUT);
  pinMode(GREEN_LIGHT_PIN, OUTPUT);

  disarm();
}

void arm() {
  is_armed = 1;

  digitalWrite(GREEN_LIGHT_PIN, LOW);
  digitalWrite(RED_LIGHT_PIN, HIGH);
}

void disarm() {
  is_armed = 0;

  digitalWrite(GREEN_LIGHT_PIN, HIGH);
  digitalWrite(RED_LIGHT_PIN, LOW);
}

int isArmed() {
  if (is_disarm_count_enabled && millis() > disarm_count + DISARM_TIME) {
    is_disarm_count_enabled = 0;
    disarm();
  } else if (is_arm_count_enabled && millis() > arm_count + ARM_TIME) {
    is_arm_count_enabled = 0;
    arm();
  }
  return is_armed;
}

void startArmCount() {
  if (!is_arm_count_enabled) {
    arm_count = millis();
    is_arm_count_enabled = 1;

    digitalWrite(GREEN_LIGHT_PIN, HIGH);
    digitalWrite(RED_LIGHT_PIN, HIGH);
  }
}

void cancelArmCount() {
  is_arm_count_enabled = 0;
  disarm();
}

void startDisarmCount() {
  if (!is_disarm_count_enabled) {
    disarm_count = millis();
    is_disarm_count_enabled = 1;
  }
}

void cancelDisarmCount() {
  is_disarm_count_enabled = 0;
}
