#include <ESP8266WiFi.h>

// Wiifii Info
const char* ssid     = ""; // Enter your WIFI name
const char* password = ""; // Enter your WIFI password
 
// Set webserver port number to 80
WiFiServer server(80);
 
// Variable to store the HTTP request
String header;
 
// Auxiliar variables to store the current output state
String output1State = "off";
 
// Assign output variables to GPIO pins
const int relay1 = 15; // GPIO15
const int relay2 = 2; // GPIO2
const int relay3 = 0; // GPIO0

int relay1State = 0;
int relay2State = 0;
int relay3State = 0;
int relaysoffState = 0;
 
// Current time
unsigned long currentTime = millis();
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
 
void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
 
  // Set outputs to LOW
  digitalWrite(relay1, HIGH);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay1, HIGH);
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
}
 
void loop() {
  delay(200); // Prevent overheating
  WiFiClient client = server.available();   // Listen for incoming clients
 
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == 'n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();




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



                          // if they go on /var of the site then putting the var to 1
            if (header.indexOf("GET /bleu") >= 0) {
              Serial.println("bleu");
              digitalWrite(relay2, HIGH);
              relay2State = 0;
              relaysoffState = 0;

              digitalWrite(relay1, LOW);
              relay1State = 1; 
            }
            else if (header.indexOf("GET /noir") >= 0) {
              Serial.println("noir");
              digitalWrite(relay1, HIGH);
              relay1State = 0;
              relaysoffState = 0;

              digitalWrite(relay2, LOW);
              relay2State = 1; 
            }
            else if (header.indexOf("GET /neutre") >= 0) {
              Serial.println("neutre");
              digitalWrite(relay1, HIGH);
              relay1State = 0; 
              digitalWrite(relay2, HIGH);
              relay2State = 0;
              relaysoffState = 1;
            }
            
            else if (header.indexOf("GET /lampe/alumme") >= 0) {
              Serial.println("lampe-alumme");
              relay3State = 1;
              digitalWrite(relay3, LOW);
            }
            else if (header.indexOf("GET /lampe/eteint") >= 0) {
              Serial.println("lampe-eteinte");
              relay3State = 0;
              digitalWrite(relay3, HIGH);
            }


            
            // Web Page Heading
            //client.println("<body><h1 class=\"titre\" >Piscine</h1>");

            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<div id=\"container\">");
            // If the relay1State is off, it displays the ON button
            if (relay1State < 1) {
              client.println("<a href=\"/bleu\"><button class=\"button\"></button></a></p>");
            } else {
              client.println("<a href=\"/bleu\"><button class=\"button button\">-></button></a></p>");
            }
            client.println("</div>");




            
            client.println("<div id=\"container\">");
            if (relaysoffState < 1) {
              client.println("<a href=\"/neutre\"><button class=\"button3\"></button></a></p>");
            } else {
              client.println("<a href=\"/neutre\"><button class=\"button3\">-></button></a></p>");
            }
            client.println("</div>");



            client.println("<div id=\"container\">");
            if (relay3State < 1) {
              client.println("<a href=\"/lampe/alumme\"><button class=\"blampeeteint\"></button></a></p>");
            } else {
              client.println("<a href=\"/lampe/eteint\"><button class=\"blampealumme\"></button></a></p>");
            }
            client.println("</div>");






            
            
            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<div id=\"container\">");
            // If the relay2State is off, it displays the ON button
            if (relay2State < 1) {
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
        } else if (c != 'r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
