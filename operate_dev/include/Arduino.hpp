#include "config.hpp"
#include "io.hpp"
#define analysis_file "./data/ping_sensor.dat"

const speed_t Arduino_baudrate = B9600;

#ifndef __ARDUINO_H__
#define __ARDUINO_H__
#define ANALYSIS_NUM 10000

typedef struct{
  unsigned char moji[20];
}arduino_sendtype;

typedef struct{
  unsigned char moji;
  unsigned char inches[10];
  unsigned char cm[10];
  unsigned char checksum[10];
  int counter;
  int flag;
}arduino_recievetype;


typedef struct{
  double inches;
  double cm;
  unsigned char checksum[3];
}ping_data;

class Arduino: private io
{
public:
  Arduino();
  virtual ~Arduino(); 
private:
  unsigned char input[30];
  unsigned char checksum;
  arduino_sendtype send;
  arduino_recievetype recieve;
  ping_data ping;
  std::fstream record_file;
  int data_num;
public:
  bool analize_start;
  int create_sendinfo(int);
  int sendinfo();
  int recieveinfo();
  int show_recieveinfo();
  void calc_checksum(unsigned char*);
};

#endif
