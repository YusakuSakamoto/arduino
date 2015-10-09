#include "../include/PS3.hpp"

Ps3Joystick::Ps3Joystick(void):
  axis(NULL),
  num_of_axis(0),
  num_of_buttons(0),
  button(NULL)
{
  JScmd.gear = FORWARD;
  JScmd.speed = 0;
  JScmd.dir =  KEEP;
  JScmd.deg = 0;
  
  SetJoystick();
}

Ps3Joystick::~Ps3Joystick(void){
  CloseJoystick();
}

int Ps3Joystick::SetJoystick(void)
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

void Ps3Joystick::CloseJoystick(void)
{
  close(JSfd);
  free(axis);
  free(button);
}


void Ps3Joystick::GetJSinfo2MVcmd(void)
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
      JScmd.speed = 0;
      break;
    default :
      //Get R2 button information
      switch(button[9])
		{
		case 1 :
		  JScmd.gear = FORWARD;
		  JScmd.speed = (float)(axis[13] + 32767) / 65534 * 220;
		  break;
		default :
		  break;
		}

      //Get L2 button information
      switch(button[8])
		{
		case 1 :
		  JScmd.gear = BACK;
		  JScmd.speed = (float)(axis[12] + 32767) / 65534 * 220;
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
      JScmd.speed = 0;
      break;
    default :
      break;
    }


  //Get Left Stick information
  switch(axis[0])
    {
    case 0 :
      JScmd.dir = KEEP;
      break;
    default :
      if(axis[0] < 0){
		JScmd.dir = LEFT;
      }
      else if(axis[0] > 0){
		JScmd.dir = RIGHT;
      }
      break;
    }
}

int Ps3Joystick::Move(void)
{
  char buf[SENDSIZE] = {'.'};

  if(JScmd.gear != FORWARD && JScmd.gear != BACK ) return 1;
  else if(JScmd.speed < 0 || 220 < JScmd.speed) return 2;
  else if(JScmd.dir != RIGHT && JScmd.dir != LEFT && JScmd.dir != KEEP) return 3;
  else{
    printf("$MV,%c%d%d%d,S%c\n;", JScmd.gear, JScmd.speed/100,JScmd.speed/10%10, JScmd.speed%10 ,JScmd.dir);
  }
  return 0;
}
