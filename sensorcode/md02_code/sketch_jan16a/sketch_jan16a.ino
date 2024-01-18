#include "TinyWireS.h"                  // wrapper class for I2C slave routines
#define I2C_SLAVE_ADDR  0x27            // i2c slave address 

void setup(){
  TinyWireS.begin(I2C_SLAVE_ADDR);      // init I2C Slave mode
}
// pin4 SDA  pin5 SCL ====nano
// pin D18 SDA  pin D19 SCL ====uno
void loop(){
 // byte byteRcvd = 0;
 // if (TinyWireS.available()){           // got I2C input!
    int val = analogRead(PB3);
    TinyWireS.send(val);           // send 5 back to master
    delay(500);
  //}
}
