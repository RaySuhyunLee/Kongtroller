#include "pid.h"

PIDController::PIDController(double p_gain, double i_gain, double d_gain, long interval) {
  this->p_gain = p_gain;
  this->i_gain = i_gain;
  this->d_gain = d_gain;
  interval_in_millis = interval;
}

double PIDController::pid(double error) {
  return error * p_gain;
}
