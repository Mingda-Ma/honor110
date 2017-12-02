#include <Arduino.h>
#include <TinyGPS++.h>
#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>
#include <sms.h>
#include <gps.h>
#include <avr/sleep.h>

SMSRec *smsrec;
GPS *gps;

//SoftwareSerial ss(RX_PIN, TX_PIN);

void setup() {
    //while (!Serial);
    Serial.begin(9600);
    smsrec = new SMSRec();
    gps = new GPS();
    if (!smsrec->init()) {
        Serial.println("SMSRec init failed!");
        while (1);
    }


    //Serial.println("SMSRec init success!");
}

void loop() {
    smsrec->listen(gps);
    if (gps->isValid())
        digitalWrite(GPS_LED, HIGH);
    else
        digitalWrite(GPS_LED, LOW);
}
