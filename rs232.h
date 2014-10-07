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

#ifndef RS232_H
#define RS232_H

#include <stdio.h>
#include <string.h>


#ifdef __linux__

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#else

#include <windows.h>

#endif

//#undef __cplusplus
#ifdef __cplusplus

namespace kfx {

class RS232
{
    char devname[13];   // Device Name
    int baudr, port;    // Baudrate and Port Number
    bool available = false;
    struct termios ops; // old port settings
public:
    RS232();
    int IsAvailable() { return available; }
    int Read(unsigned char);
    int Read(unsigned char *, int);
    int Write(unsigned char);
    int Write(unsigned char *, int);
    void Print(const char *);
    void Close();
    int IsCTSEnabled();
};

}

#else

// Private tipe
typedef struct __kfx_RS232 {
    char name[13];
    int baudr, port; // Baudrate and Port Number
    struct termios ops;
} kfx_RS232;

int kfx_rs232_init(kfx_RS232 *, int);
int kfx_rs232_Read(kfx_RS232 *, unsigned char *, int);
int kfx_rs232_WriteByte(kfx_RS232 *, unsigned char);
int kfx_rs232_WriteBuf(kfx_RS232 *, unsigned char *, int);
void kfx_rs232_Close(kfx_RS232 *);
void kfx_rs232_Print(kfx_RS232 *, const char *);
int kfx_rs232_IsCTSEnabled(kfx_RS232 *);

#endif



#endif // RS232_H
