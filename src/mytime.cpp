#include "mytime.h"
#include <time.h>


// create wifi UDP foo
WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 0);


int mytime::setup() {
    timeClient.begin();
    return 0;
}

String mytime::time() {
    timeClient.update();
    return timeClient.getFormattedTime();
}

