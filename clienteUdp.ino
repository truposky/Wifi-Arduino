#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "common.h"
//WiFi variable setup
char ssid[] = "Robotarium";     // your network SSID (name)
char pass[] = "robotarium";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;


//UDP variables setup
unsigned int port_udp = 4242;      // local port to listen on
char packetBuffer[256]; //buffer to hold incoming packet
char  data_to_send[] = "recibido";       // a string to send back
char  data_to_ini[] = "hola soy arduino1";


/*-----------------motor setup---------------*/
const int pinENA = 6;
const int pinIN1 = 7;
const int pinIN2 = 8;

const int pinIN3 = 9;
const int pinIN4 = 10;
const int pinENB = 11;

const int waitTime = 2000;   //espera entre fases
const int speed = 200;      //velocidad de giro

const int pinMotorI[3] = { pinENA, pinIN1, pinIN2 };
const int pinMotorD[3] = { pinENB, pinIN3, pinIN4 };


/*-----------ip setup-----------------*/
IPAddress ip_arduino1(192,168,2,5);
IPAddress ip_server(192,168,2,10);
WiFiUDP Udp;  

//Time variables
unsigned long previous_timer;
unsigned long timer=10000;
//operation variables
struct appdata operation;
struct appdata *server_operation;
void setup() {
 
  motorSetup();
  Serial.begin(9600);
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  WiFi.config(ip_arduino1);
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    //Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  //printWifiStatus();


  
  //clean udp buffer
  memset (packetBuffer,'\0',MAXDATASIZE);

   hola();//señal de inicio al servidor

}

void loop() {
    int error=0;
     // start the UDP on port 4242
    if(Udp.begin(port_udp)){
      Serial.print("Udp started on port: ");
      Serial.println(port_udp);
    }else{
      Serial.println("Udp not started");
    }
     
    // if there's data available, read a packet
    
    int packetSize = Udp.parsePacket();
    if (packetSize) {//se comprueba si se recibe un paquete
       IPAddress remoteIp = Udp.remoteIP();//proviene del servidor u ordenador centra
       int numbytes = Udp.read(packetBuffer, MAXDATASIZE); //se guardan los datos en el buffer
       server_operation= (struct appdata*)&packetBuffer;
      /*se comprueba longitud*/
       if((numbytes <  HEADER_LEN) || (numbytes != server_operation->len + HEADER_LEN)){

        Serial.print("(arduino1) unidad de datos recibida de manera incompleta \n");
        error=1;//error
       }
       else{

        Serial.print("(arduino1) operacion solicitada");
        Serial.print(server_operation->op);
        switch (server_operation->op){
            case OP_SALUDO:

              break;
            case OP_MOVE_FORWARD:
                  
              break;
            case OP_MOVE_BACKWARD:

              break;
            case OP_MOVE_LEFT:

              break;

            case OP_MOVE_RIGHT:

              break;
            default:

              break;
          
        }
       }
     
  
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
/*void printWifiStatus() {
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
}*/

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
