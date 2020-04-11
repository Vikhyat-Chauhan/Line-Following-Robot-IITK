int l3,l2,l1,cl,cr,r1,r2,r3;
void setup(){
  Serial.begin(115200);
  //LSA UART 
  Serial3.begin(38400);
  //LSA options
  pinMode(38,OUTPUT); // UART EN(LOW TO ENABLE)
  pinMode(40,INPUT);  //JUNCTION PULSE
  digitalWrite(38,LOW);
  //LSA DIGITAL Read pins activation
  for(int i=26;i<=33;i++){
  pinMode(i,INPUT);
  }
}

void loop(){
  lsa_DIGITALREAD();
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
}


