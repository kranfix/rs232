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

static int error;
static struct termios nps;

int kfx_RS232_Init(kfx_RS232 * h, char * dev_name, int baudrate) {
    // asigning device name
    strcpy(h->devname,dev_name);

    // Chossing baudrate
    switch(baudrate)
    {
      case      50 : h->baudr = B50;       break;
      case      75 : h->baudr = B75;       break;
      case     110 : h->baudr = B110;      break;
      case     134 : h->baudr = B134;      break;
      case     150 : h->baudr = B150;      break;
      case     200 : h->baudr = B200;      break;
      case     300 : h->baudr = B300;      break;
      case     600 : h->baudr = B600;      break;
      case    1200 : h->baudr = B1200;     break;
      case    1800 : h->baudr = B1800;     break;
      case    2400 : h->baudr = B2400;     break;
      case    4800 : h->baudr = B4800;     break;
      case    9600 : h->baudr = B9600;     break;
      case   19200 : h->baudr = B19200;    break;
      case   38400 : h->baudr = B38400;    break;
      case   57600 : h->baudr = B57600;    break;
      case  115200 : h->baudr = B115200;   break;
      case  230400 : h->baudr = B230400;   break;
      case  460800 : h->baudr = B460800;   break;
      case  500000 : h->baudr = B500000;   break;
      case  576000 : h->baudr = B576000;   break;
      case  921600 : h->baudr = B921600;   break;
      case 1000000 : h->baudr = B1000000;  break;
      default      : printf("invalid baudrate\n");
                     return 0;
    }

    h->port = open(h->devname, O_RDWR | O_NOCTTY | O_NDELAY);
    if(h->port == -1)
    {
      perror("unable to open comport ");
      return 0;
    }

    error = tcgetattr(h->port, &(h->ops) );
    if(error == -1)
    {
      close(h->port);
      perror("unable to read portsettings ");
      return 0;
    }
    memset(&nps, 0, sizeof(nps));  /* clear the new struct */

    nps.c_cflag = h->baudr | CS8 | CLOCAL | CREAD;
    nps.c_iflag = IGNPAR;
    nps.c_oflag = 0;
    nps.c_lflag = 0;
    nps.c_cc[VMIN] = 0;      /* block untill n bytes are received */
    nps.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
    error = tcsetattr(h->port, TCSANOW, &nps);
    if(error == -1)
    {
      close(h->port);
      perror("unable to adjust portsettings ");
      return 0;
    }

    h->available = 1; // true
    return h->available;
}

int kfx_RS232_ReadByte(kfx_RS232 * h, unsigned char byte)
{
  return read(h->port, &byte, 1);
}

int kfx_RS232_ReadBuf(kfx_RS232 * h, unsigned char *buf, int size)
{
#ifndef __STRICT_ANSI__                       /* __STRICT_ANSI__ is defined when the -ansi option is used for gcc */
  if(size > SSIZE_MAX)  size = (int)SSIZE_MAX;  /* SSIZE_MAX is defined in limits.h */
#else
  if(size>4096)  size = 4096;
#endif

  return read(h->port, buf, size);
}

int kfx_RS232_WriteByte(kfx_RS232 * h, unsigned char byte)
{
  return write(h->port, &byte, 1);
}

int kfx_RS232_WriteBuf(kfx_RS232 * h, unsigned char *buf, int size)
{
  return write(h->port, buf, size);
}

void kfx_RS232_Close(kfx_RS232 * h)
{
  close(h->port);
  tcsetattr(h->port, TCSANOW, &(h->ops) );
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
int kfx_RS232_IsCTSEnabled(kfx_RS232 * h)
{
  int status;
  status = ioctl(h->port, TIOCMGET, &status);
  return (status & TIOCM_CTS)? 1 : 0;
}

#endif // RS232_LINUX_C
