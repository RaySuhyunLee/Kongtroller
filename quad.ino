#include <Servo.h>

char wr;
int isread = 1;

const int FRONT_LEFT_PIN = 5;
const int FRONT_RIGHT_PIN = 4;
const int BACK_LEFT_PIN = 2;
const int BACK_RIGHT_PIN = 3;

Servo front_left;
Servo front_right;
Servo back_left;
Servo back_right;

void init_motors() {
	front_left.attach(FRONT_LEFT_PIN);
	front_right.attach(FRONT_RIGHT_PIN);
	back_left.attach(BACK_LEFT_PIN);
	back_right.attach(BACK_RIGHT_PIN);
}

// the setup routine runs once when you press reset:
void setup() {
	init_motors();
	
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("start serial");
  Serial3.begin(115200);
}

void set_motors(int fl, int fr, int bl, int br) {
	front_left.write(fl);
	front_right.write(fr);
	back_left.write(bl);
	back_right.write(br);
}

void readMotion(int* roll, int* pitch, int* yaw) {
	if (Serial3.available() > 0) {
		char buffer[8];
    short roll, pitch, yaw;
    
    while(1) {
      Serial3.readBytes(buffer, 2);
      if (buffer[0] == 0x55 && buffer[1] == 0x55) {
        Serial.print("start. ");
        break;
      }
    }
    
    Serial3.readBytes(buffer, 8);
    roll = buffer[0] * 0x100 + buffer[1];
    pitch = buffer[2] * 0x100 + buffer[3];
    yaw = buffer[4] * 0x100 + buffer[5];
    
    Serial.print(roll, DEC);
    Serial.print(' ');
    Serial.print(pitch, DEC);
    Serial.print(' ');
    Serial.println(yaw, DEC);
  }
}

void pid_roll(double p, double i, double d) {
	int out_front, out_back;
	int roll, pitch, yaw;

	readMotion(&roll, &pitch, &yaw);
}

void sendCommand() {
	/* give signal to motion sensor */
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


// the loop routine runs over and over again forever:
void loop() {
	readMotion();

	sendCommand();
  delay(1);
}
