#include <SoftwareSerial.h>
       

 
// Define the pins for RS485 communication
#define RE 8
#define DE 7
 
// Request frame for the soil sensor   
const byte soilSensorRequest[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x02, 0x71, 0xCB};

byte soilSensorResponse[9];
SoftwareSerial mod(2, 3); // Software serial for RS485 communication
 
 
void setup()
{
  Serial.begin(9600); // Initialize serial communication for debugging
  mod.begin(4800);    // Initialize software serial communication at 4800 baud rate
  pinMode(RE, OUTPUT); // Set RE pin as output
  pinMode(DE, OUTPUT); // Set DE pin as output
  delay(500);
}
 
 
void loop()
{
  // Start the transmission mode for RS485
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
 
  // Send the request frame to the soil sensor
  mod.write(soilSensorRequest, sizeof(soilSensorRequest));
 
  // End the transmission mode and set to receive mode for RS485
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(10);
 
  // Wait for the response from the sensor or timeout after 1 second
  unsigned long startTime = millis();
  while (mod.available() < 9 && millis() - startTime < 1000)
  {
    delay(1);
  }
 
  if (mod.available() >= 9) // If valid response received
  {
    // Read the response from the sensor
    byte index = 0;
    while (mod.available() && index < 9)
    {
      soilSensorResponse[index] = mod.read();
      Serial.print(soilSensorResponse[index], HEX); // Print the received byte in HEX format
      Serial.print(" ");
      index++;
    }
    Serial.println();
 
    // Parse and calculate the Moisture value
    int Moisture_Int = int(soilSensorResponse[3] << 8 | soilSensorResponse[4]);
    float Moisture_Percent = Moisture_Int / 10.0;
 
    Serial.print("Moisture: ");
    Serial.print(Moisture_Percent);
    Serial.println(" %RH");
 
 
 
    // Parse and calculate the Temperature value
    int Temperature_Int = int(soilSensorResponse[5] << 8 | soilSensorResponse[6]);
    float Temperature_Celsius = Temperature_Int / 10.0;
 
    // Check if temperature is negative and convert accordingly
    if (Temperature_Int > 0x7FFF)
    {
      Temperature_Celsius = 0x10000 - Temperature_Int;
      Temperature_Celsius = -Temperature_Celsius / 10.0;
    }
 
    Serial.print("Temperature: ");
    Serial.print(Temperature_Celsius);
    Serial.println(" °C");
 
  
  }
  else
  {
    // Print error message if no valid response received
    Serial.println("Sensor timeout or incomplete frame");
   
  }
 
  delay(1000); // Wait for a second before the next loop iteration
}