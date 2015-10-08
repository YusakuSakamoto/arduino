/*-------------------------------------------------------------------
  Name    : control.c
  Version : 1.0
  Date    : 2014/03/21
  Autor   : Ryodo Tanaka (Kyushu Institute of Technology)
  About   : Get Joy Stick (Playstation 3 DUALSHOCK 3) information
 -------------------------------------------------------------------*/
//local include
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#include "../include/PS3.hpp"

//for debug
#define printLOG( msg ) fprintf(stderr,"mesg : %s\nfile : %s\nline : %d\n",msg,__FILE__,__LINE__)

//local define
#define PORT "/dev/input/js0"            //Port path
#define SENDSIZE 13                //Send data size (Byte)

int JSfd;                                                    //File discripter
int *axis = NULL, num_of_axis = 0, num_of_buttons = 0;       //input information
char *button = NULL, JSname[80];                             //Name of button & device

/*---------------------------------------------------------------
  Name     : SetJoystick
  Argument : void
  Return   : 0 (succeed), other (failed)
  About    : Set up the Joystick controler
  Version  : Ver 1.0
  Date     : 2014/03/21
  Author   : Ryodo Tanaka (Kyushu Institute of Technology)
----------------------------------------------------------------- */
int SetJoystick(void)
{
  //File open
  if( (JSfd=open(PORT, O_RDONLY)) == -1){
    printLOG("File Open JoyStick");
    return 1;
  }

  //Get JoyStick information
  ioctl(JSfd, JSIOCGAXES, &num_of_axis);
  ioctl(JSfd, JSIOCGBUTTONS, &num_of_buttons);
  ioctl(JSfd, JSIOCGNAME(80), &JSname);

  //Get data space for axis & buttons
  axis = (int*)calloc(num_of_axis, sizeof(int));
  if(!axis){
    printLOG("calloc JoyStick axis");
    return 2;
  }
  button = (char*)calloc(num_of_buttons, sizeof(char));
  if(!button){
    printLOG("calloc JoyStick axis");
    return 3;
  }

  //Use non-blocking mode
  fcntl(JSfd, F_SETFL, O_NONBLOCK);

  printf("%s\tis Connected ...\n", JSname);
  
  return 0;

}

/*---------------------------------------------------------
  Name     : CloseJoystick
  Argument : void
  Return   : void
  About    : close file & free memory used by Joystick
  Version  : Ver 1.0
  Date     : 2014/03/21
  Author   : Ryodo Tanaka (Kyushu Insitute of Technology)
 ----------------------------------------------------------*/
void CloseJoystick(void)
{
  close(JSfd);
  free(axis);
  free(button);
}


/*--------------------------------------------------------
  Name     : GetJSinfo2MVcmd
  Argument : MVcmd *cmd
  Return   : void
  About    : Get Joystick information
             and get Move command
  Version  : Ver 1.0
  Date     : 2014/03/21
  Author   : Ryodo Tanaka (Kyushu Insitute of Technology)
-----------------------------------------------------------*/
void GetJSinfo2MVcmd(MVcmd* cmd)
{
  struct js_event JSinfo;
  
  //Read Joystick event
  while(read(JSfd, &JSinfo, sizeof(struct js_event)) == -1);

  //Get Joystick information
  switch(JSinfo.type & ~JS_EVENT_INIT)
    {
    case JS_EVENT_AXIS :
      axis[JSinfo.number] = JSinfo.value;
      break;
    case JS_EVENT_BUTTON :
      button[JSinfo.number] = JSinfo.value;
      break;
    default :
      break;
    }

  //Stop (Push both accel & brake or Don't push both)
  switch(button[8] == button[9])
    {
    case 1:
      cmd->speed = 0;
      break;
    default :
      //Get R2 button information
      switch(button[9])
	{
	case 1 :
	  cmd->gear = FORWARD;
	  cmd->speed = (float)(axis[13] + 32767) / 65534 * 220;
	  break;
	default :
	  break;
	}

      //Get L2 button information
      switch(button[8])
	{
	case 1 :
	  cmd->gear = BACK;
	  cmd->speed = (float)(axis[12] + 32767) / 65534 * 220;
	  break;
	default :
	  break;
	}
      break;
    }

  //Emergency Stop (Push R1 or R2)
  switch(button[10] | button[11])
    {
    case 1 :
      cmd->speed = 0;
      break;
    default :
      break;
    }


  //Get Left Stick information
  switch(axis[0])
    {
    case 0 :
      cmd->dir = KEEP;
      break;
    default :
      if(axis[0] < 0){
	cmd->dir = LEFT;
      }
      else if(axis[0] > 0){
	cmd->dir = RIGHT;
      }
      break;
    }

}

int Move(const char DIRflg, const unsigned char speed, const char STEERflg)
{
  char buf[SENDSIZE] = {'.'};

  if(DIRflg != 'F' && DIRflg != 'B') return 1;
  else if(speed < 0 || 220 < speed) return 2;
  else if(STEERflg != '+' && STEERflg != '-' && STEERflg != '@') return 3;
  else{
    printf("$MV,%c%d%d%d,S%c\n;", DIRflg, speed/100,speed/10%10, speed%10 ,STEERflg);
    //while(write(ARfd[AR_SEND_ID], buf, SENDSIZE) != SENDSIZE);
  }
  return 0;
}
