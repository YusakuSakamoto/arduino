#ifndef __CONTROL__
#define __CONTROL__

#define NUM_OF_ARDUINO 2           //Number of Arduino
#define AR_SEND_ID 0               //ID for send Arduino
#define AR_RECV_ID 1               //ID for recv Arduino
#define AR_ALL_ID 9                //ID for All Arduino
#define FORWARD 'F'                //Forward command
#define BACK 'B'                   //Back command 
#define RIGHT '+'                  //Turn Right command
#define LEFT '-'                   //Turn Left command
#define KEEP '@'                   //Keep the turn dir


typedef struct{
  char gear;
  unsigned char speed;
  char dir;
  unsigned char deg;
} MVcmd;

//MVcmd JScmd;

int SetJoystick(void);
void GetJSinfo2MVcmd(MVcmd*);
void CloseJoystick(void);
int Move(const char DIRflg, const unsigned char speed, const char STEERflg);

#endif
