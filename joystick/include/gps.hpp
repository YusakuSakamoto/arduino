#include "config.hpp"
#include "io.hpp"

#ifndef __GPS_H__
#define __GPS_H__


const speed_t gps_baudrate = B19200;

typedef struct{
  unsigned char moji[20];
}sendtype;

typedef struct{
  unsigned char str;
}recievetype;

class gps: private io
{
public:
  gps();
  virtual ~gps(); 
private:
  sendtype send;
  recievetype recieve;
public:
  int create_sendinfo(int);
  int sendinfo();
  int recieveinfo();
  int show_recieveinfo();
};

#endif
