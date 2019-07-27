#include "mytime.h"
#include <time.h>
#include "mqtt.h"


// create wifi UDP foo
WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 0);

tm *timeStruct;


int mytime::setup() {
    if (WiFi.status() == WL_CONNECTED) {
        timeClient.begin();
        return 0;
    }
    return 1;
}

String mytime::time() {
    timeClient.update();
    return timeClient.getFormattedTime();
}

char *mytime::isoTime() {
    timeClient.update();

    localtime_r((time_t*)timeClient.getEpochTime(), timeStruct);

    return "test";
}
