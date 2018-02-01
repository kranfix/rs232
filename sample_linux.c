#include <stdio.h>
#include "rs232.h"

int main () {
  kfx_RS232 h;  // Port Handler
  char serialPort[] = "/dev/ttyUSB0";
  int baudrate = 9600;
  int isOpen = kfx_RS232_Init(&h, serialPort, baudrate);
  if(!isOpen){
    printf("Serial port %s is not available",serialPort);
  }

  // Sending a Character
  kfx_RS232_WriteByte(&h, 'A');

  // Reciving a buffer
  char buf[30];
  kfx_RS232_ReadBuf(&h,buf,22);
  puts(buf);

  // Sending a buffer
  kfx_RS232_WriteBuf(&h, "Hello!", 6);
  kfx_RS232_ReadBuf(&h,buf,22);
  puts(buf);

  // Closing port
  puts("\nClosing Serialport ...");
  kfx_RS232_Close(&h);

  return 0;
}
