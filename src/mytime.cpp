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

char *mytime::isoTime() {
    timeClient.update();

    long epoch = timeClient.getEpochTime();

    byte second = epoch%60; epoch /= 60;
    byte minute = epoch%60; epoch /= 60;
    byte hour   = epoch%24; epoch /= 24;

    unsigned int years = epoch/(365*4+1)*4; epoch %= 365*4+1;
    unsigned int year;
    for (year=3; year>0; year--)
    {
        if (epoch >= days[year][0])
            break;
    }

    unsigned int month;
    for (month=11; month>0; month--)
    {
        if (epoch >= days[year][month])
            break;
    }

    year  = years+year;
    month = month+1;
    unsigned int day   = epoch - days[year][month]+1;

    unsigned int weekday  = (dayOfMonth += month < 3 ? year-- : year - 2, 23*month/9 + dayOfMonth + 4 + year/4- year/100 + year/400)%7;


    //char* iso = asctime(tmS);
    return sprintf("%s%s%sUTC%s",
      String(day).c_str(),
      String(hour).c_str(),
      String(minute).c_str(),
      String(month).c_str(),
    );
}
