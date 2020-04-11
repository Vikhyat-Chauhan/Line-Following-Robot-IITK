#include <SoftwareSerial.h>

SoftwareSerial mySerial(13, 12); // RX, T
const byte serialEn = 11;        // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 8;    // Connect JPULSE of LSA08 to pin 4
char address = 0x01;    // UART address as 0x01

unsigned int junctionCount = 0;

unsigned int nolinecounter = 0;

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

  sendCommand('D',2);
}

void loop() { // run over and over
 float volt1 = (analogRead(A0)*(5.0/1023.0));
 int volt2 = int((volt1)*10);
 float voltage = float(volt2)/10;
 int pos = (voltage*70)/4.5;
 Serial.println(pos);
 delay(1000);
}

void linefollowing(){
  
  if(digitalRead(junctionPulse)) { 
    edges();
  }
  else{
    lines();
  }
}

void lines(){
 
  //Line extraction 
  byte positionVal = 0;   // Declare a psition variable to store incomin
  digitalWrite(5,LOW);digitalWrite(6,LOW);
  while(mySerial.available() <= 0);   // Wait for data to be available
  positionVal = mySerial.read();    // Read incoming data and store in dummy
  if(positionVal == 255){
    nolinecounter+=1;
    analogWrite(3,150);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    analogWrite(9,150);
  }
  if(nolinecounter >100){
    digitalWrite(3,LOW);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(9,LOW);
    nolinecounter = 0;
    while(1);
  }
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
    analogWrite(9, motor_speed_1); 
    analogWrite(3, motor_speed_2);
}

void edges(){
    digitalWrite(3,LOW);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(9,LOW);
    
    sendCommand('D',1);
    
    unsigned int rightf = 0; //{248,31,255}
    unsigned int leftf = 0;
    unsigned int barf = 0;
    
    for(int i=0;i<20;i++) { 
      int pos = 0;
      while(mySerial.available() <= 0);   // Wait for data to be available
      pos = mySerial.read();    // Read incoming data and store in dummy
      Serial.println(pos);
      if(pos == 248){//right
        rightf+=1;
      }
      else if(pos == 31){//left
        leftf+=1;
      }
      else if(pos == 255){//bar
        barf+=1;
      }
    }
    
    sendCommand('D',2);

    //Serial.print(leftf);Serial.print(" ");Serial.print(barf);Serial.print(" ");Serial.println(rightf);

    //Average position calculator with frequency
    if(leftf <=5 && barf <=5 && rightf <=5){
      lines();
    }
    else{
    if(leftf>barf){
      if(leftf>rightf){
        nineleft();Serial.println("LEFT");//greater left
      }
      else{
        nineright();Serial.println("RIGHT");//greater right
      }
    }
    else{
      if(barf>rightf){
        nineright();Serial.println("BAR");//greater bar
      }
      else{
        nineright();Serial.println("RIGHT");//greater right
      }
    }
    }
  }

void nineright(){
  analogWrite(3,150);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  analogWrite(9,150);
  delay(225);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  delay(50);
  digitalWrite(3,LOW);
  analogWrite(5,150);
  digitalWrite(6,LOW);
  analogWrite(9,150);
  delay(250);
  //Line extraction 
  byte positionVal = 0;   // Declare a psition variable to store incomin
  while(mySerial.available() <= 0);   // Wait for data to be available
  positionVal = mySerial.read();    // Read incoming data and store in dummy
  while(positionVal != 35){
    //Line extraction 
    while(mySerial.available() <= 0);   // Wait for data to be available
    positionVal = mySerial.read();    // Read incoming data and store in dummy
    digitalWrite(3,LOW);
    analogWrite(5,60);
    digitalWrite(6,LOW);
    analogWrite(9,60);
  }
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
}

void nineleft(){
  analogWrite(3,150);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  analogWrite(9,150);
  delay(225);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  delay(50);
  analogWrite(3,150);
  digitalWrite(5,LOW);
  analogWrite(6,150);
  digitalWrite(9,LOW);
  delay(250);
  //Line extraction 
  byte positionVal = 0;   // Declare a psition variable to store incomin
  while(mySerial.available() <= 0);   // Wait for data to be available
  positionVal = mySerial.read();    // Read incoming data and store in dummy
  while(positionVal != 35){
    //Line extraction 
    while(mySerial.available() <= 0);   // Wait for data to be available
    positionVal = mySerial.read();    // Read incoming data and store in dummy
    analogWrite(3,60);
    digitalWrite(5,LOW);
    analogWrite(6,60);
    digitalWrite(9,LOW);
  }
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
}

/*
 * Function to send command to LSA08 in 4 continuous bytes.
 * LSA08 will reply with "OK" for every successful command sent.
 * However, reading the reply is optional, and thus not showing here.
 */
void sendCommand(char command, char data) {
  
  char checksum = address + command + data;

  mySerial.flush();
  int o=0,k=0;
  while(o != 79 && k != 75){
  mySerial.print(address);
  mySerial.print(command);
  mySerial.print(data);
  mySerial.print(checksum);
  o = mySerial.read();
  k = mySerial.read();
  delay(10);
  }
  mySerial.flush();
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

