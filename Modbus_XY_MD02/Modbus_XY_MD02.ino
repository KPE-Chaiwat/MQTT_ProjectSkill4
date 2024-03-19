#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>

SoftwareSerial mySerial(16, 17); // RX, TX

#define MAX485_DE      5
#define MAX485_RE_NEG  18

ModbusMaster node;


bool Mode_R_W = true;

//-----------------variable read sensor---------------------------------
uint8_t slaveID = 1; // Default slave ID
uint8_t Quantity = 2;
int FunctionCode_read = 3;
uint8_t Address_read = 1;
int BaudRate_Sensor = 9600;
//------------------------variable write Sensor----------------------------------------------
uint8_t Address_write = 1;
uint8_t value ="";
bool value_bool = false;
int FunctionCode_write = 3;
//----------------------------------------------------------------------

String sendMessage[1024] ; //เอาไว้เก็บdata ก่อนส่งออกไปที่ pi3




//-----------------------------SetSlaveID Function----------------------------------
void SetSlaveID() {
  
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

      bool receiveMode= doc["Mode_R_W"];
      int receiveFunctionCode =doc[]
      Mode_R_W =receiveMode;
   // variable read
      int receivedID = doc["ID"];
      int receivedQuantity = doc["Quantity"];
      int receivedAddress_read = doc["Address_Read"];
      int receiveBaudRate_Sensor=doc["Baudrate"];
      slaveID = receivedID;
      Quantity = receivedQuantity;
      Address_read = receivedAddress_read;
      BaudRate_Sensor = receiveBaudRate_Sensor;


      // variable write
      uint8_t receiveValue= doc["Value_write"];
      int receiveAddress_write = doc["Address_write"];
      value = receiveValue;
      Address_write=receiveAddress_write;
      value_bool= SetboolValue_bool(value);


     
    }
    // Reinitialize ModbusMaster with the new slave ID
    node.begin(slaveID, mySerial);

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
///------------------------SetFunctionCode-------------------------------------
  void SetFunctionCode(){

  }


///-------------------SetUp value boolean-----------------------------------------
bool SetboolValue_bool(uint8_t valueIN){
if(valueIN=='1'){
  return true;
}else{
  return false;
}
}
//-----------------------------------------------------------------
void setup()
{
  BaudRate_Sensor = 9600;
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
 //mySerial.begin(9600);
  mySerial.begin(BaudRate_Sensor);//-----set baudrate sensor
  while (!mySerial) {
    Serial.println("loop for init software serial");
  }

  node.begin(slaveID, mySerial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

}

void loop()
{
  SetSlaveID();

  uint8_t result;  
  uint16_t data[Quantity];

//  result = node.readInputRegisters(1, 2);

  result = node.readInputRegisters(Address_read, Quantity);

  if (result == node.ku8MBSuccess)
  {

    for (int i = 0; i < Quantity; i++) {  
      sendMessage[i]= node.getResponseBuffer(i);
    }
 // sent Data to Raspbery PI3
   Serial.println("["+sendMessage[0]+","+sendMessage[1]+","+sendMessage[2]+","+sendMessage[3]+","+sendMessage[4]+","+sendMessage[5]+","+sendMessage[6]+","+sendMessage[7]+","+sendMessage[8]+","+sendMessage[9]+","+sendMessage[10]+"]"+"."); 
  }
  else
  {
    Serial.print(".\n");//ห้ามแก้
  }
  delay(1000);
}

