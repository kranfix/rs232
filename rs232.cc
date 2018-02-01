/*
***********************************************
*
* Author: Frank Andre Moreno Vera
*
* Copyright (C) 2014 Frank Andre Moreno Vera
*
* frank@ariot.pe
*
***********************************************
*/

#ifndef kranfix_rs232_rs232_cc
#define kranfix_rs232_rs232_cc

#include "rs232.h"

// Only for testing purpouse
//#undef __cplusplus
//#undef __linux__

#ifndef __cplusplus
#  ifdef __linux__
#    include "rs232_linux.c"
#  else
#    include "rs232_win.c"
#  endif

int kfx_RS232_IsAvailable(kfx_RS232 * h) { return h->available; }

void kfx_rs232_Print(kfx_RS232 * h, const char *text)  /* sends a string to serial port */
{
  while(*text != 0)   kfx_rs232_WriteByte(h, *(text++));
}

#else
#  ifdef __linux__
#    include "rs232_linux.cc"
#  else
#    include "rs232_win.cc"
#  endif // __linux__

// Sends a string to serial port till finding a '\0'
void kfx::RS232::Print(const char *text)
{
  while(*text != 0) Write( *(text++) );
}

#endif // __cplusplus

#endif // kranfix_rs232_rs232_cc
