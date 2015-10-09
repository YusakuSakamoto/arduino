#include "config.hpp"
#include "io.hpp"
#include <sys/ioctl.h>
#include <linux/joystick.h>


#ifndef __PS3JOYSTICK__
#define __PS3JOYSTICK__

#define NUM_OF_ARDUINO 2           //Number of Arduino
#define AR_SEND_ID 0               //ID for send Arduino
#define AR_RECV_ID 1               //ID for recv Arduino
#define AR_ALL_ID 9                //ID for All Arduino
#define FORWARD 'F'                //Forward command
#define BACK 'B'                   //Back command 
#define RIGHT '+'                  //Turn Right command
#define LEFT '-'                   //Turn Left command
#define KEEP '@'                   //Keep the turn dir
#define SENDSIZE 13                //Send data size (Byte)

#define printLOG( msg ) fprintf(stderr,"mesg : %s\nfile : %s\nline : %d\n",msg,__FILE__,__LINE__)


typedef struct{
  char gear;
  unsigned char speed;
  char dir;
  unsigned char deg;
} MVcmd;



class Ps3Joystick{
public:
  Ps3Joystick(void);
  ~Ps3Joystick(void);
  void GetJSinfo2MVcmd(void);
  int Move(void);
  
private:
  int SetJoystick(void);
  void CloseJoystick(void);
  
private:
  MVcmd JScmd;
  int JSfd;
  int *axis;
  int num_of_axis;
  int num_of_buttons;
  char *button;
  char JSname[80];
};

#endif
