#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

//WiFi variable setup
char ssid[] = "Robotarium";     // your network SSID (name)
char pass[] = "robotarium";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;


//UDP variables setup
unsigned int port_udp = 8080;      // local port to listen on
char packetBuffer[256]; //buffer to hold incoming packet
char  data_to_send[] = "recibido";       // a string to send back
char  data_to_ini[] = "hola soy arduino1";

IPAddress ip_arduino1(192,168,2,5);
IPAddress ip_server(192,168,2,4);
WiFiUDP Udp;  

//Time variables
unsigned long previous_timer;
unsigned long timer=10000;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  WiFi.config(ip_arduino1);
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  // start the UDP on port 80
  if(Udp.begin(port_udp)){
    Serial.print("Udp started on port: ");
    Serial.println(port_udp);
  }else{
     Serial.println("Udp not started");
  }

  //I added the noLowPowerMode comand to be sure we are not in lowPower
  WiFi.noLowPowerMode();

  //Inizialization time variable
  previous_timer=micros();

  //clean udp buffer
  for(int i=0;i<256;i++)
    packetBuffer[i]=0;

    //se envia una señal de inicio
    Udp.begin(port_udp);
  //se empieza comunicacion
  if(Udp.beginPacket(ip_server, port_udp)){
      Udp.write(data_to_ini);
      if( Udp.endPacket()){
        Serial.println("data sent");
      }else{
        Serial.println("error sending data");
      }
    }else{
      Serial.println("error inizializing data sending");
    }
}

void loop() {
 /* Udp.begin(port_udp);
  //se empieza comunicacion
  if(Udp.beginPacket(ip_server, port_udp)){
      Udp.write(data_to_ini);
      if( Udp.endPacket()){
        Serial.println("data sent");
      }else{
        Serial.println("error sending data");
      }
    }else{
      Serial.println("error inizializing data sending");
    }*/
   
  // if there's data available, read a packet
  
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();//proviene del servidor u ordenador central
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255); 
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    //se envia una respuesta

    if(Udp.beginPacket(ip_server, port_udp)){
      Udp.write(data_to_send);
      if( Udp.endPacket()){
        Serial.println("data sent");
      }else{
        Serial.println("error sending data");
      }
    }else{
      Serial.println("error inizializing data sending");
    }
  }
  /*//Send data according to timer 
  if(micros() - previous_timer >= timer){
    previous_timer = micros();
    
    //Sending data, with checks to see if sending is successiful or not
    if(Udp.beginPacket(ip_server, port_udp)){
      Udp.write(data_to_send);
      if( Udp.endPacket()){
        Serial.println("data sent");
      }else{
        Serial.println("error sending data");
      }
    }else{
      Serial.println("error inizializing data sending");
    }*/
  
  
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print gateway's IP address:
  IPAddress ip2 = WiFi.gatewayIP();
  Serial.print("Gateway IP Address: ");
  Serial.println(ip2);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
