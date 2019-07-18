#include "time.h"


// create wifi UDP foo
WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 3600, 0);


int time::setup() {
    timeClient.begin();
    return 0;
}

const char *time::time() {
    return timeClient.getFormattedTime().c_str();
}