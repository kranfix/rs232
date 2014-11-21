/*
***************************************************************************
*
* Author: Frank Andre Moreno Vera
*
* Copyright (C) 2014 Frank Andre Moreno Vera
*
* frankmoreno1993@gmail.com
*
***************************************************************************
*/

#ifndef RS232_WIN_CPP
#define RS232_WIN_CPP

#include "rs232.h"

static int valid_baudrates[] = {110, 300, 600, 1200, 2400, 4800,
        9600, 19200, 38400, 57600, 115200, 128000, 256000};

kfx::RS232::RS232(char * dev_name, int baudrate)
{
  // Device name
  strcpy(devname,dev_name);

  // Looks for a valid baurate
  int i;
  for (i = 0; i  < 13 && baudrate == valid_baudrates[i]; i++)
    break;

  char baudr_conf[64];
  if (i < 13)
  {
    baudr = baudrate;
    sprintf(baudr_conf, "baud=%d data=8 parity=N stop=1",baudrate);
  }
  else
  {
    printf("invalid baudrate\n");
    return 1;
  }

  Cport = CreateFileA(devname,
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      0,                          /* no threads */
                      NULL);                      /* no templates */

  if(Cport == INVALID_HANDLE_VALUE)
  {
    printf("unable to open comport\n");
    return 1;
  }

  DCB port_settings;
  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(baudr_conf, &port_settings))
  {
    printf("unable to set comport dcb settings\n");
    CloseHandle(Cport);
    return(1);
  }

  if(!SetCommState(Cport, &port_settings))
  {
    printf("unable to set comport cfg settings\n");
    CloseHandle(Cport);
    return 1;
  }

  COMMTIMEOUTS Cptimeouts;

  Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(Cport, &Cptimeouts))
  {
    printf("unable to set comport time-out settings\n");
    CloseHandle(Cport);
    return(1);
  }

  return(0);
}

int kfx::RS232::Read(unsigned char byte)
{
  int n;
  ReadFile(Cport, byte, 1, (LPDWORD)((void *)&n), NULL);
  return n;
}

int kfx::RS232::Read(unsigned char *buf, int size)
{
  int n;

  if(size>4096)  size = 4096;

/* added the void pointer cast, otherwise gcc will complain about */
/* "warning: dereferencing type-punned pointer will break strict aliasing rules" */

  ReadFile(Cport, buf, size, (LPDWORD)((void *)&n), NULL);

  return n;
}

int kfx::RS232::Write(int comport_number, unsigned char byte)
{
  int n;
  WriteFile(Cport, &byte, 1, (LPDWORD)((void *)&n), NULL);
  return (n < 0)? 1 : 0;
}

int kfx::RS232::Write(unsigned char * buf, int size)
{
  int n;

  if(WriteFile(Cport, buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return(n);
  }

  return(-1);
}

void kfx::RS232::Close()
{
  CloseHandle(Cport);
}

int kfx::RS232::IsCTSEnabled()
{
  int status;
  GetCommModemStatus(Cport, (LPDWORD)((void *)&status));
  return (status & MS_CTS_ON)? 1 : 0;
}

#endif // RS232_WIN_CPP

