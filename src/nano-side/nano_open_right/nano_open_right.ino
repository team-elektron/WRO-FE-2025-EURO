//#include <SoftPWM_timer.h>
#include <SoftPWM.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <HCSR04.h>
#include <Servo.h>

Servo steering;
const int servoPin = 9;

// calibration
const int centerPos = 90;
const int leftPos   = 50;
const int rightPos  = 130;

int gomode = 0;
int str = 0;

HCSR04 left(2, 3);
HCSR04 front(4, 5);
HCSR04 right(6, 7);

Adafruit_MPU6050 mpu;

// Global variables
float yaw = 0.0;          // current yaw angle (degrees)
unsigned long lastTime;   // for delta time

int mot_ena = 11; //Motor Enable/Speed
int mot_in1 = 10; //Motor IN1
int mot_in2 = 12; //Motor IN2

void setup() {
  Serial.begin(115200);

  // Initialize
  //SoftPWMBegin();

  // Create and set pin 13 to 0 (off)
  //SoftPWMSet(10, 0);

  // Set fade time for pin 13 to 100 ms fade-up time, and 500 ms fade-down time
  //SoftPWMSetFadeTime(10, 100, 100);

  steering.attach(servoPin);
  centerSteering();

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  lastTime = millis();


}

// ---- Update yaw continuously ----
void updateYaw() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate elapsed time (s)
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // g.gyro.z is in rad/s → convert to deg/s
  float gyroZ = g.gyro.z * 57.2958;

  // integrate yaw
  yaw += gyroZ * dt;

  // keep yaw in range [-180, 180]
  if (yaw > 180) yaw -= 360;
  if (yaw < -180) yaw += 360;
}

// ---- Function you can call anywhere to get yaw ----
float getYaw() {
  updateYaw();
  return yaw;
}

void turnLeft(int percentage) {
  percentage = constrain(percentage, 0, 100);
  int angle = map(percentage, 0, 100, centerPos, leftPos);
  steering.write(angle);
}

void turnRight(int percentage) {
  percentage = constrain(percentage, 0, 100);
  int angle = map(percentage, 0, 100, centerPos, rightPos);
  steering.write(angle);
}

void centerSteering() {
  steering.write(centerPos);
}

void resetYaw() {
  yaw = 0.0;
}

void wallFollow() {
  while (true) {
  if (front.dist() >= 75 || front.dist() == 0) {
    delay(10);
    if (right.dist() >= 25) {
      turnRight(40);
      delay(100);
      analogWrite(mot_ena, 60);
      digitalWrite(mot_in1, HIGH);   // Start forward
      digitalWrite(mot_in2, LOW); //
    } else {
      turnLeft(40);
      delay(100);
      analogWrite(mot_ena, 50);
      digitalWrite(mot_in1, HIGH);   // Start forward
      digitalWrite(mot_in2, LOW); //
    }
  } else {
      centerSteering();
      analogWrite(mot_ena, 60);
      digitalWrite(mot_in1, LOW);   // Stop
      digitalWrite(mot_in2, HIGH); //
      delay(50);
      //SoftPWMSetPercent(mot_ena, 0);    //
      analogWrite(mot_ena, 0);
      digitalWrite(mot_in1, LOW);   // Stop
      digitalWrite(mot_in2, LOW); //
      delay(1000);
      break;
    
  } 
}
}

void leftTurn() {
  while (true) {
    float currentYaw = getYaw();
    delay(50);
    Serial.println(currentYaw);
    if (currentYaw <=80) {
      turnLeft(100);
      analogWrite(mot_ena, 65);
      digitalWrite(mot_in1, HIGH);   // Start forward
      digitalWrite(mot_in2, LOW); //
    } else {
      centerSteering();
      analogWrite(mot_ena, 0);
      digitalWrite(mot_in1, LOW);
      break;
    }
  }
}

void loop() {
  
  float currentYaw = getYaw();
  
  Serial.print("Yaw: ");
  Serial.println(currentYaw);
  Serial.print("Ultrasonic left: ");
  Serial.println(left.dist());
  delay(60);
  Serial.print("Ultrasonic righ: ");
  Serial.println(right.dist());
  delay(60);
  Serial.print("Ultrasonic front: ");
  Serial.println(front.dist());
  delay(60);

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'r') {
      yaw = 0;
      Serial.println("Yaw reset!");
    }
    if (c == 'o') {
      wallFollow();
      delay(1000);
      resetYaw();
      leftTurn();
    }
  }



}
