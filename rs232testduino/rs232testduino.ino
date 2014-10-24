#define baudr 9600

void setup() {
  Serial.begin(baudr);

  // Testing kfx::RS232::WriteByte
  byte a = Serial.ReadByte();

  // Testing kfx::RS232::ReadBuf
  Serial.print("Caracte recibido: ");

  // Testing kfx::RS232::ReadByte
  Serial.print(a);
  

  // Testing kfx::RS232::WriteBuf
  char A[80];
  A = Serial.readBytes();
  Serial.print("");



}

void loop() { }
