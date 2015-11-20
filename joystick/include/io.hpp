/*----------------------------------------
This is normal robot device i/o setteing.
C++
----------------------------------------*/
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#ifndef __IO_HPP__
#define __IO_HPP__

class io{
public:
  int Open(const char *,const speed_t); //open serial port.
  int Close();//close serial port.
  int fd;
private:
  struct termios oldtio;
  struct termios newtio;
};

#endif
