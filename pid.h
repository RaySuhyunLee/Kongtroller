#ifndef PID_H_
#define PID_H_

class PIDController {
  private:
    double p_gain;
    double i_gain;
    double d_gain;
    long interval_in_millis;
  public:
    PIDController(double, double, double, long);
    double pid(double);
};

#endif
