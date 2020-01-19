#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include <ESP8266WiFi.h>



#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1            D1
ESP8266WebServer Server;
AutoConnect portal(Server);
//#define WLAN_SSID       "CHENAB"             // Your SSID
//#define WLAN_PASS       "9810762494"        // Your password
void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}



#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "parasgoyal13"            // Replace it with your username
#define AIO_KEY         ""   // Replace with your Project Auth Key



// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/servo"); // FeedName

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);
  pinMode(D3,OUTPUT);

  // Connect to WiFi access point.
//  Serial.println(); Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(WLAN_SSID);

//  WiFi.begin(WLAN_SSID, WLAN_PASS);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
////    Serial.print(".");
//  }
//  Serial.println();
//
//  Serial.println("WiFi connected");
//  Serial.println("IP address: "); 
//  Serial.println(WiFi.localIP());
// digitalWrite(Relay1,1);

  Server.on("/", rootPage);
  // Setup MQTT subscription for onoff feed.
    if (portal.begin()) {
    Serial.println("connected:" + WiFi.SSID());
    Serial.println("IP:" + WiFi.localIP().toString());
  } else {
    Serial.println("connection failed:" + String(WiFi.status()));
    while (1) {
      delay(100);
      yield();
    }
  }
  mqtt.subscribe(&Light1);
}

void loop() {
 
  MQTT_connect();
  digitalWrite(D3,HIGH);
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &Light1) {
//      Serial.print(F("Got: "));
      String a=(char *)Light1.lastread;
//      Serial.println(a[0]+a.substring(1));
      if(a=="Address"){Serial.println(WiFi.localIP());
      continue;}
      Serial.write((char *)Light1.lastread);
      int Light1_State = (a.substring(1).toInt());
//      Serial.println(Light1_State);
//      digitalWrite(Relay1, (Light1_State));
      
    }
  }
  portal.handleClient();
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

//  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
//    Serial.println(mqtt.connectErrorString(ret));
//    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
// mqtt.publish(AIO_USERNAME"/feeds/test",(WiFi.SSID());
// mqtt.publish(AIO_USERNAME"/feeds/test",{'ip':WiFi.localIP()});
}
