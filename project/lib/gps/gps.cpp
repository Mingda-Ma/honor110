#include "gps.h"
#include <TinyGPS++.h>

GPS::GPS() {
    gps = new TinyGPSPlus();
    Serial.begin(GPS_BAUD);
    pinMode(GPS_LED, OUTPUT);
}

void GPS::smartDelay(unsigned long ms) {
    unsigned long st = millis();
    while (millis() - st < ms) {
        while (Serial.available()) {
            gps->encode(Serial.read());
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
    sprintf(locstr, "%.6f, %.6f", gps->location.lat(), gps->location.lng());
    return true;
}

bool GPS::isValid() {
    return gps->location.isValid();
}
