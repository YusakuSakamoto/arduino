#include "../include/config.hpp"
#include "../include/PS3.hpp"

using namespace std;
int main(int argc,char **argv)
{  
  Ps3Joystick JoyStick;
  while(1){
    JoyStick.GetJSinfo2MVcmd();
    JoyStick.Move();
  }
  return 0;
}
