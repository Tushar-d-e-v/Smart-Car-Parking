#include <WiFi.h>
#include <Servo.h>
#include <NTPClient.h>
#include "WiFiClientSecure.h"
#include <WiFiUdp.h>
#include <time.h>;
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID "CHENAB"
#define WLAN_PASS "44zMf3QqdU&KC3Mv"


#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 8883
#define MQTT_NAME "Adventures_of_Tintin"
#define MQTT_PASS "aio_DWtT50fMUsJHE3Op5y6ICR1zyiif"

//WiFiClient client;
WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

const char* adafruitio_root_ca = \
      "-----BEGIN CERTIFICATE-----\n"
      "MIIEjTCCA3WgAwIBAgIQDQd4KhM/xvmlcpbhMf/ReTANBgkqhkiG9w0BAQsFADBh\n"
      "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
      "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
      "MjAeFw0xNzExMDIxMjIzMzdaFw0yNzExMDIxMjIzMzdaMGAxCzAJBgNVBAYTAlVT\n"
      "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
      "b20xHzAdBgNVBAMTFkdlb1RydXN0IFRMUyBSU0EgQ0EgRzEwggEiMA0GCSqGSIb3\n"
      "DQEBAQUAA4IBDwAwggEKAoIBAQC+F+jsvikKy/65LWEx/TMkCDIuWegh1Ngwvm4Q\n"
      "yISgP7oU5d79eoySG3vOhC3w/3jEMuipoH1fBtp7m0tTpsYbAhch4XA7rfuD6whU\n"
      "gajeErLVxoiWMPkC/DnUvbgi74BJmdBiuGHQSd7LwsuXpTEGG9fYXcbTVN5SATYq\n"
      "DfbexbYxTMwVJWoVb6lrBEgM3gBBqiiAiy800xu1Nq07JdCIQkBsNpFtZbIZhsDS\n"
      "fzlGWP4wEmBQ3O67c+ZXkFr2DcrXBEtHam80Gp2SNhou2U5U7UesDL/xgLK6/0d7\n"
      "6TnEVMSUVJkZ8VeZr+IUIlvoLrtjLbqugb0T3OYXW+CQU0kBAgMBAAGjggFAMIIB\n"
      "PDAdBgNVHQ4EFgQUlE/UXYvkpOKmgP792PkA76O+AlcwHwYDVR0jBBgwFoAUTiJU\n"
      "IBiV5uNu5g/6+rkS7QYXjzkwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsG\n"
      "AQUFBwMBBggrBgEFBQcDAjASBgNVHRMBAf8ECDAGAQH/AgEAMDQGCCsGAQUFBwEB\n"
      "BCgwJjAkBggrBgEFBQcwAYYYaHR0cDovL29jc3AuZGlnaWNlcnQuY29tMEIGA1Ud\n"
      "HwQ7MDkwN6A1oDOGMWh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEds\n"
      "b2JhbFJvb3RHMi5jcmwwPQYDVR0gBDYwNDAyBgRVHSAAMCowKAYIKwYBBQUHAgEW\n"
      "HGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwDQYJKoZIhvcNAQELBQADggEB\n"
      "AIIcBDqC6cWpyGUSXAjjAcYwsK4iiGF7KweG97i1RJz1kwZhRoo6orU1JtBYnjzB\n"
      "c4+/sXmnHJk3mlPyL1xuIAt9sMeC7+vreRIF5wFBC0MCN5sbHwhNN1JzKbifNeP5\n"
      "ozpZdQFmkCo+neBiKR6HqIA+LMTMCMMuv2khGGuPHmtDze4GmEGZtYLyF8EQpa5Y\n"
      "jPuV6k2Cr/N3XxFpT3hRpt/3usU/Zb9wfKPtWpoznZ4/44c1p9rzFcZYrWkj3A+7\n"
      "TNBJE0GmP2fhXhP1D/XVfIW/h0yCJGEiV9Glm/uGOa3DXHlmbAcxSyCRraG+ZBkA\n"
      "7h4SeM6Y8l/7MBRpPCz6l8Y=\n"
      "-----END CERTIFICATE-----\n";

//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe EntryGate = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/parking.entrygate");
Adafruit_MQTT_Subscribe ExitGate = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/parking.exitgate");

//Set up the feed you're publishing to
Adafruit_MQTT_Publish CarsParked = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/feeds/parking.carsparked");
Adafruit_MQTT_Publish EntrySlot1 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/feeds/parking.entryslot1");
Adafruit_MQTT_Publish ExitSlot1 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/ExitSlot1");
Adafruit_MQTT_Publish EntrySlot2 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/EntrySlot2");
Adafruit_MQTT_Publish ExitSlot2 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/ExitSlot2");
Adafruit_MQTT_Publish EntrySlot3 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/EntrySlot3");
Adafruit_MQTT_Publish ExitSlot3 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/ExitSlot3");

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000);
Servo myservo;                          //servo as gate
Servo myservos;                               //servo as gate

int carEnter = 25;                      // entry sensor 
int carExited = 26;                   //exi sensor
int slot3 = 27;                 
int slot2 = 32;
int slot1  = 33;
int count =0; 
int CLOSE_ANGLE = 80;  // The closing angle of the servo motor arm
int OPEN_ANGLE = 0;  // The opening angle of the servo motor arm              
int  hh, mm, ss;
int pos;
int pos1;

String h, m,EntryTimeSlot1,ExitTimeSlot1, EntryTimeSlot2,ExitTimeSlot2, EntryTimeSlot3,ExitTimeSlot3;
boolean entrysensor, exitsensor,s1,s2,s3;

boolean s1_occupied = false;
boolean s2_occupied = false;
boolean s3_occupied = false;

void setup() {
  delay(1000);
  Serial.begin (115200); 
  mqtt.subscribe(&EntryGate);
  mqtt.subscribe(&ExitGate);
  timeClient.begin(); 
  myservo.attach(13);      // servo pin to D4
  myservos.attach(14);       // servo pin to D5
  pinMode(carExited, INPUT);    // ir as input
  pinMode(carEnter, INPUT);     // ir as input
  pinMode(slot1, INPUT);
  pinMode(slot2, INPUT);
  pinMode(slot3, INPUT);
                                  //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);     
  delay(1000);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(2000);                     // display ssid
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");                          // if not connected print this
    
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WLAN_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());  
  
  client.setCACert(adafruitio_root_ca);
}                                          //print local IP address


void loop() {

 MQTT_connect();
 timeClient.update();
 hh = timeClient.getHours();
 mm = timeClient.getMinutes();
 ss = timeClient.getSeconds();
 h= String(hh); 
 m= String(mm);
 h +" :" + m;
 
 entrysensor= !digitalRead(carEnter);
 exitsensor = !digitalRead(carExited);
 s1 = digitalRead(slot1);
 s2 = digitalRead(slot2);
 s3 = digitalRead(slot3);
 
   if (entrysensor == 1) {                     // if high then count and send data
   count=  count+1;                             //increment count
   myservos.write(OPEN_ANGLE);
   delay(3000);
   myservos.write(CLOSE_ANGLE);
   }
   
   if (exitsensor == 1) {                            //if high then count and send
   count= count-1;                                    //decrement count
   myservo.write(OPEN_ANGLE);
   delay(3000);
   myservo.write(CLOSE_ANGLE);
  }
  if (! CarsParked.publish(count)) {}

  if (s1 == 1 && s1_occupied == false) {                     
        Serial.println("Occupied1 ");
        Serial.println("cc");
        EntryTimeSlot1 =  h +" :" + m;
        //Serial.print("EntryTimeSlot1");
        //Serial.print(EntryTimeSlot1);
        
        s1_occupied = true;
        if (! EntrySlot1.publish((char*) EntryTimeSlot1.c_str())){}
    }
  if(s1 == 0 && s1_occupied == true) {
       Serial.println("Available1 ");
       ExitTimeSlot1 =  h +" :" + m;
       //Serial.print("ExitTimeSlot1");
       //Serial.print(ExitTimeSlot1);
     
       s1_occupied = false;
        if (! ExitSlot1.publish((char*) ExitTimeSlot1.c_str())){} 
}
  if (s2 == 1&& s2_occupied == false) {                     
      Serial.println("Occupied2 ");
      EntryTimeSlot2 =  h +" :" + m;
      //Serial.print("EntryTimeSlot2");
      //Serial.print(EntryTimeSlot2);
     
      s2_occupied = true;
      if (! EntrySlot2.publish((char*) EntryTimeSlot2.c_str())){}
    }
  if(s2 == 0 && s2_occupied == true) {
       Serial.println("Available2 ");
       ExitTimeSlot2 =  h +" :" + m;
       //Serial.print("ExitTimeSlot2");
       //Serial.print(ExitTimeSlot2);
       
       s2_occupied = false;
        if (! ExitSlot2.publish((char*) ExitTimeSlot2.c_str())){}
  }
  if (s3 == 1&& s3_occupied == false) {                     
      Serial.println("Occupied3 ");
      EntryTimeSlot3 =  h +" :" + m;
     //Serial.print("EntryTimeSlot3: ");
      //Serial.print(EntryTimeSlot3);
      s3_occupied = true;
       if (! EntrySlot3.publish((char*) EntryTimeSlot3.c_str())){}
    }
  if(s3 == 0 && s3_occupied == true) {
       Serial.println("Available3 ");
       ExitTimeSlot3 =  h +" :" + m;
       //Serial.print("ExitTimeSlot3: ");
       //Serial.print(ExitTimeSlot3);
       s3_occupied = false;
        if (! ExitSlot3.publish((char*) ExitTimeSlot3.c_str())){ }
  } 

  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
     {
    
   if (subscription == &EntryGate)
     {
      //Print the new value to the serial monitor
      Serial.println((char*) EntryGate.lastread);
     
   if (!strcmp((char*) EntryGate.lastread, "ON"))
      {
       myservos.write(OPEN_ANGLE);
       delay(3000);
       myservos.write(CLOSE_ANGLE);
    }
}
  if (subscription == &ExitGate)
     {
      //Print the new value to the serial monitor
      Serial.println((char*) EntryGate.lastread);
     
   if (!strcmp((char*) ExitGate.lastread, "ON"))
      {
       myservo.write(OPEN_ANGLE);
       delay(3000);
       myservo.write(CLOSE_ANGLE);
    }
}
}  
}
void MQTT_connect() 
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}