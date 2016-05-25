#ifndef PID_H_
#define PID_H_

class PIDController {
  private:
    double p_gain;
    double i_gain;
    double d_gain;
    double prev_error;
    double prev_i_out;
    long interval_in_millis; /* FIXME DEPRECATED. Gonna be removed in the future. */
  public:
    PIDController(double, double, double, long);
    double pid(double, unsigned long, double*, double*, double*);
    double pid(double, unsigned long);
};

#endif
