#include <Arduino.h>
#include <TinyGPS++.h>
#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>
#include <sms.h>
#include <avr/sleep.h>

SMSRec *smsrec;

void setup() {
    //while (!Serial);
    smsrec = new SMSRec();
    if (!smsrec->init()) {
        Serial.println("SMSRec init failed!");
        while (1);
    }
}

void loop() {
    smsrec->getMsg();
    delay(10);
}
