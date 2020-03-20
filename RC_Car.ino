
#include <Servo.h>

Servo servo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int potVal;    // variable to read the value from the analog pin

void setup() {
  Serial.begin(9600);
  
  // set up the servo
  servo.attach(9);  // attaches the servo on pin 9 to the servo object

  // initialize the input pins for bridge H --> for controlling the two DC motors
  
  
}

void loop() {
  
}


void controlServo() {
  potVal = analogRead(potpin); // reads the value of the potentiometer (value between 0 and 1023)
  // Serial.println(potVal); 
  potVal = map(potVal, 0, 1023, 0, 180); // scale it to use it with the servo (value between 0 and 180)
  servo.write(potVal); // sets the servo position according to the scaled value
  delay(15); // waits for the servo to get there  
}
