#include "gps.h"
#include <TinyGPS++.h>

GPS::GPS() {
    gps = new TinyGPSPlus();
    gpsSS = new SoftwareSerial(RX_PIN, TX_PIN);
    gpsSS->begin(GPS_BAUD);
    pinMode(GPS_LED, OUTPUT);
}

void GPS::smartDelay(unsigned long ms) {
    gpsSS->listen();
    unsigned long st = millis();
    while (millis() - st < ms) {
        while (gpsSS->available()) {
            gps->encode(gpsSS->read());
        }
    }
}

bool GPS::readLoc(char *locstr, char *datestr) {
    if (0) {
        return false;
    }
    sprintf(datestr, "%u/%u/%u %u:%u:%u\n%u Satellites",
        gps->date.month(),
        gps->date.day(),
        gps->date.year(),
        gps->time.hour(),
        gps->time.minute(),
        gps->time.second(),
        gps->satellites.value());
    sprintf(locstr, "%s, %s", String(gps->location.lat(),6).c_str(),
                              String(gps->location.lng(),6).c_str());
    Serial.println(gps->location.lat());
    return true;
}

bool GPS::isValid() {
    return gps->location.isValid();
}
