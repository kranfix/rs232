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

#ifndef RS232_CPP
#define RS232_CPP

#include "rs232.h"

//#undef __cplusplus
//#undef __linux__

#ifndef __cplusplus
#  ifdef __linux__
#    include "rs232_linux.c"
#  else
#    include "rs232_win.c"
#  endif

void kfx_rs232_Print(kfx_RS232 * h, const char *text)  /* sends a string to serial port */
{
  while(*text != 0)   kfx_rs232_WriteByte(h, *(text++));
}

#else
#  ifdef __linux__
#    include "rs232_linux.cpp"
#  else
#    include "rs232_win.cpp"
#endif // __linux__

// Sends a string to serial port till finding a '\0'
void kfx::RS232::Print(const char *text)
{
  while(*text != 0) Write( *(text++) );
}

#endif // __cplusplus

#endif // RS232_CPP
