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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int positionVal_C = sonar_C.ping_cm();
  int positionVal_R = sonar_R.ping_cm();
  int positionVal_L = sonar_L.ping_cm();
  Serial.print(positionVal_L);Serial.print("  ");Serial.print(positionVal_C);Serial.print("  ");Serial.println(positionVal_R);delay(100);
 
}
