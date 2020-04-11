
//Digital Reading storage 
int l3,l2,l1,cl,cr,r1,r2,r3;

const byte serialEn = 38;        // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 40;    // Connect JPULSE of LSA08 to pin 4
char address = 0x01;    // UART address as 0x01

unsigned int junctionCount = 0;

int lasterror = 0;
int error_sum = 0;

int part_counter = 0;

void setup() {
  pinMode(serialEn,OUTPUT);   // Setting serialEn as digital output pin
  pinMode(junctionPulse,INPUT);   // Setting junctionPulse as digital input pin
 
  //Motors
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);

  //LSA DIGITAL Read pins activation
  for(int i=26;i<=33;i++){
  pinMode(i,INPUT);
  }
  
  // Setting initial condition of serialEn pin to HIGH
  digitalWrite(serialEn,HIGH);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // set the data rate for the SoftwareSerial port
  Serial3.begin(38400); //38400
  Serial3.flush();   // Clear serial buffer
  
  digitalWrite(serialEn,LOW);
  clearJunction();
  sendCommand('D',2);
  Serial.flush();
  delay(1000);
}

void loop() { // run over and over
  if(part_counter == 0){
    part_A();
  }
  else if(part_counter == 1){
    //Run Part B fucntion here
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    analogWrite(9,150);
    analogWrite(11,150);
    delay(300);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    while(1);
  }
}

void part_A(){
  lsa_DIGITALREAD();
  //No line
  if(l3 == 0 && l2 == 0 && l1 == 0 && cl == 0 && cr == 0 && r1 == 0 && r2 == 0 && r3 == 0){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(300);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,150);
    analogWrite(11,150);
    delay(850);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    lsa_DIGITALREAD();
    while(r1 !=1){
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,100);
    analogWrite(11,100);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }
    
  //Bar (RIGHT)
  if(l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(200);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,150);
    analogWrite(11,150);
    delay(300);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    lsa_DIGITALREAD();
    while(r1 !=1){
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,100);
    analogWrite(11,100);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(10);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }
  //120
  if(l3 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r3 == 0){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(160);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    lsa_DIGITALREAD();
    //checking for circle 
    if(l3 == 0 || l2 == 0 || l1 == 0 || cl == 0 || cr == 0 || r1 == 0 || r2 == 0 || r3 == 0){
    digitalWrite(8,LOW);
    digitalWrite(11,LOW);
    analogWrite(9,150);
    analogWrite(10,150);
    delay(250);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    lsa_DIGITALREAD();
    while(cl!=1){
    digitalWrite(8,LOW);
    digitalWrite(11,LOW);
    analogWrite(9,100);
    analogWrite(10,100);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(10);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    }
    //Its a circle
    else{
      part_counter+=1;
    }
  }
  
  if((l3 == 0 && l2 == 0 && l1 == 0 && cl == 0 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)||
     (l3 == 0 && l2 == 0 && l1 == 0 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)||
     (l3 == 0 && l2 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0)||
     (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0)||
     (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 0 && r1 == 0 && r2 == 0 && r3 == 0)||
     (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 0 && r2 == 0 && r3 == 0)||
     (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0)||
     (l3 == 0 && l2 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0)){
    analogWrite(9,150);
    analogWrite(11,150);
    int bar = 0, degree_120 = 0,right = 0, left = 0;;
    for(int i=0;i<25;i++){
      lsa_DIGITALREAD();
      //bar
      if(l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1){
        bar+=1;
      }
      //degree_120
      if(l3 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r3 == 0){
        degree_120+=1;
      }
      //right
      if((l3 == 0 && l2 == 0 && l1 == 0 && cl == 0 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)||
         (l3 == 0 && l2 == 0 && l1 == 0 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)||
         (l3 == 0 && l2 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0)||
         (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0)){
        right+=1;
      }
      //left
      if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 0 && r1 == 0 && r2 == 0 && r3 == 0)||
         (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 0 && r2 == 0 && r3 == 0)||
         (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0)||
         (l3 == 0 && l2 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0)){
        left+=1;
      }
      delay(4);
    }//Serial.print("bar : ");Serial.print(bar);Serial.print("Right : ");Serial.print(right);Serial.print("Left : ");Serial.println(left);
       // digitalWrite(8,HIGH);
       // digitalWrite(9,HIGH);
        //digitalWrite(10,HIGH);
        //digitalWrite(11,HIGH);delay(2000);
    if(degree_120>bar && degree_120>right && degree_120>left){
        //120 degree function
        digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    lsa_DIGITALREAD();
    //checking for circle 
    if(l3 == 0 || l2 == 0 || l1 == 0 || cl == 0 || cr == 0 || r1 == 0 || r2 == 0 || r3 == 0){
    digitalWrite(8,LOW);
    digitalWrite(11,LOW);
    analogWrite(9,150);
    analogWrite(10,150);
    delay(250);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    lsa_DIGITALREAD();
    while(cl!=1){
    digitalWrite(8,LOW);
    digitalWrite(11,LOW);
    analogWrite(9,100);
    analogWrite(10,100);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(10);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    }
    //Its a circle
    else{
      part_counter+=1;
    }
      }
    if(bar>degree_120 && bar>right && bar>left){
        //barfunction
        delay(90);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,150);
    analogWrite(11,150);
    delay(300);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    lsa_DIGITALREAD();
    while(r1 !=1){
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,80);
    analogWrite(11,80);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(10);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
      }
      
    if(right>degree_120 && right>bar && right>left){
        delay(90);
        digitalWrite(8,HIGH);
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);
        digitalWrite(11,HIGH);
        delay(50);
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        analogWrite(8,100);
        analogWrite(11,100);
        delay(300);
        digitalWrite(8,HIGH);
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);
        digitalWrite(11,HIGH);
        delay(50);
        lsa_DIGITALREAD();
        while(cr !=1){
          digitalWrite(9,LOW);
          digitalWrite(10,LOW);
          analogWrite(8,80);
          analogWrite(11,80);
          lsa_DIGITALREAD();
       }
        digitalWrite(8,HIGH);
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);
        digitalWrite(11,HIGH);
        delay(10);
        digitalWrite(8,LOW);
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        digitalWrite(11,LOW);
        }
    if(left>degree_120 && left>bar && left>right){
        delay(90);
        digitalWrite(8,HIGH);
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);
        digitalWrite(11,HIGH);
        delay(50);
        digitalWrite(8,LOW);
        digitalWrite(11,LOW);
        analogWrite(9,100);
        analogWrite(10,100);
        delay(300);
        digitalWrite(8,HIGH);
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);
        digitalWrite(11,HIGH);
        delay(50);
        lsa_DIGITALREAD();
        while(cl!=1){
          digitalWrite(8,LOW);
          digitalWrite(11,LOW);
          analogWrite(9,80);
          analogWrite(10,80);
          lsa_DIGITALREAD();
        }
        digitalWrite(8,HIGH);
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);
        digitalWrite(11,HIGH);
        delay(10);
        digitalWrite(8,LOW);
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        digitalWrite(11,LOW);
        }
    }
    linefollowing_PID();
}

void linefollowing_PID(){
  //PID values
  const float kp = 9;   //9
  const float kd = 180; //180
  const float ki = 0.11; //0.1
  const int sum_resolution = 130 ; //100 to 150
  const int base_speed = 180;
  const int max_speed = 255; const int high_speed_mod = 180;
  const int setpoint = 35;

  //Line extraction 
  byte positionVal = 0;   // Declare a psition variable to store incoming data
  while(Serial3.available() <= 0);   // Wait for data to be available
  positionVal = Serial3.read();    // Read incoming data and store in dummy
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
    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
    analogWrite(11, motor_speed_1); //Serial.print(motor_speed_1); Serial.print("    ");
    analogWrite(9, motor_speed_2); //Serial.println(motor_speed_2);
  }

  //Digital Value reading and saving in global variables
void lsa_DIGITALREAD(){
  for(int i=33;i>=26;i--){
    int x = digitalRead(i);
    if(i == 33){
      l3 = x;
    }
    if(i == 32){
      l2 = x;
    }
    if(i == 31){
      l1 = x;
    }
    if(i == 30){
      cl = x;
    }
    if(i == 29){
      cr = x;
    }
    if(i == 28){
      r1 = x;
    }
    if(i == 27){
      r2 = x;
    }
    if(i == 26){
      r3 = x;
    }
  }
  //DEBUGLINE
  //Serial.print(l3);Serial.print(l2);Serial.print(l1);Serial.print(cl);Serial.print(cr);Serial.print(r1);Serial.print(r2);Serial.println(r3);
}













  
/*
 * Function to send command to LSA08 in 4 continuous bytes.
 * LSA08 will reply with "OK" for every successful command sent.
 * However, reading the reply is optional, and thus not showing here.
 */
void sendCommand(char command, char data) {
  
  char checksum = address + command + data;
  
  Serial3.print(address);
  Serial3.print(command);
  Serial3.print(data);
  Serial3.print(checksum);

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

  Serial3.write(address);
  Serial3.write(command);
  Serial3.write(data);
  Serial3.write(checksum);

  while(Serial3.available() <= 0);
  return (int(Serial3.read()));
}
// Function to clear internal junction counter of LSA08
void clearJunction() {
  char address = 0x01;
  char command = 'X';
  char data = 0x00;
  char checksum = address + command + data;

  Serial3.write(address);
  Serial3.write(command);
  Serial3.write(data);
  Serial3.write(checksum);
}

