RS232
=====

A C and C++ library for using rs232 protocole over linux and windows.

## Instalation

1) Download the source code:

```
git clone https://github.com/kranfix/rs232.git
```

2) Compile for your computer:

In C:

```
gcc -c rs232.cc -o librs232c.a
```

In C++:

```
g++ -c rs232.cc -o librs232cc.a
```

## Files

Header: rs232.h
main source file: rs232.c

For linux:
* In C: rs232_linux.c
* In C++: rs232_linux.cc

For windows:
* In C: rs232_win.c
* In C++: rs2323_win.cc


## Available Functions

In C:

In C++:

# Samples with an Arduino

For excecute the samples, first you must compile this code
in your Arduino:

```Arduino
#define baudr 9600

void setup() {
  Serial.begin(baudr);

  // Testing kfx::RS232::WriteByte
  byte a = Serial.ReadByte();

  // Testing kfx::RS232::ReadBuf
  Serial.print("Caracte recibido: ");

  // Testing kfx::RS232::ReadByte
  Serial.println(a);


  // Testing kfx::RS232::WriteBuf
  char A[80];
  A = Serial.readBytes();
  Serial.print("");



}

void loop() { }
```
