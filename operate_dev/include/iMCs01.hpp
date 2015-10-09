#include "config.hpp"
#include "io.hpp"
#include <sys/ioctl.h>
#include <stdexcept>

const speed_t iMCs01_baudrate = B115200;

class iMCs01:private io
{
public:
  iMCs01();
  virtual ~iMCs01();
private:
  struct uin  cmd_uin;
  struct ccmd cmd_ccmd;
  struct uout obuf;
public:
  int create_sendinfo();
  int sendinfo();
  int recieveinfo();
  int show_recieveinfo();
};
