/*
Project: LiFiGuard - Vehicle Communication and Safety System
Module: Transmitter (Arduino)


Sensors Used:
- HC-SR04 Ultrasonic Sensor  -> Detects obstacle distance
- SW-420 Vibration Sensor    -> Detects collision or strong vibration
- MQ-3 Gas Sensor            -> Detects alcohol presence
- Push Buttons (4)           -> Manual alert messages
*/

// ---------------- Pin Definitions ----------------
const int ultrasonicTrigPin = 2;
const int ultrasonicEchoPin = 3;
const int vibrationSensorPin = 4;
const int mq3SensorPin = A0;

const int button1Pin = 5;
const int button2Pin = 6;
const int button3Pin = 7;
const int button4Pin = 8;

const int lifiTxPin = 9;   // LiFi LED transmitter

// ---------------- Constants ----------------
const unsigned long ultrasonicTimeout = 40000;
const int lifiDelay = 10;
const int debounceDelay = 50;

const int OBSTACLE_THRESHOLD = 30;
const int MQ3_THRESHOLD = 300;

// ---------------- Variables ----------------
long duration, distance;
int vibrationState = 0;
int mq3Value = 0;

int button1State = 1;
int button2State = 1;
int button3State = 1;
int button4State = 1;

unsigned long lastDebounceTime = 0;

// ---------------- Setup ----------------
void setup() {

  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);

  pinMode(vibrationSensorPin, INPUT);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);

  pinMode(lifiTxPin, OUTPUT);

  Serial.begin(9600);
  delay(100);
}

// ---------------- Main Loop ----------------
void loop() {

  // -------- Ultrasonic Sensor --------
  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(ultrasonicTrigPin, LOW);

  duration = pulseIn(ultrasonicEchoPin, HIGH, ultrasonicTimeout);

  if (duration == 0)
    distance = -1;
  else
    distance = duration * 0.034 / 2;

  // -------- Vibration Sensor --------
  vibrationState = digitalRead(vibrationSensorPin);

  // -------- MQ3 Gas Sensor --------
  mq3Value = analogRead(mq3SensorPin);

  if (mq3Value < 0 || mq3Value > 1023)
    mq3Value = -1;

  // -------- Push Buttons --------
  button1State = debounceButton(button1Pin);
  button2State = debounceButton(button2Pin);
  button3State = debounceButton(button3Pin);
  button4State = debounceButton(button4Pin);

  // -------- Prepare Data String --------
  String data = "<D:" + String(distance) +
                ",V:" + String(vibrationState) +
                ",M:" + String(mq3Value);

  if (button1State == LOW) data += ",B1:1";
  if (button2State == LOW) data += ",B2:1";
  if (button3State == LOW) data += ",B3:1";
  if (button4State == LOW) data += ",B4:1";

  data += ">";

  // -------- Send Data via LiFi --------
  sendDataViaLiFi(data);

  Serial.println("Transmitted: " + data);

  delay(500);
}

// ---------------- LiFi Transmission Function ----------------
void sendDataViaLiFi(String data) {

  for (int i = 0; i < data.length(); i++) {

    char c = data[i];

    for (int j = 7; j >= 0; j--) {

      digitalWrite(lifiTxPin, bitRead(c, j));
      delay(lifiDelay);

    }
  }

  digitalWrite(lifiTxPin, LOW);
}

// ---------------- Button Debounce Function ----------------
int debounceButton(int pin) {

  int state = digitalRead(pin);

  if (state == LOW && millis() - lastDebounceTime > debounceDelay) {
    lastDebounceTime = millis();
    return LOW;
  }

  return HIGH;
}

