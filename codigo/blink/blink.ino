

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };

IPAddress server(192, 168, 100, 3);

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
 for (int i=0;i<length;i++) {
    Serial.println((char)payload[i]); 
    char c = (char)payload[i];
    message +=c;
    
  }
  
 Serial.println("Tópico => " + String(topic) + " | Valor => " + String(message));
 ledControl(message);
 Serial.flush();

}

EthernetClient ethClient;
PubSubClient MQTT(ethClient);

void reconnect() {
  
  while (!MQTT.connected()) {
    Serial.print("Attempting MQTT connection...");
     
    if (MQTT.connect("arduinoClient")) {
      Serial.println("connected");     
      MQTT.subscribe("/makete/");
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTT.state());
      Serial.println(" try again in 5 seconds");
   
      delay(2000);
    }
  }
}




void ledControl(char cmd){
  
  switch(cmd){
    case 'q': digitalWrite(6,HIGH);  break;
    case 'w': digitalWrite(6,LOW);   break;

    case 'e': digitalWrite(7,HIGH);  break;
    case 'r': digitalWrite(7,LOW);   break;
    
    case 't': digitalWrite(8,HIGH);  break;
    case 'y': digitalWrite(8,LOW);   break;
     
    case 'u': digitalWrite(9,HIGH); break;
    case 'i': digitalWrite(9,LOW);  break;

    case 'o': digitalWrite(10,HIGH); break;
    case 'a': digitalWrite(10,LOW); break;
     
    case 's': digitalWrite(11,HIGH); break;
    case 'd': digitalWrite(11,LOW); break;
    
  }

}









void setup()
{ 
  Serial.begin(9600);
 if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  Serial.println(Ethernet.localIP());

  MQTT.setServer(server, 1883);
  MQTT.setCallback(callback);

  
  delay(150);
}

void loop()
{
  if (!MQTT.connected()) {
    reconnect();
  }
  MQTT.loop();
}
