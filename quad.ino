#include <Servo.h>

#define DEBUG

#define MOTOR_IDLE 750
#define MOTOR_START	1180	// motor starts to turn when given this value.
#define MOTOR_MAX 1700		// for safety
#define FRONT_LEFT_PIN 5
#define FRONT_RIGHT_PIN 4
#define BACK_LEFT_PIN 2
#define BACK_RIGHT_PIN 3

int isread = 1;

Servo front_left;
Servo front_right;
Servo back_left;
Servo back_right;

void init_motors() {
	front_left.attach(FRONT_LEFT_PIN);
	front_right.attach(FRONT_RIGHT_PIN);
	back_left.attach(BACK_LEFT_PIN);
	back_right.attach(BACK_RIGHT_PIN);
	set_motors(0, 0, 0, 0);
}

// the setup routine runs once when you press reset:
void setup() {
	init_motors();
	
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start serial");
  Serial3.begin(115200);
}

int filter_bound(int value) {
	if (value > MOTOR_MAX) {
		value = MOTOR_MAX;
	} else if (value < 0) {
		value = 0;
	}
	return value;
}

void set_motors(int fl, int fr, int bl, int br) {
	front_left.writeMicroseconds(filter_bound(fl));
	front_right.writeMicroseconds(filter_bound(fr));
	back_left.writeMicroseconds(filter_bound(bl));
	back_right.writeMicroseconds(filter_bound(br));
}

void readMotion(short* roll, short* pitch, short* yaw) {
	if (Serial3.available() > 0) {
		char buffer[8];
    
    while(1) {
      Serial3.readBytes(buffer, 2);
      if (buffer[0] == 0x55 && buffer[1] == 0x55) {
        break;
      }
    }
    
    Serial3.readBytes(buffer, 8);
    *roll = buffer[0] * 0x100 + buffer[1];
    *pitch = buffer[2] * 0x100 + buffer[3];
    *yaw = buffer[4] * 0x100 + buffer[5];
    
    Serial.print(*roll, DEC);
    Serial.print(' ');
    Serial.print(*pitch, DEC);
    Serial.print(' ');
    Serial.println(*yaw, DEC);
  }
}

double pid_roll(double p, double i, double d, short roll, short desired) {
	int out_left, out_right;
	short in_diff;
	double out_diff;

	in_diff = desired - roll;
	out_diff = in_diff * p;
	return out_diff;
}

void sendCommand() {
	/* give signal to motion sensor */
	char wr;
  if (Serial.available() > 0) {
    wr = Serial.read();
    if (wr == '<') {
      isread = 0;
    } else if (wr == '>') {
      isread = 1;
    }
    Serial.write(wr);
    Serial3.write(wr);
  }
}


void loop() {
	short roll, yaw, pitch;
	double diff_roll;
	int fl, fr, bl, br;
	static int throttle = MOTOR_START+4;
	readMotion(&roll, &yaw, &pitch);
	diff_roll = pid_roll(1.0, 0, 0, roll, 0);

	diff_roll = map(diff_roll, -4000, 4000, -60, 60);
	//Serial.println(diff_roll, DEC);

	fl = throttle + diff_roll;
	fr = throttle - diff_roll;
	bl = throttle + diff_roll;
	br = throttle - diff_roll;
	set_motors(fl, fr, bl, br);
#ifdef DEBUG
	Serial.print(fl, DEC);
	Serial.print(" ");
	Serial.print(fr, DEC);
	Serial.print(" ");
	Serial.print(bl, DEC);
	Serial.print(" ");
	Serial.println(br, DEC);
#endif

	//sendCommand();
  delay(100);
}
