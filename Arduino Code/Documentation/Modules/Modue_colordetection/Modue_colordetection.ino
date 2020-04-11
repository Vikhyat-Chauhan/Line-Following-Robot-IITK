int S2=7,S3=6,S1=4,OUT=3,S0=2,red,green,blue;

void setup() {
   Serial.begin(9600);
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(OUT,INPUT);
 
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

void loop() {
  Serial.println(color_sensor());
}

char color_sensor(){
  digitalWrite(S3,LOW);
  digitalWrite(S2,LOW);
  red=pulseIn(OUT,LOW);
  red+=30;

  digitalWrite(S3,HIGH);
  digitalWrite(S2,HIGH);
  green=pulseIn(OUT,LOW);

  digitalWrite(S3,HIGH);
  digitalWrite(S2,LOW);
  blue=pulseIn(OUT,LOW);
  blue+=30;
  /*
  Serial.print("R=");
  Serial.print(red);
  Serial.print(", ");
  Serial.print("G=");
  Serial.print(green);
  Serial.print(", ");
  Serial.print("B=");
  Serial.print(blue);
  Serial.println(" ")*/;

  if(red<200 || blue<200 || green<200){
    if(red<green && red<blue)
  {
    return 'R';
  }
  else if(green<red && green<blue)
  {
    return 'G';
  }
  else if(blue<red && blue<green)
  {
    return 'B';
  } 
  }
  else{
    return 0;
  }
}

