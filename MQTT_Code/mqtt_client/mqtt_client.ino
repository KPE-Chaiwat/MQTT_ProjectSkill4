#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid = "metaradiator";
const char* password = "machanism1234";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
//int keyPress = 0;ยังไม่แน่ใจว่าจะเอาไว้ใช้อะไร
//char buff[80]; ยังไม่แน่ใจว่าจะเอาไว้ใช้อะไร
void setup() {
  Serial.begin(115200);
  wifiConnect();
  pinMode(23, INPUT_PULLUP);
}
void loop() {
  if (!mqttClient.connected()) {
    mqttConnect();
    mqttClient.subscribe("esekku/room1/toggle");
    delay(1000);
  } else {
    mqttClient.loop();
  }
}

void mqttOnReceived(char* topic, byte* message, unsigned int length) {
  String msgStr;
  for (int i = 0; i < length; i++) {
    msgStr += (char)message[i];
  }

  Serial.print("msgStr is :");
  Serial.println(msgStr); // data ที่ get มา...

}
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
  mqttClient.connect("do-not-use-the-same-ID-5678");
}
