#include <Servo.h>
#define numChars 32   //max number of chars for each line of serial input

Servo pitchServo;  
Servo rollServo;

int startY = 75;  //starting pitch position for the servo
int startX = 50;  //starting bank position for the servo

char receivedChars[numChars]; //variables for reading serial
boolean newData = false;

//code for serial input sourced from https://forum.arduino.cc/t/serial-input-basics/278284
void recvWithEndMarker() {  //receive input from serial, which comes from flightgear
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
 
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read(); //read byte from serial
    
    if (rc != endMarker) {  
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
      }
    }
}

void showNewData() {  //show and analyze the new data read from serial
  if (newData == true) {
    char valStr[numChars];  //extract value from the serial line: 2 possible formats: P[value], R[value]
    strncpy(valStr, &receivedChars[1], sizeof(receivedChars));
    float valFloat = atof(valStr);
    long valInt = round(valFloat);
    valInt = map(valInt, -100, 100, -110, 110);

    if (receivedChars[0] == 'P') {  //if first char is P, then the value is for pitch
      pitchServo.write(startY - valInt);
    } else if (receivedChars[0] == 'R') { //else if first char is R, then value is for roll
      rollServo.write(startX - valInt);
    }
    newData = false;
  }
}

void setup() {
  Serial.begin(9600);
  pitchServo.attach(9);  // attaches the pitch servo on pin 9 
  rollServo.attach(10); // attaches the roll servo on pin 10 
  
  pitchServo.write(startY);
  rollServo.write(startX);
}


void loop() {
  recvWithEndMarker();
  showNewData();
}
