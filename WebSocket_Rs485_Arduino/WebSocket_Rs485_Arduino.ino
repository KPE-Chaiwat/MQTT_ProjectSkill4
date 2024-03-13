#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
//-------------websocket----------------
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//------------- RS 485--------------
SoftwareSerial mySerial(16, 17); // RX, TX

#define MAX485_DE      5
#define MAX485_RE_NEG  18

ModbusMaster node;

void wifiConnect();


//------------- wifi and mqtt--------------
const char* ssid = "metaradiator";
const char* password = "machanism1234";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
int datatemp = 12;
char buff[80];
void setup() {


  //------setup wifi ----------------------
  Serial.begin(115200);
  wifiConnect();
  pinMode(23, INPUT_PULLUP);
  //--------------set up RS 485 ----------------
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  Serial.println("start init serial 0");

  
  while (!Serial) {
    Serial.println("loop for init serial 0"); 
  }
  Serial.println("start init software serial");
  mySerial.begin(9600);
  while (!mySerial) {
    Serial.println("loop for init software serial");
  }
  
  node.begin(1, mySerial);  
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
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



//------------------------RS485 get data ----------------------------------------

  uint8_t result;
  uint16_t data[2];

  result = node.readInputRegisters(1, 2);

  if (result == node.ku8MBSuccess)
  {
    Serial.print("Temp: ");
    Serial.println(node.getResponseBuffer(0) / 10.0f);
    datatemp=node.getResponseBuffer(0) / 10.0f;
    Serial.print("Humi: ");
    Serial.println(node.getResponseBuffer(1) / 10.0f);
    Serial.println();
  }
    else
    {
      Serial.println(".");
    }

//---------------------------sent data ---------------------------------

    //keyPress++;
    itoa(datatemp, buff, 10); //ส่งdata ออกไป
    mqttClient.publish("esekku/room1/toggle", buff, true);
    Serial.print("published ");
    Serial.println(buff);
    delay(3000);
}

//-----------------------------------MQTT function------------------------------------------------------------------------
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
///----------------------------------------RS485 function---------------------------------------------------------------

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}
