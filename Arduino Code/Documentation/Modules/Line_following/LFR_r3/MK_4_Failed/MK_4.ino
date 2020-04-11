#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//display
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


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

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init donen  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  // Clear the buffer.
  display.clearDisplay();
  
  delay(3000);
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
  int noline=0,bar=0,right=0,left=0,degree_120=0;
  //No line
  int sum = l3 + l2 + l1 + cl + cr + r1 + r2 + r3;
  if(sum==0){
      noline = 31;
    }
  else if(sum >= 5){
    analogWrite(9,150);
    analogWrite(11,150);
  for(int i=0;i<=50;i++){
    lsa_DIGITALREAD();
    //bar
    if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)){
      bar +=1;
    }
    //120
    if((l3 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r3 == 0))
        {
      degree_120+=1;
    }
    //RIGHT
    if((l3 == 0 && l2 == 0 && l1 == 0 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)||
            (l3 == 0 && l2 == 0 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 0)||
            (l3 == 0 && l2 == 0 && l1 == 0 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)){
              delay(30);lsa_DIGITALREAD();
              //bar
              if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)){
                bar = 60;
              }
      right+=1;  
    }
    //LEFT
    if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 0 && r2 == 0 && r3 == 0)||
            (l3 == 0 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0)||
            (l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 0 && r3 == 0)){
              delay(30);lsa_DIGITALREAD();
              //bar
              if((l3 == 1 && l2 == 1 && l1 == 1 && cl == 1 && cr == 1 && r1 == 1 && r2 == 1 && r3 == 1)){
                bar = 60;
              }
      left+=1;
    }
   }delay(3);
  Serial.print("noline: ");Serial.print(noline);Serial.print(" bar : ");Serial.print(bar);Serial.print(" right : ");Serial.print(right);Serial.print(" left :");Serial.print(left);Serial.print(" 120 : ");Serial.println(degree_120);
  }
  
  if(noline>2 || bar>2 || right>2 || left>2 || degree_120>30){
    digitalWrite(8,LOW);
    digitalWrite(9,LOW); 
    digitalWrite(10,LOW);
    digitalWrite(11,LOW); delay(1000);
  //No line
  if((noline>bar)&&(noline>right)&&(noline>left)&&(noline>degree_120)){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(150);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    analogWrite(8,150);
    analogWrite(11,150);
    delay(800);
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
  else if((bar>noline)&&(bar>right)&&(bar>left)&&(bar>degree_120)){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(50);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(50);while(1);
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
  else if((degree_120>noline)&&(degree_120>bar)&&(degree_120>right)&&(degree_120>left)){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(10);
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
  //RIGHT
  else if((right>noline)&&(right>bar)&&(right>left)&&(right>degree_120)){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(150);
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
  //LEFT
  else if((left>noline)&&(left>bar)&&(left>right)&&(left>degree_120)){
    analogWrite(9,150);
    analogWrite(11,150);
    delay(150);
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
  }
    else{
      linefollowing_PID();
    }
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






//display
void testdrawchar(int i) {
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("Number of boxes :");
    display.setTextSize(3);
    display.setCursor(57,10);
    display.write(i+48);
    display.display();
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

