const byte serialEn = 38;        // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 40;    // Connect JPULSE of LSA08 to pin 4

void setup(){
  pinMode(serialEn,OUTPUT);   // Setting serialEn as digital output pin
  pinMode(junctionPulse,INPUT);   // Setting junctionPulse as digital input pin

  // Setting initial condition of serialEn pin to HIGH
  digitalWrite(serialEn,HIGH);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // set the data rate for the SoftwareSerial port
  Serial3.begin(38400); //38400
  Serial3.flush();   // Clear serial buffer

  digitalWrite(serialEn,LOW);
  Serial.flush();
}

void loop(){
  byte positionVal = 0;   // Declare a psition variable to store incoming data
  while(Serial3.available() <= 0);   // Wait for data to be available
  positionVal = Serial3.read();    // Read incoming data and store in dummy
  Serial.print(positionVal);Serial.print("  ");Serial.println(digitalRead(junctionPulse));
}
