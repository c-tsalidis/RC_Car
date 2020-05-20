
// left motor
const int EN_A = 6;
const int IN_1 = 7;
const int IN_2 = 8;
int motorSpeed = 210;

void setup() {
  // set the pins to output
  pinMode(EN_A, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);

  // set the rotation of the DC motor to counter lockwise
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);

  // set the speed of the motor
  analogWrite(EN_A, motorSpeed);
}
