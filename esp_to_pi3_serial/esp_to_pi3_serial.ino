
#include <SoftwareSerial.h>

int datawr =0;
void setup() {
  Serial.begin(9600);
}

void loop() {
 datawr++;
 // Serial.write(datawr); 
 Serial.println(datawr); 

 if(datawr==10){
  datawr=0;
  }

 delay(1000);
}
