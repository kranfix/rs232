#define baudr 9600

void setup() {
  Serial.begin(baudr);
}

void loop() {
  while(!Serial.available());

  // Testing kfx::RS232::WriteByte
  byte a = Serial.read();

  // Testing kfx::RS232::ReadBuf
  Serial.print("Recived Character: ");

  // Testing kfx::RS232::ReadByte
  //Serial.println(a);
  Serial.write(a);
  Serial.println();

  // Testing kfx::RS232::WriteBuf
  char A[80];
  int cont = 0;
  while(!Serial.available());
  while(Serial.available()){
    A[cont] = Serial.read();
    delay(10); // A timeout with delay
    if(cont == 0 && A[cont] == '\n'){
      continue;
    }
    cont++;
  }
  A[cont] = 0; // Null character

  Serial.print("Recived buffer: ");
  Serial.print(A);
  Serial.println();
}
