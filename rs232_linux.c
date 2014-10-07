/*
***********************************************
*
* Author: Frank Andre Moreno Vera
*
* Copyright (C) 2014 Frank Andre Moreno Vera
*
* frankmoreno1993@gmail.com
*
***********************************************
*/

#ifndef RS232_LINUX_C
#define RS232_LINUX_C

#include "rs232.h"

int Cport[22],
    error;

struct termios nps; // new port settings // Must it be static?

char comports[22][13] = {"/dev/ttyACM0", \
    "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3", \
    "/dev/ttyS4", "/dev/ttyS5", "/dev/ttyS6", \
    "/dev/ttyS7", "/dev/ttyS8", "/dev/ttyS9", \
  "/dev/ttyS10", "/dev/ttyS11", "/dev/ttyS12", \
  "/dev/ttyS13", "/dev/ttyS14", "/dev/ttyS15", \
"/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2",\
"/dev/ttyUSB3", "/dev/ttyUSB4", "/dev/ttyUSB5"};



int OpenComport(int comport_number, int baudrate)
{
  int baudr;

  if((comport_number>21)||(comport_number<0))
  {
    printf("illegal comport number\n");
    return(1);
  }

  switch(baudrate)
  {
    case      50 : baudr = B50;         break;
    case      75 : baudr = B75;         break;
    case     110 : baudr = B110;        break;
    case     134 : baudr = B134;        break;
    case     150 : baudr = B150;        break;
    case     200 : baudr = B200;        break;
    case     300 : baudr = B300;        break;
    case     600 : baudr = B600;        break;
    case    1200 : baudr = B1200;       break;
    case    1800 : baudr = B1800;       break;
    case    2400 : baudr = B2400;       break;
    case    4800 : baudr = B4800;       break;
    case    9600 : baudr = B9600;       break;
    case   19200 : baudr = B19200;      break;
    case   38400 : baudr = B38400;      break;
    case   57600 : baudr = B57600;      break;
    case  115200 : baudr = B115200;     break;
    case  230400 : baudr = B230400;     break;
    case  460800 : baudr = B460800;     break;
    case  500000 : baudr = B500000;     break;
    case  576000 : baudr = B576000;     break;
    case  921600 : baudr = B921600;     break;
    case 1000000 : baudr = B1000000;    break;
    default      : printf("invalid baudrate\n");
                   return(1);
                   break;
  }

  Cport[comport_number] = open(comports[comport_number], O_RDWR | O_NOCTTY | O_NDELAY);
  if(Cport[comport_number]==-1)
  {
    perror("unable to open comport ");
    return(1);
  }

  error = tcgetattr(Cport[comport_number], old_port_settings + comport_number);
  if(error==-1)
  {
    close(Cport[comport_number]);
    perror("unable to read portsettings ");
    return(1);
  }
  memset(&new_port_settings, 0, sizeof(new_port_settings));  /* clear the new struct */

  new_port_settings.c_cflag = baudr | CS8 | CLOCAL | CREAD;
  new_port_settings.c_iflag = IGNPAR;
  new_port_settings.c_oflag = 0;
  new_port_settings.c_lflag = 0;
  new_port_settings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
  new_port_settings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
  error = tcsetattr(Cport[comport_number], TCSANOW, &new_port_settings);
  if(error==-1)
  {
    close(Cport[comport_number]);
    perror("unable to adjust portsettings ");
    return(1);
  }

  return(0);
}


int kfx_rs232_Read(int comport_number, unsigned char *buf, int size)
{
  int n;

#ifndef __STRICT_ANSI__                       /* __STRICT_ANSI__ is defined when the -ansi option is used for gcc */
  if(size>SSIZE_MAX)  size = (int)SSIZE_MAX;  /* SSIZE_MAX is defined in limits.h */
#else
  if(size>4096)  size = 4096;
#endif

  n = read(Cport[comport_number], buf, size);

  return(n);
}


int SendByte(int comport_number, unsigned char byte)
{
  int n;

  n = write(Cport[comport_number], &byte, 1);
  if(n<0)  return(1);

  return(0);
}


int SendBuf(int comport_number, unsigned char *buf, int size)
{
  return write(Cport[comport_number], buf, size);
}


void CloseComport(int comport_number)
{
  close(Cport[comport_number]);
  tcsetattr(Cport[comport_number], TCSANOW, old_port_settings + comport_number);
}

/*
Constant  Description
TIOCM_LE  DSR (data set ready/line enable)
TIOCM_DTR DTR (data terminal ready)
TIOCM_RTS RTS (request to send)
TIOCM_ST  Secondary TXD (transmit)
TIOCM_SR  Secondary RXD (receive)
TIOCM_CTS CTS (clear to send)
TIOCM_CAR DCD (data carrier detect)
TIOCM_CD  Synonym for TIOCM_CAR
TIOCM_RNG RNG (ring)
TIOCM_RI  Synonym for TIOCM_RNG
TIOCM_DSR DSR (data set ready)
*/

int IsCTSEnabled(int comport_number)
{
  int status;

  status = ioctl(Cport[comport_number], TIOCMGET, &status);

  if(status & TIOCM_CTS) return(1);
  return(0);
}


#endif // RS232_LINUX_C
