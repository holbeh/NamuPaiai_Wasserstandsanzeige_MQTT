#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <Arduino.h>

#include "config.h"

#ifdef DEBUG
#define DEBUG_LOG(fmt, args...) Serial.print(fmt, ## args)
#else
#define DEBUG_LOG(fmt, args...)
#endif


#endif // __DEBUG_H__