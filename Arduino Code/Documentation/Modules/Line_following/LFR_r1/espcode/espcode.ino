#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
char WiFiAPPSK[] = "0987654321";
char WiFINAME[] = "Super Veloce";

/////////////////////
// Pin Definitions //
/////////////////////
const int board_led = D4; //Speaker switch
//const int ANALOG_PIN = A0; // The only analog pin on the Thing

WiFiServer server(80);

void setup() 
{
  Serial.begin(115200);
  
  pinMode(board_led, OUTPUT); digitalWrite(board_led,HIGH);//default on
  
  setupWiFi();
  server.begin();
}

void loop() 
{
  Serial.println("hello.");delay(1000);
  //server_system();
}

void server_system(){
  String serial_data = "x.";
  // Check if a client has connected
  WiFiClient client = server.available();
 
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  
  if (req.indexOf("/speaker/on") != -1){
    digitalWrite(board_led, LOW); // Will write speaker ON S
    val = 1;
  }
    
  if (req.indexOf("/speaker/off") != -1){
    digitalWrite(board_led, HIGH); // Will write speaker OFF
    val = 0;
  }
    
  if (req.indexOf("/settings") != -1){
    val = 2; // Will print pin reads
  // Otherwise request will be invalid. We'll say as much in HTML
  }

  client.flush();
  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "Speaker Status : ";
  
  if(val == 1){
    s+="On";
    }
  else if(val == 0){
    s+="Off";
  }
  else if (val == 2)
  { 
    serial_data= "";
    for(int i=14;i<=req.length();i++){
      serial_data+=req[i];
      if(req[i]=='.'){
        break;
      }
    }
    Serial.println(serial_data);
  }
  else
  {
    s += "Invalid Request.<br> .";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  
  String AP_NameString = WiFINAME + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}


