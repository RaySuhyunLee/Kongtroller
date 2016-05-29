#ifndef PID_H_
#define PID_H_

class PIDController {
  private:
    double p_gain;
    double i_gain;
    double d_gain;
    double prev_error;
    double prev_i_out;
    double i_max;
  public:
    PIDController(double, double, double);
    void setIMax(double);
    double pid(double, unsigned long, double*, double*, double*);
    double pid(double, unsigned long);
};

#endif
