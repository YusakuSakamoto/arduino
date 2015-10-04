#include "../include/gps.hpp"

using namespace std;

const char *compare_subject[5] = {
  "$", "\n", ","
};

gps::gps()
{
  Open( gps_serial.c_str() , gps_baudrate);
}

gps::~gps()
{
  Close();
}

int gps::create_sendinfo(int i)
{
  if(i%2==0){ send.moji[0]='0';}
  else if(i%2==1){ send.moji[0]='1';}
  return 0;
}


int gps::sendinfo()
{
  if( write(fd, &send, sizeof(sendtype))<0 ){return -1;}
  return 0;
}



int gps::recieveinfo()
{
  if( read(fd, &recieve, sizeof(recievetype)) < 0 ){ return -1; }
  return 0;
}



int gps::show_recieveinfo()
{
  cout << recieve.str ;
  return 0;
}
