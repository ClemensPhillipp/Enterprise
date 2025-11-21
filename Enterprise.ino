#include <Servo.h>

Servo enterpriseServo;

// --- Configuration ---
const int servoPin = 9;
const int startPos = 90;
const int topPos = 0;

const int stepDelay = 50;
const int holdTimeTop = 5000;
const int holdTimeBottom = 2000;

const int IN1 = 5;
const int IN2 = 6;
const int GSM1 = 11;

const int baseSpeed = 100;
const int maxSpeed = 255;

int servoHalfPos = 45;

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(GSM1, OUTPUT);

  enterpriseServo.attach(servoPin);
  enterpriseServo.write(startPos);

  delay(holdTimeBottom);
}


// ----------------------------------------------------------
// Motorfunktionen
// ----------------------------------------------------------
void motorSetDirectionForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void motorSetDirectionStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void motorRampUp(int fromSpeed, int toSpeed, int step = 1) {
  motorSetDirectionForward();

  for (int s = fromSpeed; s <= toSpeed; s += step) {
    analogWrite(GSM1, s);
    Serial.print("MS UP: ");
    Serial.println(s);
    delay(30);
  }
}

void motorRampDown(int fromSpeed, int toSpeed, int step = 1) {
  for (int s = fromSpeed; s >= toSpeed; s -= step) {
    analogWrite(GSM1, s);
    Serial.print("MS DOWN: ");
    Serial.println(s);
    delay(30);
  }

  if (toSpeed == 0) motorSetDirectionStop();
}


// ----------------------------------------------------------
// Servo Bewegen
// ----------------------------------------------------------
void moveServo(int from, int to) {
  if (from < to) {
    for (int p = from; p <= to; p++) {
      enterpriseServo.write(p);
      Serial.print("POS: ");
      Serial.println(p);
      delay(stepDelay);
    }
  } else {
    for (int p = from; p >= to; p--) {
      enterpriseServo.write(p);
      Serial.print("POS: ");
      Serial.println(p);
      delay(stepDelay);
    }
  }
}


// ----------------------------------------------------------
// LOOP
// ----------------------------------------------------------
void loop() {

  // ====== STEIGEN ===================================================
  Serial.println("UP: Phase 1 – Motor auf 100");
  motorRampUp(0, baseSpeed);

  Serial.println("UP: Phase 2 – Servo 25% nach oben");
  moveServo(startPos, startPos - servoHalfPos);

  Serial.println("UP: Phase 3 – Motor auf MAX");
  motorRampUp(baseSpeed, maxSpeed);

  Serial.println("UP: Phase 4 – Servo auf TOP");
  moveServo(startPos - servoHalfPos, topPos);

  delay(holdTimeTop);


  // ====== SINKEN ====================================================


  Serial.println("DOWN: Phase 2 – Servo 25% runter");
  moveServo(topPos, topPos + servoHalfPos);

  Serial.println("DOWN: Phase 1 – Motor auf 100");
  motorRampDown(maxSpeed, baseSpeed);

  Serial.println("DOWN: Phase 4 – Servo nach unten");
  moveServo(topPos + servoHalfPos, startPos);

  motorRampDown(baseSpeed, 0);
  delay(holdTimeBottom);
}
