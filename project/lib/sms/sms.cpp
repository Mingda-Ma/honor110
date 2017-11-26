#include "sms.h"
#include "Adafruit_FONA.h"

SMSRec::SMSRec() {
    fonaSS = new SoftwareSerial(FONA_TX, FONA_RX);
    fona = new Adafruit_FONA(FONA_RST);
    Serial.begin(9600);
}

bool SMSRec::init() {
    fonaSS->begin(4800);
    return fona->begin(*fonaSS);
}

bool SMSRec::getMsg() {
    char* bufPtr = fonaNotificationBuffer;    //handy buffer pointer
    *bufPtr = 0;

    if (fona->available())      //any data available from the FONA?
    {
        int slot = 0;            //this will be the slot number of the SMS
        unsigned int charCount = 0;
        //Read the notification into fonaInBuffer
        do  {
            *bufPtr = fona->read();
            delay(1);
        } while ((*bufPtr++ != '\n') && (fona->available()) && (++charCount < (sizeof(fonaNotificationBuffer)-1)));

        //Add a terminal NULL to the notification string
        *bufPtr = 0;

        //Scan the notification string for an SMS received notification.
        //  If it's an SMS message, we'll get the slot number in 'slot'
        if (1 == sscanf(fonaNotificationBuffer, "+CMTI: \"SM\",%d", &slot)) {
            //Serial.print("slot: "); Serial.println(slot);

            char callerIDbuffer[32];  //we'll store the SMS sender number in here

            // Retrieve SMS sender address/phone number.
            if (! fona->getSMSSender(slot, callerIDbuffer, 31)) {
                return false;
            }
            //Serial.print(F("FROM: ")); Serial.println(callerIDbuffer);

            // Retrieve SMS value.
            uint16_t smslen;
            if (!fona->readSMS(slot, smsBuffer, 256, &smslen)) { // pass in buffer and max len!
                return false;
            }

            fona->deleteSMS(slot);
            return true;
        }
    }
    return false;
}
