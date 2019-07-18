#ifndef __TIME_H__
#define __TIME_H__

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

namespace time {
    int setup();
    const char *time();
}

#endif //__TIME_H__