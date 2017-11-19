#include "sms.h"

SMSRec::SMSRec() {
    fonaSS = new SoftwareSerial(FONA_TX, FONA_RX);
    fona = new Adafruit_FONA(FONA_RST);
}

bool SMSRec::init() {
    fonaSS->begin(4800);
    return fona->begin(*fonaSS);
}
