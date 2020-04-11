// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define TRIGGER_PIN_L  22  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_L     23 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_C  48  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_C     50 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_R  52  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_R     53 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE   40 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar_L(TRIGGER_PIN_L, ECHO_PIN_L, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_C(TRIGGER_PIN_C, ECHO_PIN_C, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_R(TRIGGER_PIN_R, ECHO_PIN_R, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int lasterror_L ;
int lasterror_R ;

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
}

void loop() {
  wallfollowing_R();
  int positionVal_C = sonar_C.ping_cm();
  int positionVal_R = sonar_R.ping_cm();
  int positionVal_L = sonar_L.ping_cm();
  //Serial.print(positionVal_L);Serial.print("  ");Serial.print(positionVal_C);Serial.print("  ");Serial.println(positionVal_R);
}

void wallfollowing_R(){
  const float kp = 13;   //9
  const float kd = 34; //180
  const int base_speed = 120;
  const int max_speed = 255; 
  const int setpoint = 14;
 
  delay(60); //29 min                 
  int positionVal_C = sonar_C.ping_cm();
  int positionVal_R = sonar_R.ping_cm();
  Serial.print(positionVal_C);Serial.print("  ");Serial.println(positionVal_R);
  if((positionVal_C <17)&&(positionVal_C !=0)){
    //halt
    digitalWrite(9,LOW);
    digitalWrite(11,LOW);
    delay(100);
    //nineleft
    analogWrite(9,150);
    analogWrite(10,150);
    delay(450);
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

void wallfollowing_L() {
  const float kp = 13;   //9
  const float kd = 34; //180
  const int base_speed = 120; 
  const int max_speed = 255; 
  const int setpoint =14;

  delay(60); //29 min                 
  int positionVal_C = sonar_C.ping_cm();
  int positionVal_L = sonar_L.ping_cm();
  Serial.print(positionVal_L);Serial.print("  ");Serial.println(positionVal_C);
  if((positionVal_C <17)&&(positionVal_C !=0)){
    //halt
    digitalWrite(9,LOW);
    digitalWrite(11,LOW);
    delay(100);
    //nineleft
    analogWrite(8,150);
    analogWrite(11,150);
    delay(450);
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
  
    analogWrite(9, motor_speed_1); //Serial.print(motor_speed_1); Serial.print("    ");
    analogWrite(11, motor_speed_2); //Serial.println(motor_speed_2);
  
  // Send ping, get distance in cm and print result (0 = outside set distance range)
  }
}
