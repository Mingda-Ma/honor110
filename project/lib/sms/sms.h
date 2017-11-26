#ifndef __SMS_H__
#define __SMS_H__

#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>

#define FONA_RX  9
#define FONA_TX  8
#define FONA_RST 4

class SMSRec {
private:
    Adafruit_FONA *fona;
    SoftwareSerial *fonaSS;
    char fonaNotificationBuffer[64];
    char smsBuffer[256];
public:
    SMSRec();
    bool init();
    bool getMsg();
};

#endif
