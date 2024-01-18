#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid = "Good Boy";
const char* password = "chaiwat10478";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
int keyPress = 0;
char buff[80];
void setup() {
  Serial.begin(115200);
  wifiConnect();
  pinMode(23, INPUT_PULLUP);
}
void loop() {
  if (!mqttClient.connected()) {
    Serial.println("connecting........");
    mqttConnect();
    delay(1000);
  } else {
    Serial.println("connect ok");
    mqttClient.loop();
  }

//  if (digitalRead(23) == LOW) {
//    keyPress++;
//    itoa(keyPress, buff, 10);
//    mqttClient.publish("esekku/room1/count", buff, true);
//    Serial.print("published ");
//    Serial.println(buff);
//    delay(3000);
//  }

    keyPress++;
    itoa(keyPress, buff, 10); //ส่งdata ออกไป
    mqttClient.publish("esekku/room1/toggle", buff, true);
    Serial.print("published ");
    Serial.println(buff);
    delay(3000);
}
void mqttOnReceived(char* topic, byte* message, unsigned int length) {}
void wifiConnect(void) {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
  }
}
void mqttConnect(void) {
  mqttClient.setCallback(mqttOnReceived);
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.connect("do-not-use-the-same-ID-9012");
}
