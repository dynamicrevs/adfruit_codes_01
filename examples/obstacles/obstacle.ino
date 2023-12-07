#include <Servo.h>

Servo myservo;  // Create servo object to control a servo motor

const int trigPin = A1;  // Ultrasonic sensor trigger pin
const int echoPin = A0;  // Ultrasonic sensor echo pin

const int motor1A = 6;  // L298 Motor Driver Pins
const int motor1B = 5;
const int motor1Enable = 7;  // Enable pin for motor 1
const int motor2A = 4;
const int motor2B = 3;
const int motor2Enable = 2;  // Enable pin for motor 2

const int forwardSpeed = 80;  // Adjust the speed as needed
const int backwardSpeed = 50;  // Adjust the speed as needed

void setup() {
  myservo.attach(8);  // Servo control pin
  Serial.begin(9600);  // Initialize Serial Monitor with baud rate 9600
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor1Enable, OUTPUT);  // Enable pin for motor 1
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(motor2Enable, OUTPUT);  // Enable pin for motor 2
}

void loop() {
  int distance = getDistance();

  if (distance < 40) {
    obstacleDetected();
  } else {
    moveForward();
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.0343 / 2; // Speed of sound is 343 meters/second

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

void stopMotors() {
  analogWrite(motor1Enable, 0);  // Stop motor 1
  analogWrite(motor2Enable, 0);  // Stop motor 2
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
}

void activateServo() {
  myservo.write(90);  // Move the servo to the center position (or any desired position)
}

void stopServo() {
  myservo.write(90);
}

void moveForward() {
  analogWrite(motor1Enable, forwardSpeed);
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);

  analogWrite(motor2Enable, forwardSpeed);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void moveBackward() {
  analogWrite(motor1Enable, backwardSpeed);
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);

  analogWrite(motor2Enable, backwardSpeed);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void obstacleDetected() {
  stopMotors();
  delay(100);
  activateServo();
  int rightDistance = getSideDistance(0);

  if (rightDistance > 40) {
    // There is enough space on the right side
    stopServo();
    moveForward();
  } else {
    int leftDistance = getSideDistance(180);

    if (leftDistance > 40) {
      // There is enough space on the left side
      stopServo();
      moveForward();
    } else {
      moveBackward();
      delay(500);  // Adjust the delay time for backward motion
      stopMotors();
      randomRotate();
      stopServo();
      delay(600);  // Adjust the delay time for very short rotation
      stopServo();
      moveForward();
    }
  }
}

int getSideDistance(int angle) {
  myservo.write(angle);
  delay(100);  // Allow time for the servo to move
  return getDistance();
}

void randomRotate() {
  int randomRotate = random(2);  // Randomly choose between 0 and 1

  if (randomRotate == 0) {
    rotateLeftFull();
  } else {
    rotateRightFull();
  }

  delay(600);  // Adjust the delay time for very short rotation
  stopMotors();
}

void rotateLeftFull() {
  analogWrite(motor1Enable, forwardSpeed);
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);

  analogWrite(motor2Enable, forwardSpeed);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void rotateRightFull() {
  analogWrite(motor1Enable, forwardSpeed);
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);

  analogWrite(motor2Enable, forwardSpeed);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}
