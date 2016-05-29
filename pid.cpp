#include "pid.h"
#include "constants.h"
#include <Arduino.h>

PIDController::PIDController(double p_gain, double i_gain, double d_gain) {
  this->p_gain = p_gain;
  this->i_gain = i_gain;
  this->d_gain = d_gain;
  this->prev_error = 0;
  this->prev_i_out = 0;
  this->i_max = -1;
}

void PIDController::setIMax(double i_max) {
  this->i_max = i_max;
}

double PIDController::pid(double error, unsigned long elapsed_time, double *p_out, double *i_out, double *d_out) {
  double p, i, d;
  p = error * p_gain;
  i = prev_i_out + (error * (elapsed_time / 1000.0) * i_gain);
  d = (error - prev_error) / (elapsed_time / 1000.0) * d_gain;

  // cut i value for safety
  if (i_max >= 0) {
    i = constrain(*i_out, -i_max, i_max);
  }

  prev_error = error;
  prev_i_out = i;

  *p_out = p;
  *i_out = i;
  *d_out = d;

  return p + i + d;
}

double PIDController::pid(double error, unsigned long elapsed_time) {
  double p_out, i_out, d_out;
  
  return pid(error, elapsed_time, &p_out, &i_out, &d_out);
}

