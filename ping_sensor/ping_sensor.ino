typedef struct{
  double inches;
  double cm;
  unsigned char checksum;
}sendpc;

const int pingPin = 7;
unsigned char packet[30];
sendpc data = {0};
  
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
  i = create_checksum();
  Serial.write(packet,i);
  //Serial.println();
  
}

void calc_checksum(unsigned char* confirm){
  int m,count,k;
  
  count = sizeof(confirm)/sizeof(unsigned char);

  data.checksum = confirm[1];
  
  for( m=2; confirm[m] != '*'; m++ ){
	if( confirm[m] != '*'){
	  if(confirm[m] != '$' && confirm[m] != ',')
		data.checksum = data.checksum ^ confirm[m];
	}
  }
}

int create_checksum(){

  int k;
  int m;
  int inches;
  int i=0;
  int cm;
  int count=0;
  data.checksum = 0;
  packet[count++] = '$';  

  //inches
  //========================================
  inches = (int)data.inches;
  while(inches != 0){
    i++;
    inches = data.inches * pow(10,-1*i);
  }
  
  while( i > -2 ){
    if(i==0){
      packet[count++] = '.';
    }
    i--;
    inches = data.inches * pow(10,-1*i);
    data.inches = data.inches - inches * pow(10,i);  
    packet[count++] = (unsigned char)inches + 48;
  }
  //========================================
  
  packet[count++] = ',';
  
  //cm
  //==========================================
  i=0;
  cm = (int)data.cm;
  while(cm != 0){
    i++;
    cm = data.cm * pow(10,-1*i);
  }
  
  while( i > -2 ){
    if(i==0){
      packet[count++] = '.';
    }
    i--;
    cm = data.cm * pow(10,-1*i);
    data.cm = data.cm - cm * pow(10,i);
    packet[count++] = (unsigned char) cm + 48;
  }
  //============================================
  calc_checksum(packet);
  packet[count++] = '*';  
  //packet[count++] = data.checksum;
  
  switch((int)(data.checksum/16)){
  case 10:
	packet[count++] = 'a';
	break;
  case 11:
	packet[count++] = 'b';
	break;
  case 12:
	packet[count++] = 'c';
	break;
  case 13:
	packet[count++] = 'd';
	break;
  case 14:
	packet[count++] = 'e';
	break;
  case 15:
	packet[count++] = 'f';
	break;
  default:
	packet[count++] = 48 + (int)(data.checksum/16);
  }
  
  switch((int)(data.checksum%16)){
  case 10:
	packet[count++] = 'a';
	break;
  case 11:
	packet[count++] = 'b';
	break;
  case 12:
	packet[count++] = 'c';
	break;
  case 13:
	packet[count++] = 'd';
	break;
  case 14:
	packet[count++] = 'e';
	break;
  case 15:
	packet[count++] = 'f';
	break;
  default:
	packet[count++] = 48 + (int)(data.checksum%16);
	break;
  }

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
