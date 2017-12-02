#ifndef _GPS_H_
#define _GPS_H_

#define GPS_BAUD 9600
#define GPS_LED 13
#define RX_PIN 11
#define TX_PIN 6

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class GPS {
private:
    TinyGPSPlus *gps;
    SoftwareSerial *gpsSS;
public:
    GPS();
    bool readLoc(char *locstr, char *datestr);
    bool checkAvalability();
    bool isValid();
    void smartDelay(unsigned long ms);
};

#endif
