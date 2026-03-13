/*
Project: LiFiGuard - Vehicle Communication and Safety System
Module: Receiver (Arduino)

The received information may contain:
- Obstacle distance alerts
- Collision detection alerts
- Gas/alcohol detection warnings
- Manual driver messages (Help, Slow, Stop, All Clear)

The decoded messages are displayed on a 16x2 LCD screen and a beeper
is triggered for critical alerts.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int solarPanelPin = A1;
const int beeperPin = 10;

const int threshold = 512;

void setup() {

    Serial.begin(9600);

    lcd.init();
    lcd.backlight();

    pinMode(beeperPin, OUTPUT);
    digitalWrite(beeperPin, LOW);
}

void loop() {

    String receivedData = receiveDataFromLiFi();

    Serial.println("Received: " + receivedData);

    processReceivedData(receivedData);

    delay(500);
}

String receiveDataFromLiFi() {

    String data = "";
    bool started = false;

    while (true) {

        int lightIntensity = analogRead(solarPanelPin);

        if (lightIntensity > threshold) {

            if (!started) {
                started = true;
                data = "";
            }

            data += "1";

        } else {

            data += "0";

        }

        if (data.endsWith("11111111")) break;

        delay(10);
    }

    return binaryToText(data);
}

String binaryToText(String binary) {

    String text = "";

    for (int i = 0; i < binary.length(); i += 8) {

        int val = 0;

        for (int j = 0; j < 8; j++) {

            val = (val << 1) | (binary[i + j] - '0');

        }

        text += char(val);
    }

    return text;
}

void processReceivedData(String data) {

    lcd.clear();

    if (data.indexOf("D:") != -1) {

        int startIndex = data.indexOf("D:") + 2;
        int endIndex = data.indexOf(",", startIndex);

        if (endIndex == -1) endIndex = data.length();

        int distance = data.substring(startIndex, endIndex).toInt();

        if (distance > 0 && distance < 30) {

            lcd.setCursor(0, 0);
            lcd.print("Obstacle: ");
            lcd.print(distance);
            lcd.print("cm!");

            alertBeeper();
            return;
        }
    }

    if (data.indexOf("V:1") != -1) {

        lcd.setCursor(0, 0);
        lcd.print("Collision Detected!");

        lcd.setCursor(0, 1);
        lcd.print("Check Vehicle!");

        alertBeeper();
        return;
    }

    if (data.indexOf("M:") != -1) {

        int startIndex = data.indexOf("M:") + 2;
        int endIndex = data.indexOf(",", startIndex);

        if (endIndex == -1) endIndex = data.length();

        int mq3Value = data.substring(startIndex, endIndex).toInt();

        if (mq3Value > 300) {

            lcd.setCursor(0, 0);
            lcd.print("Gas Leak Detected!");

            lcd.setCursor(0, 1);
            lcd.print("Evacuate!");

            alertBeeper();
            return;
        }
    }

    if (data.indexOf("B1:1") != -1) {

        lcd.setCursor(0, 0);
        lcd.print("Help!");

        lcd.setCursor(0, 1);
        lcd.print("Need Assistance!");

        alertBeeper();
        return;
    }

    if (data.indexOf("B2:1") != -1) {

        lcd.setCursor(0, 0);
        lcd.print("Slow!");

        lcd.setCursor(0, 1);
        lcd.print("Traffic Ahead!");

        return;
    }

    if (data.indexOf("B3:1") != -1) {

        lcd.setCursor(0, 0);
        lcd.print("Stop!");

        lcd.setCursor(0, 1);
        lcd.print("Emergency!");

        alertBeeper();
        return;
    }

    if (data.indexOf("B4:1") != -1) {

        lcd.setCursor(0, 0);
        lcd.print("All Clear!");

        return;
    }
}

void alertBeeper() {

    digitalWrite(beeperPin, HIGH);
    delay(500);
    digitalWrite(beeperPin, LOW);
}
