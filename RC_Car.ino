// RC Car controlled via an IR remote. It automatically avoids obstacles 
// For Physical Interface Design course Medialogy BSc. semester 4
// By Christian Tsalidis

#include <Servo.h>
#include <IRremote.h>
#include "pins_arduino.h"

// IR
const int IR_RECV_PIN = 5; // Pin should be PWM
IRrecv irrecv(IR_RECV_PIN);
decode_results results; // results we get from the ir remote

// pins for the first DC motor
const int EN_A = 11;
const int INPUT_1 = 8;
const int INPUT_2 = 12;

// pins for the second DC motor
const int EN_B = 3;
const int INPUT_3 = 2;
const int INPUT_4 = 4;

// pins for the ultrasonic sensor
const int ULTRASONIC_TRIGGER_PIN = 7;
const int ULTRASONIC_ECHO_PIN = 7;
int cmDistance = 0;

// define servo
const int SERVO_PIN = 13;
Servo servo;
int servoAngle = 90;

// led pin
const int LED_PIN = 6;

bool checkLeft = true;
bool isAvoidingObstacles = false;
bool ledPinIsOn = false;

void setup() {
  Serial.begin(9600); // to print in the serial monitor
  
  // setting the dc motors pins as output
  // DC motor 1
  pinMode(EN_A, OUTPUT);
  pinMode(INPUT_1, OUTPUT);
  pinMode(INPUT_2, OUTPUT);
  // DC motor 2
  pinMode(EN_B, OUTPUT);
  pinMode(INPUT_3, OUTPUT);
  pinMode(INPUT_4, OUTPUT);
  
  // Send signal to L298N Enable pins
  digitalWrite(EN_A, HIGH);
  digitalWrite(EN_B, HIGH);
  
  // Initialization of servo motor
  servo.attach(SERVO_PIN);
  servo.write(servoAngle);
  
  // the led
  pinMode(LED_PIN, OUTPUT);
  
  // setup the ir remote control and ir sensor
  irrecv.enableIRIn(); // Start the ir receiver
  
  CarIsReady();
}

void loop() {
  GetUltrasonicDistance();
  CheckIRData();
  if(cmDistance < 10) {
    isAvoidingObstacles = true;
    MoveCar(5); // stop the car
    AvoidObstacle(); // avoid obstacle
  }
  else if(isAvoidingObstacles) {
    isAvoidingObstacles = false;
    MoveCar(2); // forward
  }
}

void CheckIRData() {
  // 4 --> Left --> IR Code --> 16591063 
  // 6 --> Right --> IR Code --> 16607383 
  // 8 --> Back --> IR Code --> 16619623 
  // 2 --> Forward --> IR Code --> 16615543 
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    switch(results.value) {
      case 16591063: MoveCar(4); break; // left
      case 16607383: MoveCar(6); break; // right
      case 16619623: MoveCar(8); break; // back
      case 16615543: MoveCar(2); break; // forward
      case 16623703: MoveCar(5); break; // stop
      case 16582903: RotateLight(1); break; // rotate light to the left
      case 16599223: RotateLight(3); // rotate light to the right
      case 16593103: checkLeft = !checkLeft; break; // button 0 in IR remote --> ChangeObstacleAvoidingDirection
      case 16603303: {   ledPinIsOn = !ledPinIsOn; // button 9 in IR remote --> turn on / off led light
                        if(ledPinIsOn) digitalWrite(LED_PIN, HIGH);
                        else digitalWrite(LED_PIN, LOW); };
      default: break;
    }
    irrecv.resume(); // Continue receiving
  }
}

void GetUltrasonicDistance() {
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);  // Clear the trigger
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  long soundWaveTravel = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  
  cmDistance = 0.01723 * soundWaveTravel;
  // Serial.print(cmDistance);
  // Serial.println("cm");
  delay(100); // Wait for 100 millisecond(s)
}

void MoveCar(int direction) {
  // control the DC motors
  if(direction == 4) { // left
    // Set the default rotation direction of the first DC motor
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);
    // Set the default rotation direction of the second DC motor
    digitalWrite(INPUT_3, LOW);
    digitalWrite(INPUT_4, HIGH);
  }
  else if(direction == 6) { // right
    // Set the default rotation direction of the first DC motor
    digitalWrite(INPUT_1, HIGH);
    digitalWrite(INPUT_2, LOW);
    // Set the default rotation direction of the second DC motor
    digitalWrite(INPUT_3, HIGH);
    digitalWrite(INPUT_4, LOW);
  }
  else if(direction == 8) { // back
    // Set the default rotation direction of the first DC motor
    digitalWrite(INPUT_1, HIGH);
    digitalWrite(INPUT_2, LOW);
    // Set the default rotation direction of the second DC motor
    digitalWrite(INPUT_3, LOW);
    digitalWrite(INPUT_4, HIGH);
  }
  else if(direction == 2) { // forward
    // Set the default rotation direction of the first DC motor
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);
    // Set the default rotation direction of the second DC motor
    digitalWrite(INPUT_3, HIGH);
    digitalWrite(INPUT_4, LOW);
  }
  else if(direction == 5) { // stop
    // Set the default rotation direction of the first DC motor
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, LOW);
    // Set the default rotation direction of the second DC motor
    digitalWrite(INPUT_3, LOW);
    digitalWrite(INPUT_4, LOW);
  }
  
  delay(50); // Wait for 50 millisecond(s)
}

void CarIsReady() {
  // blink the LED
  for(int i = 0; i < 3; i++) {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    // turn the LED off by making the voltage LOW
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void AvoidObstacle() {
  if(!isAvoidingObstacles) return;
  if(checkLeft) MoveCar(4); // left
  else MoveCar(6); // right
}

// the led light is attached on top of the servo motor, pointing straight to where the servo motor is pointing
void RotateLight(int val) {
  ledPinIsOn = true;
  digitalWrite(LED_PIN, HIGH);
  // the servoAngle is changed depending if the user clicks on number 1 or 3 (left and right respectively)
  // control the servo motor
  if(val == 1 && servoAngle >= 45) servoAngle -= 45;
  else if(val == 3 && servoAngle <= 135) servoAngle += 45;
  servo.write(servoAngle);
  delay(20);
}
