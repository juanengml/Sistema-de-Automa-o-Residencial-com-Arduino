

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };

IPAddress server(192, 168, 100, 3);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
     
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
       
      client.publish("/maquete/","Server ON ");
       
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
   
      delay(100);
    }
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
  
  

  client.setServer(server, 1883);
  client.setCallback(callback);

  
  delay(150);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
