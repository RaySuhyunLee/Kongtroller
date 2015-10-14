#include <Servo.h>

const int MOTOR_READY = 20;
const int MOTOR_START = 24;		// motor starts to turn when given this value.
const int MOTOR_MAX = 60;			// for safety
const int FRONT_LEFT_PIN = 5;
const int FRONT_RIGHT_PIN = 4;
const int INTERVAL = 20;

int isread = 1;

Servo front_left;
Servo front_right;

void init_motors() {
	front_left.attach(FRONT_LEFT_PIN);
	front_right.attach(FRONT_RIGHT_PIN);
	set_motors(MOTOR_READY, MOTOR_READY);
}

// the setup routine runs once when you press reset:
void setup() {
	init_motors();
	
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start serial");
	Serial.println("enter s to start");
	while(Serial.read() != 's');
	Serial.println("Starting in 3 seconds");
	delay(1000);
	Serial.println("2 seconds");
	delay(1000);
	Serial.println("1 second");
	delay(1000);
	Serial.println("Start!");
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

void set_motors(int fl, int fr) {
	front_left.write(filter_bound(fl));
	front_right.write(filter_bound(fr));
}

void readMotion(double* roll, double* pitch, double* yaw) {
	char buffer[100];
	char *split;
	int start=0, i;

	while(!Serial3.available());
	// read data
	for (i=0; i<100; i++) {
		//Serial3.readBytes(&buffer[i], 1);
		buffer[i] = Serial3.read();
		//Serial.println(buffer[i], HEX);
		if (buffer[i] == '*')
			break;
	}
	Serial.print(start, DEC);
	Serial.print(", ");
	Serial.print(i, DEC);
	Serial.print(", ");
	buffer[i-1] = '\0';
	Serial.println(buffer);
	split = strtok(&buffer[start+1], ",");
	*roll = atof(split);
	split = strtok(NULL, ",");
	*pitch = atof(split);
	split = strtok(NULL, ",");
	*yaw = atof(split);
	/*
	Serial.print("read: ");
	Serial.print(*roll, DEC);
	Serial.print(' ');
	Serial.print(*pitch, DEC);
	Serial.print(' ');
	Serial.println(*yaw, DEC);*/
}

double pid_pitch(double p, double i, double d, double pitch, double desired) {
	int out_left, out_right;
	short in_diff;
	double out_diff;
	static short prev_pitch = pitch;
	double differential;

	differential = (pitch - prev_pitch) / (INTERVAL / 1000);
	prev_pitch = pitch;
	in_diff = desired - pitch;
	out_diff = in_diff * p - differential * d;
	/*Serial.print(in_diff, DEC);
	Serial.print(", ");
	Serial.println(differential, DEC);*/
	return out_diff;
}

void sendCommand() {
	/* give signal to motion sensor */
	char wr;
  while (Serial.available() > 0) {
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


// the loop routine runs over and over again forever:
void loop() {
	double roll, yaw, pitch;
	double diff_pitch;
	int fl, fr;
	static int throttle = MOTOR_START+20;
	readMotion(&roll, &pitch, &yaw);
	//diff_pitch = pid_pitch(0.6, 0, 100, pitch, 0);

	//diff_pitch = map(diff_pitch, -4000, 4000, -18, 18);
	//Serial.println(diff_pitch, DEC);

	fl = throttle + diff_pitch;
	fr = throttle - diff_pitch;
	//set_motors(fl, fr);

	/*
	Serial.print(fl, DEC);
	Serial.print(" ");
	Serial.println(fr, DEC);
	*/

	/*if(Serial.available()) {
		throttle = Serial.parseInt();
	}*/

	sendCommand();
  delay(10);
}
