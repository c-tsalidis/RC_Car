// RC Car 
// For Physical Interface Design course Medialogy BSc. semester 4
// By Christian Tsalidis

#include <Servo.h>

Servo servo; // create servo object to control a servo

int potpin = 0; // analog pin used to connect the potentiometer
int potVal; // variable to read the value from the analog pin

const int numberMotors = 2; // amount of motors the circuit has
const int leftMotor[numberMotors] = {2, 3};
const int rightMotor[numberMotors] = {4, 5};
const int delayTime = 2000;

void setup() {
  Serial.begin(9600);
  
  // set up the servo
  servo.attach(9);  // attaches the servo on pin 9 to the servo object

  // initialize the input pins for bridge H --> for controlling the two DC motors
  for(int i = 0; i < numberMotors; i++) {
    pinMode(leftMotor[i], OUTPUT);
    pinMode(rightMotor[i], OUTPUT);
  }
  stopCar();
}

void loop() {
  controlServo(); // update the servo based on potentiometer analog values
  controlMotors();
}

void controlServo() {
  potVal = analogRead(potpin); // reads the value of the potentiometer (value between 0 and 1023)
  Serial.println(potVal); 
  potVal = map(potVal, 0, 1023, 0, 180); // scale it to use it with the servo (value between 0 and 180)
  servo.write(potVal); // sets the servo position according to the scaled value
  delay(15); // waits for the servo to get there  
}


void controlMotors() {
  advance(leftMotor);
  // delay(delayTime);
  advance(rightMotor);
  // delay(delayTime);
  
  backwards(leftMotor);
  // delay(delayTime);
  backwards(rightMotor);
  // delay(delayTime);

  stopMotor(leftMotor);
  // delay(delayTime);
  stopMotor(rightMotor);
  // delay(delayTime);
}

void advance(int motor[]) {
  digitalWrite(motor[0], HIGH);
  digitalWrite(motor[1], LOW);
}

void backwards(int motor[]) {
  digitalWrite(motor[0], LOW);
  digitalWrite(motor[1], HIGH);
}


void stopCar() {
  for(int i = 0; i < numberMotors * numberMotors; i++) {
    if(i < numberMotors) stopMotor(leftMotor);
    else stopMotor(rightMotor);
  }
}

void stopMotor(int motor[]) {
  int motorArraySize = (sizeof(motor) / sizeof(motor[0]));
  for(int i = 0; i < motorArraySize; i++) {
    digitalWrite(motor[i], LOW);
  }
}
