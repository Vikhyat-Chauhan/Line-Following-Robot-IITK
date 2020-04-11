#include <NewPing.h>

#define TRIGGER_PIN_L  22  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_L     23  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_C  48  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_C     50  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_R  52  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_R     53  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE   40  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar_L(TRIGGER_PIN_L, ECHO_PIN_L, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_C(TRIGGER_PIN_C, ECHO_PIN_C, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_R(TRIGGER_PIN_R, ECHO_PIN_R, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int lasterror_L ;
int lasterror_R ;

int S2=7,S3=6,S1=4,OUT=3,S0=2,red,green,blue;

//Digital Reading storage 
int l3,l2,l1,cl,cr,r1,r2,r3;

const byte serialEn = 38;        // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 40;    // Connect JPULSE of LSA08 to pin 4
char address = 0x01;    // UART address as 0x01

unsigned int junctionCount = 0;

int lasterror = 0;
int error_sum = 0;

int part_counter = 0 ;

double distance_counter = 3.94;//14.86;
int distance_switch = 0;

void setup() {
  
  pinMode(serialEn,OUTPUT);   // Setting serialEn as digital output pin
  pinMode(junctionPulse,INPUT);   // Setting junctionPulse as digital input pin
 
  //Motors
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  //Color Sensor
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(OUT,INPUT);
  //END led
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  //color sensor
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  //IR sensor
  pinMode(2,INPUT);
  
  //LSA DIGITAL Read pins activation
  for(int i=26;i<=33;i++){
  pinMode(i,INPUT);
  }
  
  // Setting initial condition of serialEn pin to HIGH
  digitalWrite(serialEn,HIGH);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial2.begin(115200);
  while(Serial2.available()<=0){ }
  part_counter = (Serial2.read() - 48);  
  // set the data rate for the SoftwareSerial port
  Serial3.begin(38400); //38400
  Serial3.flush();   // Clear serial buffer
  
  digitalWrite(serialEn,LOW);
  clearJunction();
  sendCommand('D',2);
  Serial.flush();
}

void loop() { // run over and over
  if(part_counter == 0){
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      lsa_DIGITALREAD();
      while(l1 == 1 && cl == 1 && cr == 1 && r1 == 1){
        analogWrite(9,200);
        analogWrite(11,200);
        lsa_DIGITALREAD();
      }
      part_counter+=1;
      delay(20);
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);  
  }
  else if(part_counter == 1){ // Part_A
    part_A();
  }
  else if(part_counter == 2){
    part_B();
  }
  else if(part_counter == 3){
    part_C();
  }
  else if(part_counter == 4){
    part_D();
  }
  else{
     digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    Serial2.println(distance_counter);
    delay(10);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);  
    while(1); 
  } 
}

void part_A(){
  lsa_DIGITALREAD();
  //No line done
  if(l3 == 0 && l2 == 0 && l1 == 0 && cl == 0 && cr == 0 && r1 == 0 && r2 == 0 && r3 == 0){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(200);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,200);
    analogWrite(11,200);
    delay(390);
    lsa_DIGITALREAD();
    while(r1 !=1){
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,100);
    analogWrite(11,100);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW); 
  }
  else if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0) ||
      (l3 == 0 && l2 == 0 && l1 == 0 && cl == 0 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 0 && l2 == 0 && l1 == 0 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 0 && l2 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 0 && r1 == 0 && r2 == 0 && r3 == 0) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 0 && r2 == 0 && r3 == 0) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0) ||
      (l3 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r3 == 0) ||
      (l3 == 1 && cl == 0 && cr == 0 && r3 == 1) ||
      (((cl+cr)<=1) && ((l2+l1)>=1) &&((r1+r2)>=1) && ((l3+r3)==0))){
    analogWrite(9,50);
    analogWrite(11,50);
    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
    int node = 0,bar = 0,right = 0, left = 0,one_twenty = 0, no_line = 0;
    for(int i=0;i<200;i++){
      lsa_DIGITALREAD();
      //bar
      if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0)){
        bar+=1;
      }
      //right
      if((l3 == 0 && l2 == 0 && l1 == 0 && cl == 0 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)||
         (l3 == 0 && l2 == 0 && l1 == 0 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)||
         (l3 == 0 && l2 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)){
        right+=1;
      }
      //left
      if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 0 && r1 == 0 && r2 == 0 && r3 == 0)||
         (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 0 && r2 == 0 && r3 == 0)||
         (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0)){
        left+=1;
      }
      //120
      if((l3 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r3 == 0) ||
          (l3 == 1 && cl == 0 && cr == 0 && r3 == 1)){
            one_twenty+=1;
          }
      //node
      if(((cl+cr)<=1) && ((l2+l1)>=1) &&((r1+r2)>=1) && ((l3+r3)==0)){
        node+=1;
      }
      //noline for in node detection
      if(l3+l2+l1+cl+cr+r1+r2+r3<=1){
        no_line+=1;
      }
          
    }//Serial2.print("noline");Serial2.print(no_line);Serial2.print("Node :");Serial2.print(node);Serial2.print("one twenty :");Serial2.print(one_twenty);Serial2.print("bar : ");Serial2.print(bar);Serial2.print("Right : ");Serial2.print(right);Serial2.print("Left : ");Serial2.println(left);
     //Serial.print("bar : ");Serial.print(bar);Serial.print("Right : ");Serial.print(right);Serial.print("Left : ");Serial.println(left);
     //digitalWrite(8,HIGH);
     //digitalWrite(9,HIGH);
     //digitalWrite(10,HIGH);
     //digitalWrite(11,HIGH);delay(100000000);
    if(node>one_twenty && node>right && node>left && node>bar){
      digitalWrite(8,LOW);
      digitalWrite(10,LOW);
      analogWrite(9,200);
      analogWrite(11,200);
      delay(200);
      lsa_DIGITALREAD();
      if(no_line>0){
        Serial2.println("True Node");
      }
      else{
        Serial2.println("False Node");
      }
    }
    
   if(one_twenty>right && one_twenty>left && one_twenty>bar && one_twenty>node){
    delay(8);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    lsa_DIGITALREAD();    //checking for circle 
    //if(l3+l2+l1+cl+cr+r1+r2+r3 < 2){
    lsa_DIGITALREAD();
    while(cl!=1){
    digitalWrite(8,LOW);
    digitalWrite(11,LOW);
    analogWrite(9,100);
    analogWrite(10,100);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }
    
    if(bar>right && bar>left && bar>one_twenty && bar>node){//Serial2.println("Bar Encountered");  //done
        //barfunction 
        delay(20);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    lsa_DIGITALREAD();
    if(l3+l2+l1+cl+cr+r1+r2+r3 <= 3){
        digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,200);
    analogWrite(11,200);
    delay(200);
    lsa_DIGITALREAD();
    while(r1 !=1){
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,100);
    analogWrite(11,100);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    }
    else{//Serial2.println("circle");
      part_counter+=1;
      //Run Part B fucntion here
      lsa_DIGITALREAD();
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      while((l1 == 1 && cl == 1 && cr == 1 && r1 == 1)){
        analogWrite(9,200);
        analogWrite(11,200);
        lsa_DIGITALREAD();
      }
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
    }
      }

    if(right>bar && right>left && right>one_twenty && right>node){ //Serial2.print("Right");
      delay(100);
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        analogWrite(8,200);
        analogWrite(11,200);
        delay(120);
        lsa_DIGITALREAD();
        while(cr !=1){
          digitalWrite(9,LOW);
          digitalWrite(10,LOW);
          analogWrite(8,100);
          analogWrite(11,100);
          lsa_DIGITALREAD();
       }
        digitalWrite(8,LOW);
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        digitalWrite(11,LOW);
        }
    if(left>bar && left>right && left>one_twenty && left>node){ 
      delay(100);
        digitalWrite(8,LOW);
        digitalWrite(11,LOW);
        analogWrite(9,200);
        analogWrite(10,200);
        delay(120);
        lsa_DIGITALREAD();
        while(cl!=1){
          digitalWrite(8,LOW);
          digitalWrite(11,LOW);
          analogWrite(9,100);
          analogWrite(10,100);
          lsa_DIGITALREAD();
        }
        digitalWrite(8,LOW);
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        digitalWrite(11,LOW);
        }
    } 
    linefollowing_PID();
}

void part_B(){
    lsa_DIGITALREAD();
    if(l1 == 1 && cl == 1 && cr == 1 && r1 == 1){
      part_counter+=1;
      //Run Part B fucntion here
      lsa_DIGITALREAD();
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      while((l1 == 1 && cl == 1 && cr == 1 && r1 == 1)){
        analogWrite(9,200);
        analogWrite(11,200);
        lsa_DIGITALREAD();
      }
      delay(20);
    }
    else{
      wallfollowing_R();
    }
}

void part_C(){
    lsa_DIGITALREAD();
    if(l1 == 1 && cl == 1 && cr == 1 && r1 == 1){
      part_counter+=1;
      //Run Part B fucntion here
      lsa_DIGITALREAD();
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      while(l1 == 1 && cl == 1 && cr == 1 && r1 == 1){
        analogWrite(9,150);
        analogWrite(11,150);
        lsa_DIGITALREAD();
      }
      delay(20);
    }
    else{
      wallfollowing_L();
    }
}

void part_D(){
  lsa_DIGITALREAD();
  if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0) ||
      (l3 == 0 && l2 == 0 && l1 == 0 && cl == 0 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 0 && l2 == 0 && l1 == 0 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 0 && l2 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 0 && r1 == 0 && r2 == 0 && r3 == 0) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 0 && r2 == 0 && r3 == 0) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0) ||
      (l3 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r3 == 0) ||
      (l3 == 1 && cl == 0 && cr == 0 && r3 == 1) ||
      (((cl+cr)<=1) && ((l2+l1)>=1) &&((r1+r2)>=1) && ((l3+r3)==0))){
    analogWrite(9,50);
    analogWrite(11,50);
    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
    int node = 0,bar = 0,no_line = 0;
    for(int i=0;i<200;i++){
      lsa_DIGITALREAD();
      //bar
      if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1) ||
      (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0)){
        bar+=1;
      }
      //node
      if(((cl+cr)<=1) && ((l2+l1)>=1) &&((r1+r2)>=1) && ((l3+r3)==0)){
        node+=1;
      }
      //noline for in node detection
      if(l3+l2+l1+cl+cr+r1+r2+r3<=1){
        no_line+=1;
      }
          
    }//Serial2.print("noline");Serial2.print(no_line);Serial2.print("Node :");Serial2.print(node);Serial2.print("bar : ");Serial2.print(bar);
     //Serial.print("bar : ");Serial.print(bar);Serial.print("Right : ");Serial.print(right);Serial.print("Left : ");Serial.println(left);
     //digitalWrite(8,HIGH);
     //digitalWrite(9,HIGH);
     //digitalWrite(10,HIGH);
     //digitalWrite(11,HIGH);delay(100000000);
    if(node>bar){ //Serial2.println("Node Encountered");
      digitalWrite(8,LOW);
      digitalWrite(10,LOW);
      analogWrite(9,150);
      analogWrite(11,150);
      delay(245);//digitalWrite(8,HIGH);digitalWrite(9,HIGH);digitalWrite(10,HIGH);digitalWrite(11,HIGH);delay(100000000);
      lsa_DIGITALREAD();
      if(no_line>0){
        Serial2.println("True Node");
        if(distance_switch == 0){
          distance_switch = 1;
        }
        else{
          distance_switch = 0;
        }
      }
      else{
        Serial2.println("False Node");
      }
    }
    
    if(bar>node){//Serial2.println("Bar Encountered");  //done
        //barfunction 
        delay(20);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    lsa_DIGITALREAD();
    if(l3+l2+l1+cl+cr+r1+r2+r3 <= 3){
        digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,200);
    analogWrite(11,200);
    delay(200);
    lsa_DIGITALREAD();
    while(r1 !=1){
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,100);
    analogWrite(11,100);
    lsa_DIGITALREAD();
    }
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    }
    else{//Serial2.println("circle");
      part_counter+=1;
      //Run Part B fucntion here
      lsa_DIGITALREAD();
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      while((l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1)){
        analogWrite(9,200);
        analogWrite(11,200);
        lsa_DIGITALREAD();
             }
       }
           }
    } 
    linefollowing_PID();
    if(distance_switch == 1){
      distance_Measurment();
    }
}

void distance_Measurment(){
  int x = analogRead(A0);//Serial.println(x);
  if(x<40){
  x = analogRead(A0);
    while(!(x>=100 && x<=110)){
      x = analogRead(A0);
    }
    distance_counter+=1.1417;
  }
  /* old
  int x = 0;
  if(analogRead(A0)>200){
    x=1;
  }
  //Serial.println(x);
 if(x == 1){
    distance_counter +=1.1417;
    while(x == 1){
      if(analogRead(A0)>200){
      x=1;
  }
  else{
    x = 0;
  }
    }
  }*/
}

void linefollowing_PID(){
  //PID values
  const float kp = 13;   //9
  const float kd = 330; //180
  const float ki = 0.01; //0.1
  const int sum_resolution = 130 ; //100 to 150
  const int base_speed = 255;
  const int max_speed = 255; const int high_speed_mod = 255;
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
  Serial.print(l3);Serial.print(l2);Serial.print(l1);Serial.print(cl);Serial.print(cr);Serial.print(r1);Serial.print(r2);Serial.println(r3);
  //Serial2.print(l3);Serial2.print(l2);Serial2.print(l1);Serial2.print(cl);Serial2.print(cr);Serial2.print(r1);Serial2.print(r2);Serial2.println(r3);
}

void wallfollowing_R(){
  const float kp = 7;   //9
  const float kd = 40; //180
  const int base_speed = 190;
  const int max_speed = 255; 
  const int setpoint =9;
  
  int positionVal_C = sonar_C.ping_cm();
  int positionVal_R = sonar_R.ping_cm();
  delay(60); //29 min                 
  positionVal_C = sonar_C.ping_cm();
  positionVal_R = sonar_R.ping_cm(); 
  if((positionVal_C <12)&&(positionVal_C !=0)){
    //halt
    digitalWrite(9,LOW);
    digitalWrite(11,LOW);
    delay(100);
    //nineleft
    analogWrite(9,150);
    analogWrite(10,150);
    delay(300);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }
  else{
  if(positionVal_R == 0){ //auto nineright
  positionVal_R = 20;
  }
  float error = setpoint - positionVal_R; 
  
  int motor_speed_1 = base_speed - (error*kp) - (kd*(error - lasterror_R));
  int motor_speed_2 = base_speed + (error*kp) + (kd*(error - lasterror_R));
  lasterror_R = error;
  
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
    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
    analogWrite(11, motor_speed_1); //Serial.print(motor_speed_1); Serial.print("    ");
    analogWrite(9, motor_speed_2); //Serial.println(motor_speed_2);
  
  // Send ping, get distance in cm and print result (0 = outside set distance range)
  }
}

void wallfollowing_L(){
  const float kp = 7;   //9
  const float kd = 40; //180
  const int base_speed = 190; 
  const int max_speed = 255; 
  const int setpoint = 9;

  int positionVal_C = sonar_C.ping_cm();
  int positionVal_L = sonar_L.ping_cm();
  delay(60); //29 min                 
  positionVal_C = sonar_C.ping_cm();
  positionVal_L = sonar_L.ping_cm();
  if((positionVal_C <12)&&(positionVal_C !=0)){
    //halt
    digitalWrite(9,LOW);
    digitalWrite(11,LOW);
    delay(100);
    //nineleft
    analogWrite(8,150);
    analogWrite(11,150);
    delay(300);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }
  else{
  if(positionVal_L == 0){ //auto nineright
  positionVal_L = 20;
  }
  float error = setpoint - positionVal_L; 
  
  int motor_speed_1 = base_speed - (error*kp) - (kd*(error - lasterror_L));
  int motor_speed_2 = base_speed + (error*kp) + (kd*(error - lasterror_L));
  lasterror_L = error;

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
    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
    analogWrite(9, motor_speed_1); //Serial.print(motor_speed_1); Serial.print("    ");
    analogWrite(11, motor_speed_2); //Serial.println(motor_speed_2);
  
  // Send ping, get distance in cm and print result (0 = outside set distance range)
  }
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

