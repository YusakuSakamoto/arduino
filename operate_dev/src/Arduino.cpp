#include "../include/Arduino.hpp"

using namespace std;

Arduino::Arduino():
  analize_start(false),
  data_num(0)
{
  Open( arduino_serial.c_str() , Arduino_baudrate);
  record_file.open(analysis_file,ios::out);
  if(!record_file.is_open()){
	exit(-1);
  }
  recieve.counter = 0;
  recieve.flag = false;
}

Arduino::~Arduino()
{
  Close();
  record_file.close();
}

int Arduino::create_sendinfo(int i)
{
  if(i%2==0){ send.moji[0]='0';}
  else if(i%2==1){ send.moji[0]='1';}
  return 0;
}
int Arduino::sendinfo()
{
  if( write(fd, &send, sizeof(arduino_sendtype))<0 ){return -1;}
  return 0;
}
int Arduino::recieveinfo()
{
  if( read(fd, &recieve.moji, sizeof(unsigned char)) < 0 ){ return -1; }
  return 0;
}



int Arduino::show_recieveinfo()
{
  if( (('0' <= recieve.moji && recieve.moji <= '9') || ('a' <= recieve.moji && recieve.moji <= 'f') || recieve.moji == '.' || recieve.moji == '*' || recieve.moji == ',') && recieve.counter < 30 ){//データを受け取り
	input[  recieve.counter++ ] = recieve.moji;
  }else if( recieve.moji == '$'){//ヘッダー無視
  }else{
	if( recieve.moji == '\n' || recieve.moji == '\0' ){//文の終わりを発見した時
	  //データの分割==========================================
	  int l=0;
	  int y=0;
	  while(input[l++] != '\0' && l<30){
		if(recieve.flag == 2){
		  if(input[l] != '$'){
			recieve.checksum[y++] = input[l];
		  }else{
			y=0;
			recieve.flag = 0;
		  }
		}
		if( recieve.flag == 0 ){
		  if(input[l] != ','){
			recieve.inches[y++] = input[l];
		  }else{
			y=0;
			recieve.flag = 1;
		  }
		}else if( recieve.flag  == 1){
		  if(input[l] != '*'){
			recieve.cm[y++] = input[l];
		  }else{
			y=0;
			recieve.flag = 2;
		  }
		}
	  }
	  //=========================================

	  //情報を浮動小数点型、整数型に変換し、ターミナルに出力
	  //===========================================
	  char* inches = (char *)recieve.inches;
	  char* cm = (char *)recieve.cm;
	  
	  ping.inches = atof(inches);
	  ping.cm = atof(cm);
	  calc_checksum(input);
	  
	  if ((recieve.checksum[0] == ping.checksum[0]) && (recieve.checksum[1] == ping.checksum[1])){
		printf("%s .....[ ok ]\n",ping.checksum);
		//データ出力	  
		input[  recieve.counter++ ] = '\0';
		record_file << data_num+1 << "\t" <<ping.inches << "\t" << ping.cm << "\t" << 0 << endl;
		if(++data_num >= ANALYSIS_NUM){
		  analize_start = true;
		}
	  }
	  else{
		printf("%s .....[ false ]\n",ping.checksum);
	  }
	  //===========================================
	  
	  recieve.flag = 0;
	  
	  //保存用配列の初期化
	  //==========================================
	  int k=0;
	  for(int k=0;k<30;k++){
		input[k] = '\0';
	  }
	  for(k=0;k<10;k++){
		recieve.inches[k] = 0;
		recieve.cm[k] = 0;
		recieve.checksum[k] = 0;
	  }
	  //===========================================
	}
	recieve.counter = 0;
  }
  return 0;
}

void Arduino::calc_checksum(unsigned char* confirm){
  int m;
  unsigned char checksum;
  checksum = confirm[0];  
  for( m=1; confirm[m] != '*'; m++ ){
	if(confirm[m] != '$' && confirm[m] != ',')
	  checksum = checksum ^ confirm[m];
  }

  switch((int)(checksum/16)){
  case 10:
	ping.checksum[0] = 'a';
	break;
  case 11:
	ping.checksum[0] = 'b';
	break;
  case 12:
	ping.checksum[0] = 'c';
	break;
  case 13:
	ping.checksum[0] = 'd';
	break;
  case 14:
	ping.checksum[0] = 'e';
	break;
  case 15:
	ping.checksum[0] = 'f';
	break;
  default:
	ping.checksum[0] = 48 + (int)(checksum/16);
  }
  
  switch((int)(checksum%16)){
  case 10:
	ping.checksum[1] = 'a';
	break;
  case 11:
	ping.checksum[1] = 'b';
	break;
  case 12:
	ping.checksum[1] = 'c';
	break;
  case 13:
	ping.checksum[1] = 'd';
	break;
  case 14:
	ping.checksum[1] = 'e';
	break;
  case 15:
	ping.checksum[1] = 'f';
	break;
  default:
	ping.checksum[1] = 48 + (int)(checksum%16);
	break;
  }
  ping.checksum[2] = '\0';
}
