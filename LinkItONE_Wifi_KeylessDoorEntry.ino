//@CarmelitoA 24-Nov-2015 Keyless door entry project
//Creative Commons Share-alike 4.0 (https://creativecommons.org/licenses/by-sa/4.0/)
//using LinkIt One WiFi(http://www.seeedstudio.com/depot/LinkIt-ONE-p-2017.html) and Servo
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiServer.h>
#include <LWiFiClient.h>
#include <Servo.h> 

#define WIFI_AP "XXXXXXXXXXX" //CHANGE the WiFi  name
#define WIFI_PASSWORD "XXXXXXXXXXXXX" //CHANGE the WiFi password
#define WIFI_AUTH LWIFI_WEP  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP according to your WiFi AP configuration

LWiFiServer server(80);
String HTTP_req;          //stores the HTTP request
String readString;
int pos=0;
int led = 13;
Servo doorServo; 

void setup(){
  LWiFi.begin();
  //Serial.begin(115200); //CHANGE - comment out after debuing is complete using the Serial Monitor
  pinMode(led, OUTPUT);
  doorServo.attach(9);
  doorServo.write(95); //CHANGE the default servo postion in Lock //Note: You may have to change this base on you door knob setup
  // keep retrying until connected to AP
  Serial.println("Connecting to AP");
  LWiFi.connectWPA(WIFI_AP, WIFI_PASSWORD);//CJA using WPA connection per LinkIt One user guide
  printWifiStatus();
  Serial.println("Start Server");
  server.begin();
  Serial.println("Server Started");
}

void loop(){
delay(500);
//Wifi Controller
LWiFiClient client = server.available();
if (client)
  {
   while (client.connected()) {  
      if (client.available()) {
        char c = client.read();
        //read  one by one char HTTP request
        if (readString.length() < 100) {
          //storing characters to string
          readString += c;
         }
         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString);//comment out for deployment of the project
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println("Connection: close"); 
           client.println();  
           client.println("<!DOCTYPE HTML>");   
           client.println("<html>");
           client.println("<head>");
           client.println("<title>LinkIt ONE Keyless Door Opener</title>");
           client.println("<link href='https://rawgit.com/XXXXXXXXXXXXXXXXXXXXXXXXXXXXX.css' rel='stylesheet' type='text/css'>"); //CHANGE this to your css webserver link
           client.println("</head>");
           client.println("<body>");
           client.println("<h1>Keyless Door Opener</h1>");
           client.println("<br/><form method=get><table><tr><td><h2>Key Code :</h3></td>");
           client.println("<td><input type='text' name='keycode'></input></td>");  
           client.println("<td></td></tr><tr>");
           client.println("<td><button onclick='window.top.location.href='?'+ document.getElementById('keycode').value;'>Open</button></td>");  
           client.println("<td><button onclick='href='?lock'>Lock</button></td>");
           client.println("<td></td>");
           client.println("</tr></table></form>");    
           client.println("<br/>"); 
           client.println("</body>");
           client.println("</html>");
     
           delay(5);
           //stopping client
           client.stop();
           if (readString.indexOf("?keycode=linkit") >0){ //password set as linkit
            Serial.println("Inside Open Door");
            digitalWrite(led, HIGH);
            //for(pos = 50; pos < 150; pos += 1){
               doorServo.write(0);  //CHANGE OPEN the door - change the value based on you knob setup 
             //  delay(15); 
            // }
            delay(100);
           }
           else { //if (readString.indexOf("?lock") >0 || readString.indexOf("?keycode=") >0 )
            Serial.println("Lock Door");
            digitalWrite(led, LOW);                        
            doorServo.write(95);     //CHANGE LOCK  the door - change the value based on you knob setup        
            delay(100); 
           }


            //clearing string for next read
            readString="";      
         }
       }
    }
  }
  
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(LWiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = LWiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("subnet mask: ");
  Serial.println(LWiFi.subnetMask());

  Serial.print("gateway IP: ");
  Serial.println(LWiFi.gatewayIP());

  // print the received signal strength:
  long rssi = LWiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
