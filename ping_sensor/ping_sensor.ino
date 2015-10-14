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
void calc_checksum(unsigned char* confirm){
  int m=2;
  ping.data.checksum = confirm[1];
  for( m=2; confirm[m] != '*'; m++ ){
	if(confirm[m] != '$' && confirm[m] != ',')
	  ping.data.checksum = ping.data.checksum ^ confirm[m];
  }
}

//the aid array is unsigned char.
//this function cast unsigned char to hexadecimal.
//and input pack 2 byte.
void Convert_Hexadecimal(unsigned char aid, unsigned char *pack,int& count){
  if( (int)( aid/16 ) >= 10 ){
	pack[count++] = 97 + (int)(aid/16);
  }else{
	pack[count++] = 48 + (int)(aid/16);
  }
  
  if( (int)(aid%16) >= 10 ){
	pack[count++] = 87 + (int)(aid%16);
  }else{
	pack[count++] = 48 + (int)(aid%16);
  }    
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
  calc_checksum(ping.packet);//copy packet to data.checksum with sum unsigned char
  ping.packet[count++] = '*';
  
  //convert data.checksum to Hexadecimal and add pocket 
  Convert_Hexadecimal(ping.data.checksum,ping.packet,count);
  ping.packet[count++] = '\n';
  
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
