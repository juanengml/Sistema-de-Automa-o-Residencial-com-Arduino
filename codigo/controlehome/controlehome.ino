// Libs
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// Vars
const char* SSID = "tapodi"; // rede wifi
const char* PASSWORD = "naolembro"; // senha da rede wifi

const char* BROKER_MQTT = "192.168.100.3"; // ip/host do broker
int BROKER_PORT = 1883; // porta do broker

// prototypes
void initPins();
void initSerial();
void initWiFi();
void initMQTT();

WiFiClient espClient;
PubSubClient MQTT(espClient); // instancia o mqtt


int ledPin = D3; 
int ldrPin = 0; 
int ldrValor = 0;
Servo garagem; 



// setup
void setup() {
  initPins();
  initSerial();
  initWiFi();
  initMQTT();
}

void loop() {
  //led_ldr();
  if (!MQTT.connected()) {
    reconnectMQTT();
  }
  recconectWiFi();
  MQTT.loop();
}

// implementacao dos prototypes

void initPins() {
   pinMode(D8,OUTPUT);
   pinMode(D7,OUTPUT);
   pinMode(D6,OUTPUT);
   pinMode(D5,OUTPUT);
   pinMode(D4,OUTPUT);
   pinMode(ledPin,OUTPUT); //define a porta D3 como saída   
   garagem.attach(D2);
   pinMode(D1,OUTPUT);
}

void initSerial() {
  Serial.begin(115200);
}
void initWiFi() {
  delay(10);
  Serial.println("Conectando-se em: " + String(SSID));

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado na Rede " + String(SSID) + " | IP => ");
  Serial.println(WiFi.localIP());
}

// Funcão para se conectar ao Broker MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

//Função que recebe as mensagens publicadas
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.println("Tópico => " + String(topic) + " | Valor => " + String(message));
  ledControl(message[0]);
  motor(message[0]);
  Serial.flush();
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.println("Tentando se conectar ao Broker MQTT: " + String(BROKER_MQTT));
    if (MQTT.connect("ESP8266-ESP12-E")) {
      Serial.println("Conectado");
      MQTT.subscribe("/makete/");

    } else {
      Serial.println("Falha ao Reconectar");
      Serial.println("Tentando se reconectar em 2 segundos");
      delay(2000);
    }
  }
}

void recconectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
}






void led_ldr(){
   ldrValor = analogRead(ldrPin);
   if (ldrValor>= 800) digitalWrite(ledPin,HIGH);
   else digitalWrite(ledPin,LOW);
   Serial.println(ldrValor);
}





void ledControl(char cmd){
  
  switch(cmd){
    case 'q': digitalWrite(D8,HIGH);  break;
    case 'w': digitalWrite(D8,LOW);   break;

    case 'e': digitalWrite(D7,HIGH);  break;
    case 'r': digitalWrite(D7,LOW);   break;
    
    case 't': digitalWrite(D6,HIGH);  break;
    case 'y': digitalWrite(D6,LOW);   break;
     
    case 'u': digitalWrite(D5,HIGH); break;
    case 'i': digitalWrite(D5,LOW);  break;

    case 'o': digitalWrite(D4,HIGH); break;
    case 'a': digitalWrite(D4,LOW); break;
     
    break;
  }

}




void motor(char cmd){
  int pos = 0;
  switch(cmd){
    case 'l': 
     Serial.print("Portao Aberto\n"); 
     for (pos = 2; pos <= 90; pos += 1) { 
      garagem.write(pos);
      Serial.println(pos);
      delay(15);         
     }
    break;
    case 'k': 
     Serial.print("Portao Fechado\n");
     for (pos = 90; pos >= 2; pos -= 1) { 
      garagem.write(pos);              
      Serial.println(pos);
      delay(15);                       
  }
    break;
  }
}






