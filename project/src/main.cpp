#include <Arduino.h>
#include <TinyGPS++.h>
#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>

#define FONA_RX  9
#define FONA_TX  8
#define FONA_RST 4
#define FONA_RI  7

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
uint8_t type;

void setup() {
    while (!Serial);

    Serial.begin(115200);
    Serial.println(F("FONA basic test"));
    Serial.println(F("Initializing....(May take 3 seconds)"));

    fonaSerial->begin(4800);
    if (! fona.begin(*fonaSerial)) {
        Serial.println(F("Couldn't find FONA"));
        while (1);
    }
    type = fona.type();
    Serial.println(F("FONA is OK"));
    Serial.print(F("Found "));
    switch (type) {
    case FONA800L:
        Serial.println(F("FONA 800L")); break;
    case FONA800H:
        Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
        Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
        Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
        Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
        Serial.println(F("FONA 3G (European)")); break;
    default:
        Serial.println(F("???")); break;
    }

    // Print module IMEI number.
    char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
    uint8_t imeiLen = fona.getIMEI(imei);
    if (imeiLen > 0) {
      Serial.print("Module IMEI: "); Serial.println(imei);
    }
}

void loop() {
    if (fona.available()) {
        Serial.write(fona.read());
    }
}

void flushSerial() {
    while (Serial.available())
        Serial.read();
}

char readBlocking() {
    while (!Serial.available());
    return Serial.read();
}

uint16_t readnumber() {
    uint16_t x = 0;
    char c;
    while (! isdigit(c = readBlocking())) {
    //Serial.print(c);
    }
    Serial.print(c);
    x = c - '0';
    while (isdigit(c = readBlocking())) {
        Serial.print(c);
        x *= 10;
        x += c - '0';
    }
    return x;
}

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
    uint16_t buffidx = 0;
    boolean timeoutvalid = true;
    if (timeout == 0) timeoutvalid = false;

    while (true) {
        if (buffidx > maxbuff) {
            //Serial.println(F("SPACE"));
            break;
        }

        while (Serial.available()) {
            char c =  Serial.read();

            //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

            if (c == '\r') continue;
            if (c == 0xA) {
                if (buffidx == 0)   // the first 0x0A is ignored
                    continue;

                timeout = 0;         // the second 0x0A is the end of the line
                timeoutvalid = true;
                break;
            }
            buff[buffidx] = c;
            buffidx++;
        }

        if (timeoutvalid && timeout == 0) {
            //Serial.println(F("TIMEOUT"));
            break;
        }
        delay(1);
    }
    buff[buffidx] = 0;  // null term
    return buffidx;
}
