#include <SoftwareSerial.h>

SoftwareSerial mySerial(13, 12); // RX, T
const byte serialEn = 11;        // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 8;    // Connect JPULSE of LSA08 to pin 4
char address = 0x01;    // UART address as 0x01

unsigned int junctionCount = 0;
unsigned int counter = 0;
boolean junction = 0;
boolean uturn = 0;

//PID values
float kp = 9;   //9
float kd = 180; //180
float ki = 0.11; //0.1
const int sum_resolution = 130 ; //100 to 150
const int base_speed = 150;
const int max_speed = 255; const int high_speed_mod = 150;
const int setpoint = 35;
int error ;
int lasterror = 0;
int error_sum = 0;

void setup() {
  pinMode(serialEn,OUTPUT);   // Setting serialEn as digital output pin
  pinMode(junctionPulse,INPUT);   // Setting junctionPulse as digital input pin
 
  //Motors
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  
  // Setting initial condition of serialEn pin to HIGH
  digitalWrite(serialEn,HIGH);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(38400); //38400
  mySerial.flush();   // Clear serial buffer

  digitalWrite(serialEn,LOW);

  clearJunction();

  digitalWrite(serialEn,HIGH);
  digitalWrite(serialEn,LOW);
  sendCommand('D',2);
  Serial.flush();
  digitalWrite(serialEn,HIGH);
  digitalWrite(serialEn,LOW);
  sendCommand('D',2);
  Serial.flush();
}

void loop() { // run over and over
  linefollowing();
  
}

void linefollowing(){
  if(counter == 110){
  sendCommand('D',2);
  Serial.flush();
  digitalWrite(serialEn,HIGH);
  digitalWrite(serialEn,LOW);
  counter = 0;
  }
  else{
    counter+=1;
  }
  //Line extraction 
  byte positionVal = 0;   // Declare a psition variable to store incomin
  digitalWrite(5,LOW);digitalWrite(6,LOW);
  while(mySerial.available() <= 0);   // Wait for data to be available
  positionVal = mySerial.read();    // Read incoming data and store in dummy

  if(digitalRead(junctionPulse)) { 
    junction = 1;
    uturn = 0;
    counter = 110;
    digitalWrite(3,LOW);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(9,LOW);
    digitalWrite(serialEn,HIGH);
    digitalWrite(serialEn,LOW);
    sendCommand('D',1);
    Serial.flush();
    digitalWrite(serialEn,HIGH);
    digitalWrite(serialEn,LOW);
    sendCommand('D',1);
    Serial.flush();
    int pos = 0;
    unsigned int frequency[2]; //{248,31,255}
    
    for(int i=0;i<5;i++){ //averager with 20 sampling rate
    while(pos != 248 && pos != 31 && pos != 255){ 
    while(mySerial.available() <= 0);   // Wait for data to be available
    pos = mySerial.read();    // Read incoming data and store in dummy
    }
    if(pos == 248){
      frequency[0]+=1;
    }
    else if(pos == 31){
      frequency[1]+=1;
    }
    else if(pos == 255){
      frequency[2]+=1;
    }
    pos = 0;
    }
    Serial.print(frequency[0]);Serial.print(" ");Serial.print(frequency[1]);Serial.print(" ");Serial.println(frequency[2]);
    //Average position calculator with frequency
    if(frequency[0] == 0 && frequency[1] == 0 && frequency[2] == 0){
     pos = NULL; 
    }//comparator
    if(frequency[0]>frequency[1]){
      if(frequency[0]>frequency[2]){
        pos = 248;//greater [0]
      }
      else{
        pos = 255;//greater[2]
      }
    }
    else{
      if(frequency[1]>frequency[2]){
        pos = 31;//greater[1]
      }
      else{
        pos = 255;//greater[2]
      }
    }
    
    if(pos == 255) {
        nineright();
     }
    else if(pos==248){  //RIGHT 252 240 224
          nineright();
        }
    else if(pos==31){  //LEFT 7, 15
          nineleft();
        }
     
    digitalWrite(serialEn,HIGH);
    digitalWrite(serialEn,LOW);
    sendCommand('D',2);
    Serial.flush();
    digitalWrite(serialEn,HIGH);
    digitalWrite(serialEn,LOW);
    sendCommand('D',2);
    Serial.flush();
  }
  else if(positionVal == 255){ 
    if(abs(int(error))<=5 &&junction == 0 && uturn == 0){
      junction = 0;
      uturn = 1;
      analogWrite(3,150);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      analogWrite(9,150);
      delay(170);
      digitalWrite(3,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(9,LOW);
      delay(100);
      analogWrite(5,150);
      digitalWrite(6,LOW);
      analogWrite(9,150);
      delay(770);
      digitalWrite(3,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(9,LOW);;
      delay(500);
   }
  }
  else {
  junction = 0;
  uturn = 0;
  //PID following
  float error = setpoint - positionVal; 
  int motor_speed_1 = base_speed - (error*kp) - (kd*(error - lasterror)) - (ki*error_sum);
  int motor_speed_2 = base_speed + (error*kp) + (kd*(error - lasterror)) + (ki*error_sum);
  lasterror = error;
  error_sum = error + error_sum; 
  if(error_sum > sum_resolution){
    error_sum = sum_resolution;
  }
  else if(error_sum < -sum_resolution){
    error_sum = -sum_resolution;
  }
  
  //motor write
  if(motor_speed_1 < 0){
    motor_speed_1 = 0;
  }
  if(motor_speed_1 > max_speed){
    motor_speed_1 = max_speed;
  }
  if(motor_speed_2 < 0){
    motor_speed_2 = 0;
  }
  if(motor_speed_2 > max_speed){
    motor_speed_2 = max_speed;
  }
  //highSpeed mod
  if(motor_speed_1 == motor_speed_2){
    motor_speed_1 = motor_speed_2 = high_speed_mod;
  }
  
    analogWrite(9, motor_speed_1); //Serial.print(motor_speed_1); Serial.print("    ");
    analogWrite(3, motor_speed_2); //Serial.println(motor_speed_2);
  }
}

void nineright(){
  analogWrite(3,150);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  analogWrite(9,150);
  delay(170);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  digitalWrite(3,LOW);
  delay(100);
  analogWrite(5,150);
  digitalWrite(6,LOW);
  analogWrite(9,150);
  delay(400);//398
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  delay(150);
}

void nineleft(){
  analogWrite(3,150);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  analogWrite(9,150);
  delay(170);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  delay(100);
  analogWrite(3,150);
  digitalWrite(5,LOW);
  analogWrite(6,150);
  digitalWrite(9,LOW);
  delay(400);//398
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  delay(150);
}

/*
 * Function to send command to LSA08 in 4 continuous bytes.
 * LSA08 will reply with "OK" for every successful command sent.
 * However, reading the reply is optional, and thus not showing here.
 */
void sendCommand(char command, char data) {
  
  char checksum = address + command + data;
  
  mySerial.print(address);
  mySerial.print(command);
  mySerial.print(data);
  mySerial.print(checksum);

}
void commands(char command, char data){
  
  if(command == 'X'){
  // Clear internal junction count of LSA08
  command = 'X';
  data = 0x00;
  sendCommand(command,data);
  }
 
  else if(command == 'S'){
  // Setting LCD contrast to 90
  command = 'S';
  sendCommand(command,data);
  }

  else if(command == 'B'){
  // Setting LCD backlight to level 5
  command = 'B';
  data = 0x0 + data;
  sendCommand(command,data);
  }

  else if(command == 'J'){
  // Setting junction width to 6
  command = 'J';
  data = 0x0 + data;
  sendCommand(command,data);
  }
  
  else if(command == 'T'){
  // Setting threshold value default : 1
  command = 'T';
  data = 0x0 + data;
  sendCommand(command,data);
  }

  else if(command == 'L'){
  // Setting line mode to Dark-On default : 2 
  command = 'L';
  data = 0x0 + data;
  sendCommand(command,data);
  }

  else if(command == 'D'){
  // Setting UART ouput to mode Default : 2
  command = 'D';
  data = 0x0 + data;
  sendCommand(command,data);
  }

  else if(command == 'C'){
  // Start calibration
  command = 'C';
  data = 0x00;
  sendCommand(command,data);
  }
}
// Function to retrieve junction count from LSA08
int getJunction() {
  char address = 0x01;
  char command = 'X';
  char data = 0x01;
  char checksum = address + command + data;

  mySerial.write(address);
  mySerial.write(command);
  mySerial.write(data);
  mySerial.write(checksum);

  while(mySerial.available() <= 0);
  return (int(mySerial.read()));
}
// Function to clear internal junction counter of LSA08
void clearJunction() {
  char address = 0x01;
  char command = 'X';
  char data = 0x00;
  char checksum = address + command + data;

  mySerial.write(address);
  mySerial.write(command);
  mySerial.write(data);
  mySerial.write(checksum);
}

