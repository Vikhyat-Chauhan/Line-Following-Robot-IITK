#include <SoftwareSerial.h>

SoftwareSerial mySerial(13, 12); // RX, T
const byte serialEn = 11;        // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 8;    // Connect JPULSE of LSA08 to pin 4
char address = 0x01;    // UART address as 0x01

//PID values
float kp = 6;
float kd = 1;
float ki = 0;
const int base_speed = 70;
const int max_speed = 255;
const int setpoint = 35;
int error ;
int lasterror = 0;

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
  mySerial.begin(38400); 
  mySerial.flush();   // Clear serial buffer

  digitalWrite(serialEn,LOW);

  //delay(3000);
  //sendCommand('C',0);
}

void loop() { // run over and over
  linefollowing();
  // Stay here to prevent infinite loop
}

void linefollowing(){
  //Line extraction 
  byte dummy = 0;   // Declare a dummy variable to store incomin
  
  while(mySerial.available() <= 0);   // Wait for data to be available
  dummy = mySerial.read();    // Read incoming data and store in dummy
  Serial.println(dummy);
  //PID following
  float error = setpoint - dummy;
  
  int motor_speed_1 = base_speed - (error*kp) - (kd*(error - lasterror));
  int motor_speed_2 = base_speed + (error*kp) + (kd*(error - lasterror));
  lasterror = error;

  //motor write
  if(motor_speed_1 >0 && motor_speed_1< max_speed && motor_speed_2 >0 && motor_speed_2< max_speed){
    analogWrite(6, motor_speed_1);
    analogWrite(5, motor_speed_2);
  }
  
  //digitalWrite(serialEn,HIGH);    // Stop requesting for UART data
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

