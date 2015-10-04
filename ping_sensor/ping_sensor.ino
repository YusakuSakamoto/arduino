//the body to send to pc.
unsigned char packet[30];

//structure of send information but not body.
typedef struct{
  double inches;
  double cm;
  unsigned char checksum;
}sendpc;
sendpc data = {0};

const int pingPin = 7;

void setup() {
  Serial.begin(9600);
}

void loop() {
  double duration;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  data.inches = microsecondsToInches(duration);
  data.cm = microsecondsToCentimeters(duration);

  
  int i;
  i = create_packet();
  Serial.write(packet,i);
}


// this function calc XOR unsigned char array after '$' before '*' 
void calc_checksum(unsigned char* confirm){
  int m=2;
  data.checksum = confirm[1];
  for( m=2; confirm[m] != '*'; m++ ){
	if(confirm[m] != '$' && confirm[m] != ',')
	  data.checksum = data.checksum ^ confirm[m];
  }
}

//the aid array is unsigned char.
//this function cast unsigned char to hexadecimal.
//and input pack 2 byte.
void Convert_Hexadecimal(unsigned char aid, unsigned char *pack,int count){
  switch( (int)( aid/16 ) ){
  case 10:
	pack[count++] = 'a';
	break;
  case 11:
	pack[count++] = 'b';
	break;
  case 12:
	pack[count++] = 'c';
	break;
  case 13:
	pack[count++] = 'd';
	break;
  case 14:
	pack[count++] = 'e';
	break;
  case 15:
	pack[count++] = 'f';
	break;
  default:
	pack[count++] = 48 + (int)(aid/16);
  }
  
  switch((int)(aid%16)){
  case 10:
	pack[count++] = 'a';
	break;
  case 11:
	pack[count++] = 'b';
	break;
  case 12:
	pack[count++] = 'c';
	break;
  case 13:
	pack[count++] = 'd';
	break;
  case 14:
	pack[count++] = 'e';
	break;
  case 15:
	pack[count++] = 'f';
	break;
  default:
	pack[count++] = 48 + (int)(aid%16);
	break;
  }    
}


int create_packet(){
  int index = 0;
  int inches; 
  int cm;
  int count = 0;
  data.checksum = 0;
  packet[count++] = '$';  

  //inches
  //========================================
  index = 0;
  inches = (int)data.inches;
  while(inches != 0){
    index++;
    inches = data.inches * pow( 10, -1 * index );
  }
  
  while( index > -2 ){
    if(index==0){
      packet[count++] = '.';
    }
    index--;
    inches = data.inches * pow(10,-1*index);
    data.inches = data.inches - inches * pow(10,index);  
    packet[count++] = (unsigned char)inches + 48;
  }
  //========================================
  
  packet[count++] = ',';
  
  //cm
  //==========================================
  index=0;
  cm = (int)data.cm;
  while(cm != 0){
    index++;
    cm = data.cm * pow( 10, -1 * index );
  }
  
  while( index > -2 ){
    if( index == 0){
      packet[count++] = '.';
    }
    index--;
    cm = data.cm * pow( 10, -1 * index );
    data.cm = data.cm - cm * pow( 10, index );
    packet[count++] = (unsigned char) cm + 48;
  }
  //============================================
  calc_checksum(packet);//copy packet to data.checksum with sum unsigned char
  packet[count++] = '*';
  
  //convert data.checksum to Hexadecimal and add pocket 
  Convert_Hexadecimal(data.checksum,packet,count);
  packet[count++] = '\n';
  
  return count;  
}

double microsecondsToInches(double microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

double microsecondsToCentimeters(double microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
