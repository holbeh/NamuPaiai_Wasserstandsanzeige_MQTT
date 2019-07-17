#include "myTypes.h"
#include "mqtt.h"
#include "config.h"
#include "debug.h"


WiFiClient wClient;
PubSubClient client(wClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  DEBUG_LOG("\n");
  DEBUG_LOG("Connecting to ");
  DEBUG_LOG(CONF_WIFI_SSID);
  DEBUG_LOG("\n");

  WiFi.begin(CONF_WIFI_SSID, CONF_WIFI_PW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG_LOG(".");
  }

  DEBUG_LOG("\n");
  DEBUG_LOG("WiFi connected\n");
  DEBUG_LOG("IP address: ");
  DEBUG_LOG(WiFi.localIP());
  DEBUG_LOG("\n");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect(CONF_MQTT_NAME, CONF_MQTT_USER, CONF_MQTT_PW)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //delay(5000);
    }
  }
}

boolean publish(int percentage) {
    if (!client.connected()) {
        reconnect();
    }
    if (!client.connected()) {
        return false;
    }
    return client.publish(CONF_MQTT_PUB_CH0, String(percentage).c_str(), true);
}