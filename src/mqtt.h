#ifndef __MQTT_H__
#define __MQTT_H__

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

extern configData_t cfg;
extern WiFiClient wClient;
extern PubSubClient client;

extern char *timestr;

void setup_wifi();
boolean publish(int percentage);

#endif // __MQTT_H__