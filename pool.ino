  #include <ESP8266WiFi.h>
  #include <NTPClient.h>
  #include <WiFiUdp.h>
  #include <RCSwitch.h>
  #include <EEPROM.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
// Replace with your network credentials
const char* ssid = "phi-rika"; // Input your wifi network name
const char* password = "cafedelapompe"; // Input your wifi password

// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
int relay1State = 0;
int relay2State = 0;
int relay3State = 0;
int relaysoffState = 0;


int virtual1State = 0;
int virtual2State = 0;
int virtual3State = 0;
int virtualoffState = 0;


//heure
int interval= 5;
int interval2= 5;
int previousCounter = 5;
int currentCounter = 50;


// RF
RCSwitch RF = RCSwitch();
int onderf = 0;
int ondebleu = 1394004;
int ondeblanche = 1397076;
int ondenoir = 1397844;
int ondelampe = 1398036;
int rfpin = 14;













int relay1wantState = 0;
int relay2wantState = 0;
int relay3wantState = 0;
int relayswantoffState = 0;
unsigned long temp = 0;
unsigned long lasttemp = 0;

// Assign output variables to GPIO pins
const int relay1 = 5; // GPIO5 D1
const int relay2 = 4; // GPIO4 D2
const int relay3 = 16;
void setup() {
  Serial.begin(115200);
  
  RF.enableReceive(14); // PIN du receiver RF
  // Initialize the output variables as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  // Set outputs to HIGH. relay active LOW
  
  // last states 
  EEPROM.begin(4);//begin eeprom communication 4values
  relay1State = EEPROM.read(1);
  virtual1State = EEPROM.read(1);
  relay2State = EEPROM.read(2);
  virtual2State = EEPROM.read(2);
  relay3State = EEPROM.read(3);
  relay3wantState = EEPROM.read(3);
  relaysoffState = EEPROM.read(4);
  Serial.println(EEPROM.read(1));
  Serial.println(EEPROM.read(2));
  Serial.println(EEPROM.read(3));
  Serial.println(EEPROM.read(4));
  digitalWrite(relay1, EEPROM.read(1));
  digitalWrite(relay2, EEPROM.read(2));
  digitalWrite(relay3, EEPROM.read(3));


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  timeClient.begin();
  timeClient.setTimeOffset(7200);
} 
void loop(){
  //WIFI reset
  currentCounter = millis();
  if (interval2 > 70000){
    Serial.println(relay1State);
    EEPROM.write(1, relay1State); 
    EEPROM.write(2, relay2State); 
    EEPROM.write(3, relay3State); 
    EEPROM.write(4, relaysoffState);
    EEPROM.commit();
    Serial.println("suicide");
    ESP.reset();
  }
  interval = currentCounter - previousCounter;
  interval2 = interval2 + interval;
  
  previousCounter = currentCounter;
  


    //RF
  if (RF.available()) {
  // Getting RF VALUE
  onderf = RF.getReceivedValue();
  Serial.println(onderf);

  // IF value = x then do something
  if (onderf == ondebleu){
    Serial.println("ondebleu recue");
    relay1wantState = 1;
    virtual1State = 1;
    virtual2State = 0;
    virtualoffState = 0;
   }
  if (onderf == ondeblanche){
    Serial.println("onde blanche recue");
    relayswantoffState = 1;
    virtual1State = 0;
    virtual2State = 0;
    virtualoffState = 1;
   }
  if (onderf == ondenoir){
    Serial.println("onde noire recue");
    relay2wantState = 1;
    virtual1State = 0;
    virtual2State = 1;
    virtualoffState = 0;
   }
  if (onderf == ondelampe){
    Serial.println("onde lampe recue");
    if (relay3State == 1){
      relay3wantState = 0;
      delay(500);
    }
    else{
      relay3wantState = 1;
      delay(500);
    }
   }


   
    RF.resetAvailable();
  }
                                                

  

 
  
  
  // Bleu
  if (relay1wantState > 0) {
    relay1wantState = 0;
    if (relay2State == 0) {
    relaysoffState = 0;
    relay1State = 1;
    Serial.println("relay 2 was off putting relay 1 on");
    digitalWrite(relay1, 1); 
     }

  else if (relay2State > 0) {
    Serial.println("relay 2 was on putting hem off delaying 1000ms and putting relay 1 on");
    relay2State = 0;
    relaysoffState = 0;
    relay1State = 1;
    digitalWrite(relay2, 0);
    delay(1000);
    digitalWrite(relay1, 1);
              }
              

            }



           // Noir 
  if (relay2wantState > 0) {
    relay2wantState = 0;
    if (relay1State < 1) {
      Serial.println("relay 1 was off putting relay 2 on");
      relay2State = 1;
      relaysoffState = 0;
      digitalWrite(relay2, 1);
                }


    else if (relay1State > 0) {
      Serial.println("relay 1 was on putting hem off delaying 1000ms and putting relay 2 on");
      relay1State = 0;
      relaysoffState = 0;
      relay2State = 1;
      digitalWrite(relay1, 0);
      delay(1000);
      digitalWrite(relay2, 1);

              }
            }

            //Neutre
  if (relayswantoffState > 0) {
    relayswantoffState = 0;
    Serial.println("putting relays off");
    relay1State = 0;
    relay2State = 0;
    relaysoffState = 1;
    digitalWrite(relay1, 0);
    digitalWrite(relay2, 0);

            }





  //lampe
  if (relay3wantState > 0) {
    relay3State = 1;
    digitalWrite(relay3, 1);
    virtual3State = 1;
  }
  if (relay3wantState < 1) {
    relay3State = 0;
    digitalWrite(relay3, 0);
    virtual3State = 0;
    
  }










  
  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            timeClient.update();
            




             // if they go on /var of the site then putting the var to 1
            if (header.indexOf("GET /bleu") >= 0) {
              relay1wantState = 1;
              virtual1State = 1;
              virtual2State = 0;
              virtualoffState = 0;
            }
            else if (header.indexOf("GET /noir") >= 0) {
              relay2wantState = 1;
              virtual1State = 0;
              virtual2State = 1;
              virtualoffState = 0;
            }
            else if (header.indexOf("GET /neutre") >= 0) {
              relayswantoffState = 1;
              virtual1State = 0;
              virtual2State = 0;
              virtualoffState = 1;
            }
            
            else if (header.indexOf("GET /lampe/alumme") >= 0) {
              relay3wantState = 1;
              virtual3State = 1;
            }
            else if (header.indexOf("GET /lampe/eteint") >= 0) {
              relay3wantState = 0;
              virtual3State = 0;
            }





            

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println(" <head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">  <title>Volet Piscine</title>   <link rel=\"shortcut icon\"   href=\"http://limal.ml/piscine/logo.ico\" />   " );
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html, body { font-family: Helvetica; display: block; margin: 0px auto; background-color: grey;position: fixed;}");
            client.println(".button { background-color: #35d0e8; border: none; height:25vh ;width: 70vw;         position: absolute;left: 25vw;top:5vh;  border-radius: 12px; color: white; font-size: 1000%;");
            client.println("margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: black; height:25vh ;width: 90vw;      position: absolute;left: 5vw;top:65vh;border: none;   border-radius: 12px;color: white; font-size: 1000%;cursor: pointer;}");
            client.println(".button3 {background-color: white; height:25vh ;width: 90vw;  position: absolute;left: 5vw;top:35vh;border: none;   border-radius: 12px; color: black; font-size: 1000%;cursor: pointer;  }");
            
            
            client.println(".blampealumme {background-color: yellow; height:25vh ;width: 20vw;  position: absolute;left: 5vw;top:5vh;border: none;   border-radius: 12px; color: black; font-size: 1000%;cursor: pointer;  }");
            
            client.println(".blampeeteint {background-color: black; height:25vh ;width: 20vw;  position: absolute;left: 5vw;top:5vh;border: none;   border-radius: 12px; color: black; font-size: 1000%;cursor: pointer;  }");

            
            client.println(".textbox {width: 80px; border: 1px solid #333; padding: 16px 20px 0px 24px; background-image: linear-gradient(180deg, #fff, #ddd 40%, #ccc);}");
            //client.println(".titre {color: white;text-align: center;}");
            client.println("#container {width: 100%; height: 100%; margin-left: 5px; margin-top: 20px; padding: 10px; display: -webkit-flex; -webkit-justify-content: center; display: flex; justify-content: center;} ");
            
            client.println("</style></head>");

            
            // Web Page Heading
            //client.println("<body><h1 class=\"titre\" >Piscine</h1>");

            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<div id=\"container\">");
            // If the relay1State is off, it displays the ON button
            if (virtual1State < 1) {
              client.println("<a href=\"/bleu\"><button class=\"button\"></button></a></p>");
            } else {
              client.println("<a href=\"/bleu\"><button class=\"button button\">-></button></a></p>");
            }
            client.println("</div>");




            
            client.println("<div id=\"container\">");
            if (virtualoffState < 1) {
              client.println("<a href=\"/neutre\"><button class=\"button3\"></button></a></p>");
            } else {
              client.println("<a href=\"/neutre\"><button class=\"button3\">-></button></a></p>");
            }
            client.println("</div>");



            client.println("<div id=\"container\">");
            if (virtual3State < 1) {
              client.println("<a href=\"/lampe/alumme\"><button class=\"blampeeteint\"></button></a></p>");
            } else {
              client.println("<a href=\"/lampe/eteint\"><button class=\"blampealumme\"></button></a></p>");
            }
            client.println("</div>");






            
            
            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<div id=\"container\">");
            // If the relay2State is off, it displays the ON button
            if (virtual2State < 1) {
              client.println("<a href=\"/noir\"><button class=\"button2\"></button></a></p>");
            } else {
              client.println("<a href=\"/noir\"><button class=\"button button2\">-></button></a></p>");
            }
            client.println("</div>");

  

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();

    Serial.println("");
    Serial.println("");
  }
}
