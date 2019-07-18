#ifndef __TIME_H__
#define __TIME_H__

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

namespace mytime {
    int setup();
    String time();
}

#endif //__TIME_H__