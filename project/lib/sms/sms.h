#ifndef __SMS_H__
#define __SMS_H__

#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>
#include <vector>

#define FONA_RX  9
#define FONA_TX  8
#define FONA_RST 4

#define SECRET "142857"

const char help_manual[] =
    "Commands Available:\n"
    "[a]: add phone number\n"
    "[b]: battery voltage\n"
    "[c]: check authorization"
    "[d]: delete this phone number\n"
    "[g]: get location\n"
    "[h]: help manual\n";


class SMSRec {
private:
    Adafruit_FONA       *fona;
    SoftwareSerial      *fonaSS;
    std::vector<char*>  known_id;

    char                fonaNotificationBuffer[64];
    char                smsBuffer[256];
    char                callerIDbuffer[32];

    bool checkID();

public:
    SMSRec();
    bool init();
    bool getMsg();
    bool addUser();
    bool deleteUser();
    uint16_t getBatt();
    void listen();
};

#endif
