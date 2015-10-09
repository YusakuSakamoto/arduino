#include "../include/config.hpp"
#include "../include/Arduino.hpp"
#include "../include/iMCs01.hpp"
#include "../include/UTM-30LX.hpp"
#include "../include/gps.hpp"
#include "../include/PS3.hpp"

using namespace std;

/*
int main(int argc,char **argv)
{
  
  Ps3Joystick JoyStick;

  while(1){
	JoyStick.GetJSinfo2MVcmd();
	JoyStick.Move();
  }
  return 0;
}
*/

//GPS
/*
int main(int argc,char **argv){
  gps GPS;

  while(1){
    
    if(GPS.recieveinfo() == 0){
	  GPS.show_recieveinfo();
    }
  }
  return 0;
}
*/





int main(int argc,char **argv){
  Arduino ARDUINO;
  while(1){
	if(ARDUINO.recieveinfo()!=-1){
	  ARDUINO.show_recieveinfo();
	  if(ARDUINO.analize_start == true) break;
	}
  }
  
  if(ARDUINO.analize_start == true){
  	fstream file;
  	file.open(analysis_file,ios::in);
  	string str;
  	while( getline(file,str) ){
  	  cout << str << endl;
  	}
  	file.close();
  }
  
  FILE *fp;
  char fname[100] = "./data/ping_sensor.dat";
  int ret, n1, n2;
  float f1[ANALYSIS_NUM];
  float f2[ANALYSIS_NUM];
  float average=0;
  float dispersion=0;
  float standard_deviation=0;
  int w=0;
  int q;

  fp = fopen( fname, "r" );
  if( fp == NULL ){
	printf( "%sファイルが開けません¥n", fname );
	return -1;
  }
  
  while( ( ret = fscanf( fp, "%d %f %f %d",&n1, &f1[w], &f2[w],&q ) ) != EOF ){
	printf( "%d %f %f %d\n",n1, f1[w], f2[w], q );
	average += f2[w];
	w++;
  }
  fclose( fp );
  average /= ANALYSIS_NUM;


  for(w=0;w<ANALYSIS_NUM;w++){
	dispersion += (pow(f2[w]-average,2));
  }
  dispersion /= ANALYSIS_NUM;
  standard_deviation = pow(dispersion,0.5);

  cout << "平均値=   " << average	        << endl
	   << "分散 =    " << dispersion	        << endl
	   << "標準偏差= " << standard_deviation  << endl;

  return 0;
}

