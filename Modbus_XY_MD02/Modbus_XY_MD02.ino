#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>

SoftwareSerial mySerial(16, 17); // RX, TX

#define MAX485_DE      5
#define MAX485_RE_NEG  18

ModbusMaster node;
uint8_t slaveID = 1; // Default slave ID

uint8_t Quanitity = 2;

uint8_t Address = 1;

//

String sendMessage[1024] ; //เอาไว้เก็บdata ก่อนส่งออกไปที่ pi





void setSlaveID() {
  if (Serial.available() > 0) {
    // Read the JSON string from Serial
    String jsonString = Serial.readString();

    // Create a JSON document
    DynamicJsonDocument doc(200);

    // Deserialize the JSON string into the document
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
      Serial.print("Failed to parse JSON: ");
      Serial.println(error.c_str());
    } else {
      // Access the value in the document
      int receivedID = doc["ID"];
      int receivedQuanitity = doc["Quanitity"];
      int receivedAddress = doc["Address"];
      // when set SlaveID when receivedID

      slaveID = receivedID;
      Quanitity = receivedQuanitity;
      Address = receivedAddress;
      //      Serial.println(receivedID);
      //      Serial.println(Quanitity);
      //      Serial.println(Address);
    }
    // Reinitialize ModbusMaster with the new slave ID
    node.begin(slaveID, mySerial);
    //Serial.print("Updated Slave ID: ");
   Serial.println(slaveID);
  }
}

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

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  Serial.println("start init serial 0");
  Serial.begin(115200);

  while (!Serial) {
    Serial.println("loop for init serial 0");
  }
  Serial.println("start init software serial");
  mySerial.begin(9600);
  while (!mySerial) {
    Serial.println("loop for init software serial");
  }

  node.begin(slaveID, mySerial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

}

void loop()
{
  setSlaveID();

  uint8_t result;  
  uint16_t data[2];

//  result = node.readInputRegisters(1, 2);
result = node.readInputRegisters(Address, Quanitity);
  if (result == node.ku8MBSuccess)
  {
    //    Serial.print("Temp: ");
    //    Serial.println(node.getResponseBuffer(0) / 10.0f);
    //    Serial.print("Humi: ");
    //    Serial.println(node.getResponseBuffer(1) / 10.0f);
    //    Serial.println();

    for (int i = 0; i < Quanitity; i++) {  
      sendMessage[i]= node.getResponseBuffer(i);
    }
 // sent Data
   Serial.println("["+sendMessage[0]+","+sendMessage[1]+","+sendMessage[2]+","+sendMessage[3]+","+sendMessage[4]+","+sendMessage[5]+","+sendMessage[6]+","+sendMessage[7]+","+sendMessage[8]+","+sendMessage[9]+","+sendMessage[10]+"]"+"."); 
  }
  else
  {
    Serial.print(".\n");//ห้ามแก้
  }
  delay(1000);
}





String SetType(char * type) {

 
  
  String tempString = String(type);
  return tempString;

  
}
