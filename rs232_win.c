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

#ifndef RS232_WIN_C
#define RS232_WIN_C

#include "rs232.h"

static int valid_baudrates[] = {110, 300, 600, 1200, 2400, 4800,
        9600, 19200, 38400, 57600, 115200, 128000, 256000};

int kfx_RS232_Init(kfx_RS232 * h, char * dev_name, int baudrate)
{
  // Device name
  strcpy(h->devname,dev_name);

  // Looks for a valid baurate
  int i;
  for (i = 0; i  < 13 && baudrate == valid_baudrates[i]; i++)
    break;

  char baudr_conf[64];
  if (i < 13)
  {
    h->baudr = baudrate;
    sprintf(baudr_conf, "baud=%d data=8 parity=N stop=1",baudrate);
  }
  else
  {
    printf("invalid baudrate\n");
    return 1;
  }

  h->Cport = CreateFileA(h->devname,
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      0,                          /* no threads */
                      NULL);                      /* no templates */

  if(h->Cport == INVALID_HANDLE_VALUE)
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
    CloseHandle(h->Cport);
    return(1);
  }

  if(!SetCommState(h->Cport, &port_settings))
  {
    printf("unable to set comport cfg settings\n");
    CloseHandle(h->Cport);
    return 1;
  }

  COMMTIMEOUTS Cptimeouts;

  Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(h->Cport, &Cptimeouts))
  {
    printf("unable to set comport time-out settings\n");
    CloseHandle(h->Cport);
    return(1);
  }

  return(0);
}

int kfx_RS232_ReadByte(kfx_RS232 * h, unsigned char byte)
{
  int n;
  ReadFile(h->Cport, byte, 1, (LPDWORD)((void *)&n), NULL);
  return n;
}

int kfx_RS232_ReadBuf(kfx_RS232 * h, unsigned char *buf, int size)
{
  int n;

  if(size>4096)  size = 4096;

/* added the void pointer cast, otherwise gcc will complain about */
/* "warning: dereferencing type-punned pointer will break strict aliasing rules" */

  ReadFile(h->Cport, buf, size, (LPDWORD)((void *)&n), NULL);

  return n;
}

int kfx_RS232_WriteByte(int comport_number, unsigned char byte)
{
  int n;
  WriteFile(h->Cport, &byte, 1, (LPDWORD)((void *)&n), NULL);
  return (n < 0)? 1 : 0;
}

int kfx_RS232_WriteBuf(kfx_RS232 * h, unsigned char * buf, int size)
{
  int n;

  if(WriteFile(h->Cport, buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return(n);
  }

  return(-1);
}

void kfx_RS232_Close(kfx_RS232 * h)
{
  CloseHandle(h->Cport);
}

int kfx_RS232_IsCTSEnabled(kfx_RS232 * h)
{
  int status;
  GetCommModemStatus(h->Cport, (LPDWORD)((void *)&status));
  return (status & MS_CTS_ON)? 1 : 0;
}

#endif // RS232_WIN_C
