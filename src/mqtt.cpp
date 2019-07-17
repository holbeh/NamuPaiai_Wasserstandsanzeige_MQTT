#include "myTypes.h"
#include "mqtt.h"
#include "config.h"
#include "debug.h"


WiFiClient wClient;
PubSubClient client(wClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.print("\n");
  Serial.print("Connecting to ");
  Serial.print(CONF_WIFI_SSID);
  Serial.print("\n");

  WiFi.begin(CONF_WIFI_SSID, CONF_WIFI_PW);

  for (int i = 0; i < CONF_WIFI_TRIES; i++) {
    if (WiFi.status() == WL_CONNECTED) { break; }
      delay(500);
      Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) 
    return;
  delay(1000);

  Serial.print("\n");
  Serial.print("WiFi connected\n");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");
  WiFi.printDiag(Serial);

  client.setServer(CONF_MQTT_HOST, CONF_MQTT_PORT);
}

void reconnect() {
  // Loop until we're reconnected
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    Serial.print(wClient.localIP());
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