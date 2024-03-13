#define RXD1 (16) 
#define TXD1 (17) 

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
}

void loop() {

 

  while (Serial.available()) { 
    char Datainput = Serial.read();
    String Datainput1 = String(Datainput); 

  Serial.println(Datainput1);
  }

// delay(1000);
}
