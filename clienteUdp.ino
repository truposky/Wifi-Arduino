#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

//WiFi variable setup
char ssid[] = "Robotarium";     // your network SSID (name)
char pass[] = "robotarium";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;


//UDP variables setup
unsigned int port_udp = 4242;      // local port to listen on
char packetBuffer[256]; //buffer to hold incoming packet
char  data_to_send[] = "recibido";       // a string to send back
char  data_to_ini[] = "hola soy arduino1";

//motor setup
const int pinENA = 6;
const int pinIN1 = 7;
const int pinIN2 = 8;
const int pinIN3 = 9;
const int pinIN4 = 10;
const int pinENB = 11;

const int waitTime = 2000;   //espera entre fases
const int speed = 200;      //velocidad de giro

const int pinMotorA[3] = { pinENA, pinIN1, pinIN2 };
const int pinMotorB[3] = { pinENB, pinIN3, pinIN4 };



IPAddress ip_arduino1(192,168,2,5);
IPAddress ip_server(192,168,2,4);
WiFiUDP Udp;  

//Time variables
unsigned long previous_timer;
unsigned long timer=10000;


void setup() {
  //Initialize serial and wait for port to open:
 // Serial.begin(9600);
 // while (!Serial) {
   // ; // wait for serial port to connect. Needed for native USB port only
  //}
  motorSetup();
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    //Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  WiFi.config(ip_arduino1);
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    //Serial.print("Attempting to connect to SSID: ");
    //Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
 // Serial.println("Connected to WiFi");
  printWifiStatus();

  // start the UDP on port 8080
  if(Udp.begin(port_udp)){
   // Serial.print("Udp started on port: ");
    //Serial.println(port_udp);
  }else{
  //   Serial.println("Udp not started");
  }

  //I added the noLowPowerMode comand to be sure we are not in lowPower
  //WiFi.noLowPowerMode();

  hola();//señal de inicio

  //clean udp buffer
  for(int i=0;i<256;i++)
    packetBuffer[i]=0;
    

}

void loop() {

   
  // if there's data available, read a packet
  
  int packetSize = Udp.parsePacket();
    if (packetSize) {
      
     // Serial.print("From ");
      IPAddress remoteIp = Udp.remoteIP();//proviene del servidor u ordenador central
      //Serial.print(remoteIp);
      //Serial.print(", port ");
      //Serial.println(Udp.remotePort());
      // read the packet into packetBufffer
      int len = Udp.read(packetBuffer, 255); 
      if (len > 0) {
         packetBuffer[len] = 0;
        
      }
      
    //  Serial.println("Contents:");
     // Serial.println(packetBuffer);//se muestra mensaje de servidor por puerto serie
      if (strstr(packetBuffer, "motor")){

         moveForward(pinMotorA, 180);
         moveForward(pinMotorB, 180);
      }
      if(strstr(packetBuffer, "parar")){
        fullStop(pinMotorA);
        fullStop(pinMotorB);
      }
      //se envia una respuesta
  
      if(Udp.beginPacket(ip_server, port_udp)){
          Udp.write(data_to_send);
          if( Udp.endPacket()){
       //     Serial.println("data sent");
          }else{
         //   Serial.println("error sending data");
          }
      }
      else{
          //Serial.println("error inizializing data sending");
      }
  }
 
  
  
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
 // Serial.print("SSID: ");
  //Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  //Serial.print("IP Address: ");
  //Serial.println(ip);

  // print gateway's IP address:
  IPAddress ip2 = WiFi.gatewayIP();
  //Serial.print("Gateway IP Address: ");
  //Serial.println(ip2);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
 // Serial.print("signal strength (RSSI):");
 // Serial.print(rssi);
  //Serial.println(" dBm");
}

void motorSetup(){

    pinMode(pinIN1, OUTPUT);
   pinMode(pinIN2, OUTPUT);
   pinMode(pinENA, OUTPUT);
   pinMode(pinIN3, OUTPUT);
   pinMode(pinIN4, OUTPUT);
   pinMode(pinENB, OUTPUT);
}
void hola(){
  
    //se envia una señal de inicio
    Udp.begin(port_udp);
    //se empieza comunicacion
    if(Udp.beginPacket(ip_server, port_udp)){
        Udp.write(data_to_ini);
        if( Udp.endPacket()){
        //  Serial.println("data sent");
        }else{
         // Serial.println("error sending data");
        }
    }else{
          //Serial.println("error inizializing data sending");
    }
}
void moveForward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], HIGH);
   digitalWrite(pinMotor[2], LOW);

   analogWrite(pinMotor[0], speed);
}
void fullStop(const int pinMotor[3])
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], LOW);

   analogWrite(pinMotor[0], 0);
}
