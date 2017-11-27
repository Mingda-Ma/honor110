#ifndef _GPS_H_
#define _GPS_H_

#define GPS_BAUD 9600
#define GPS_LED 11

#include <TinyGPS++.h>

class GPS {
private:
    TinyGPSPlus *gps;
public:
    GPS();
    bool readLoc(char *locstr, char *datestr);
    bool checkAvalability();
    bool isValid();
    void smartDelay(unsigned long ms);
};

#endif
