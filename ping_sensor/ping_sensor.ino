#define pingPin 7

//===============================================
//structure of send information but not body.
typedef struct{
  double mm;
  unsigned char checksum;
}sendpc;

class Pingsensor{
 public:
  //the body to send to pc.
  unsigned char packet[30];
  sendpc data = {0};
};
//============================================

Pingsensor ping;

void setup() {
  Serial.begin(9600);
}

void loop() {
  get_mm_data();
  int i;
  i = create_packet();
  Serial.write(ping.packet,i);
}

// this function calc XOR unsigned char array after '$' before '*' 
//the aid array is unsigned char.
//this function cast unsigned char to hexadecimal.
//and input pack 2 byte.
void Convert_Hexadecimal(unsigned char *pack,int& count){
  int m;
  ping.data.checksum = pack[1];
  for( m=2; pack[m] != '*'; m++ ){
	if( pack[m] != '$' && pack[m] != ',')
	  ping.data.checksum = ping.data.checksum ^ pack[m];
  }
  
  ping.packet[count++] = '*';
  
  if( (int)( ping.data.checksum/16 ) >= 10 ){
	pack[count++] = 97 + (int)(ping.data.checksum/16);
  }else{
	pack[count++] = 48 + (int)(ping.data.checksum/16);
  }
  
  if( (int)(ping.data.checksum%16) >= 10 ){
	pack[count++] = 87 + (int)(ping.data.checksum%16);
  }else{
	pack[count++] = 48 + (int)(ping.data.checksum%16);
  }
    ping.packet[count++] = '\n';
}


int create_packet(){
  int index = 0;
  int inches; 
  int mm;
  int count = 0;
  ping.data.checksum = 0;
  ping.packet[count++] = '$';  

  //mm
  //==========================================
  index=0;
  mm = (int)ping.data.mm;
  while(mm != 0){
    index++;
    mm = ping.data.mm * pow( 10, -1 * index );
  }
  
  while( index > -2 ){
    if( index == 0){
      ping.packet[count++] = '.';
    }
    index--;
    mm = ping.data.mm * pow( 10, -1 * index );
    ping.data.mm = ping.data.mm - mm * pow( 10, index );
    ping.packet[count++] = (unsigned char) mm + 48;
  }
  //============================================
  //copy packet to data.checksum with sum unsigned char
  //convert data.checksum to Hexadecimal and add pocket 
  Convert_Hexadecimal(ping.packet,count);
  return count;  
}

int get_mm_data(){
  double duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  digitalWrite(pingPin, HIGH);
  duration = pulseIn(pingPin, HIGH)/2;
  ping.data.mm = duration*0.34442;
  delay(250);
}
