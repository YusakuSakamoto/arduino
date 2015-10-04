#include "../include/iMCs01.hpp"

using namespace std;

iMCs01::iMCs01()
{
  Open( imcs01_port_name.c_str() ,iMCs01_baudrate);
  if(fd > 0){
  }else{
    throw logic_error("Faild to open pintort: imcs01");
  }
  if(ioctl(fd, URBTC_CONTINUOUS_READ) < 0){
    throw logic_error("Faild to ioctl: URBTC_CONTINUOUS_READ");
  }

  cmd_ccmd.selout     = SET_SELECT | CH0 | CH1 | CH2 | CH3; // All PWM.
  cmd_ccmd.selin      = SET_SELECT; // All input using for encoder count.

  cmd_ccmd.setoffset  = CH0 | CH1 | CH2 | CH3;
  cmd_ccmd.offset[0]  = 0x7fff;
  cmd_ccmd.offset[1]  = 0x7fff;//58981;
  cmd_ccmd.offset[2]  = 0x7fff;//58981;
  cmd_ccmd.offset[3]  = 0x7fff;//58981;	// 1/2
  cmd_ccmd.setcounter = CH0 | CH1 | CH2 | CH3;
  // cmd_ccmd.counter[1] = 29134;	//32767-67[deg]*(1453/27), initialize.
  cmd_ccmd.counter[1] = -3633;	//-67[deg]*(1453/27), initialize.
  cmd_ccmd.counter[2] = 0;
  cmd_ccmd.posneg     = SET_POSNEG | CH0 | CH1 | CH2 | CH3; //POS PWM out.
  cmd_ccmd.breaks     = SET_BREAKS | CH0 | CH1 | CH2 | CH3; //No Brake;
  cmd_ccmd.magicno    = 0x00;
  cmd_ccmd.setcounter = 0;
}

iMCs01::~iMCs01()
{
  Close();
}

int iMCs01::create_sendinfo()
{
  int i;
  cmd_ccmd.offset[0] = 0x7fff;
  cmd_ccmd.offset[1] = 0x7fff;
  cmd_ccmd.offset[2] = 0x7fff;//58981;
  cmd_ccmd.offset[3] = 0xffff;//58981;

  for(i = 0; i < 4; i++){
    obuf.ch[i].x = 0;
    obuf.ch[i].d = 0;
    obuf.ch[i].kp = 0;
    obuf.ch[i].kpx = 0;
    obuf.ch[i].kd = 0;
    obuf.ch[i].kdx = 0;
    obuf.ch[i].ki = 0;
    obuf.ch[i].kix = 0;
  }
  return 0;
}

int iMCs01::sendinfo()
{
  if(ioctl(fd, URBTC_COUNTER_SET) < 0){ return -1; }
  if(write(fd, &cmd_ccmd, sizeof(cmd_ccmd)) < 0){ return -1;}
  if(ioctl(fd, URBTC_DESIRE_SET) < 0){ return -1;}
  if(write(fd, &obuf, sizeof(obuf)) < 0){ return -1; }
  return 0;
}

int iMCs01::recieveinfo()
{
  int i=0;

  while(1) {
    if ((i = read(fd, &cmd_uin, sizeof(cmd_uin))) != sizeof(cmd_uin)) {
      fprintf(stderr, "Warning: read size mismatch (%d!=%lu).\n", i, sizeof(cmd_uin));
      continue;
    }
    for (i=0; i<4; i++) {
      cmd_uin.ad[i] = cmd_uin.ad[i] >> 5;
    }
    show_recieveinfo();
  }
  return 0;
}

int iMCs01::show_recieveinfo()
{
  printf(
	 "%d %d %d %d %d %d %d %d %d %d %d %d %d %x %x %d %d %x\n",
	 cmd_uin.time,
	 cmd_uin.ad[0],
	 cmd_uin.ad[1],
	 cmd_uin.ad[2],
	 cmd_uin.ad[3],
	 cmd_uin.ct[0],
	 cmd_uin.ct[1],
	 cmd_uin.ct[2],
	 cmd_uin.ct[3],
	 cmd_uin.da[0],
	 cmd_uin.da[1],
	 cmd_uin.da[2],
	 cmd_uin.da[3],
	 cmd_uin.din,
	 cmd_uin.dout,
	 cmd_uin.intmax,
	 cmd_uin.interval,
	 cmd_uin.magicno
	 );
  return 0;
}
