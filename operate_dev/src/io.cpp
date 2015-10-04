#include "../include/io.hpp"

int io::Open(const char *port_name,const speed_t baudrate){
  tcgetattr(STDOUT_FILENO,&oldtio);

  memset(&newtio,0,sizeof(newtio));
  newtio.c_cflag = (baudrate | CS8 | CREAD | CLOCAL);
  newtio.c_iflag = (IGNPAR | ICRNL);
  newtio.c_oflag = 0;
  newtio.c_lflag = ~ICANON;
  newtio.c_cc[VMIN]=1;
  newtio.c_cc[VTIME]=5;
 
  fd = open(port_name, O_RDWR | O_NONBLOCK);
  cfsetospeed(&newtio,baudrate);
  cfsetispeed(&newtio,baudrate);
 
  tcsetattr(fd,TCSANOW,&newtio);
  return 0;
}

int io::Close(){
  close(fd);
  tcsetattr(STDOUT_FILENO,TCSANOW,&oldtio);
  fd = -1;
  return 0;
}
