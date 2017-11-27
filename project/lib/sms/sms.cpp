#include "sms.h"
#include "Adafruit_FONA.h"
#include "gps.h"
#include <vector>

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

bool SMSRec::addUser() {
    unsigned long st = millis();
    while (millis() - st <= 60000) {
        if (!getMsg()) {
            delay(200);
            continue;
        }
        if (strcmp(smsBuffer, SECRET) == 0) {
            char *new_id = new char[31];
            strcpy(new_id, callerIDbuffer);
            known_id.push_back(new_id);
            char response[80];
            sprintf(response, "User %s added succesfully!", callerIDbuffer);
            fona->sendSMS(callerIDbuffer, response);
            return true;
        }
        else {
            fona->sendSMS(callerIDbuffer, "Sercret not correct");
            return false;
        }
    }
    return false;
}

bool SMSRec::checkID() {
    for (int i = 0; i < known_id.size(); i++)
        if (strcmp(known_id[i], callerIDbuffer) == 0)
            return true;
    return false;
}

bool SMSRec::deleteUser() {
    for (int i = 0; i < known_id.size(); i++) {
        if (strcmp(known_id[i], callerIDbuffer) == 0) {
            delete known_id[i];
            known_id.erase(known_id.begin() + i);
            return true;
        }
    }
    return false;
}

void SMSRec::listen(GPS *gps) {
    if (getMsg()) {
        char cmd = smsBuffer[0];
        char response[100];
        if (cmd >= 'A' && cmd <= 'Z')
            cmd -= 'A' - 'a';
        switch (cmd) {
        case 'a':
            if (checkID()) {
                fona->sendSMS(callerIDbuffer, "User already exist");
                break;
            }
            fona->sendSMS(callerIDbuffer, "Please enter your secret");
            addUser();
            break;
        case 'c':
            if (checkID())
                fona->sendSMS(callerIDbuffer, "You are authorized user!");
            else
                fona->sendSMS(callerIDbuffer, "You are not authorized!");
            break;
        case 'd':
            if (deleteUser())
                fona->sendSMS(callerIDbuffer, "Succesfully removed!");
            break;
        case 'b':
            if (checkID()) {
                sprintf(response, "Battery Remaining: %u\%", getBatt());
                fona->sendSMS(callerIDbuffer, response);
            }
            break;
        case 'g':
            char locstr[40];
            char datestr[70];
            if(gps->readLoc(locstr, datestr)){
                fona->sendSMS(callerIDbuffer, datestr);
                fona->sendSMS(callerIDbuffer, locstr);
            }
            else {
                fona->sendSMS(callerIDbuffer, "GPS Not Yet Available");
            }
            break;
        default:
            fona->sendSMS(callerIDbuffer, help_manual);
            break;
        }
    }
}

uint16_t SMSRec::getBatt() {
    uint16_t vbat;
    fona->getBattPercent(&vbat);
    return vbat;
}
