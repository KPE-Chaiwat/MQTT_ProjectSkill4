#define RXD1 (16) // กำหนดขา RX ของ Serial1 เป็นขา 25
#define TXD1 (17) // กำหนดขา TX ของ Serial1 เป็นขา 26

void setup() {
  Serial.begin(9600); // เริ่มต้นใช้งาน Serial0 ที่ความเร็ว (baud rate) 115200
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // เริ่มต้นใช้งาน Serial1 ที่ความเร็ว (baud rate) 9600 พร้อมกำหนดขา RX, TX ใหม่
}
String  mystring;
void loop() {
//  while (Serial.available()) { // วนลูปหากมีข้อมูลเข้ามาจาก Serial0 (จาก Serial Monitor)
//    Serial1.write(Serial.read()); // อ่านค่าจาก Serial0 แล้วเอาค่าที่อ่านได้เขียนไปที่ Serial1
//  }
//  while (Serial1.available()) { // วนลูปหากมีข้อมูลเข้ามาจาก Serial1 (จาก Serial Monitor)
//    Serial.write(Serial1.read()); // อ่านค่าจาก Serial1 แล้วเอาค่าที่อ่านได้เขียนไปที่ Serial0
//  }

  while (Serial.available()) { 
  char x =  Serial.read();
//  Serial.println(x);
; 
Serial.println(x);
  }
  delay(10);
}
