#include <Arduino.h>
#include <TinyGPS++.h>
#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>
#include <sms.h>
#include <gps.h>
#include <avr/sleep.h>

SMSRec *smsrec;
GPS *gps;

void setup() {
    //while (!Serial);
    smsrec = new SMSRec();
    gps = new GPS();
    if (!smsrec->init()) {
        Serial.println("SMSRec init failed!");
        while (1);
    }
}

void loop() {
    smsrec->listen(gps);
    gps->smartDelay(200);
    if (gps->isValid())
        digitalWrite(GPS_LED, HIGH);
    else
        digitalWrite(GPS_LED, LOW);
}
