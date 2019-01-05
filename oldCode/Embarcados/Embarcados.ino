#define LED1 PE_0
#define LED2 PE_1
#define LED3 PE_2
#define LED4 PE_3
#define LED5 PE_4
#define LED6 PE_5
#define LED7 PK_6
#define LED8 PM_6
#define VENT PF_1
#define MOT  PH_0
#define LUM  PK_7
#define MOV  PH_1


#include "Ethernet.h"
#include <Servo.h>

void printEthernetData();
void GetState(EthernetClient);
void SetStates(EthernetClient, String);

EthernetServer server(80);
int statusConfig = 0;
EthernetClient client;
String HTTP_req;            // Guarda pedidos HTTP
Servo myservo;

void setup() {
  Serial.begin(19200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(VENT, OUTPUT);
  pinMode(MOV, INPUT);
  pinMode(LUM, INPUT);
  myservo.attach(MOT);
  myservo.write(0);
  digitalWrite(VENT, LOW);



  Serial.println("Connecting to Ethernet....");

  Ethernet.begin(0);
  // PAra uso de IP estatico descomente o código

  // IPAddress ip = IPAddress(10, 2, 7, 158);
  /*IPAddress dns = IPAddress(10, 2, 7, 200);
    IPAddress gw = IPAddress(10, 2, 7, 254);
    IPAddress mask = IPAddress(255, 255, 0, 0);
    Ethernet.begin(0, ip, dns, gw);*/

  server.begin();
  printEthernetData();
}

void loop()
{
  client = server.available();
  
  if (client)
  { // if you get a client,
    Serial.print("new client on port ");           
    Serial.println(client.port());
    String currentLine = "";                
    boolean newConnection = true;     
    unsigned long connectionActiveTimer;  

    boolean currentLineIsBlank = true;
    while (client.connected())
    { // loop enquanto o cliente está conectado
      if (newConnection)
      {
        connectionActiveTimer = millis(); // log de quando o cliente conectou
        newConnection = false;          // não é mais uma nova conexão
      }
      if (!newConnection && connectionActiveTimer + 1000 < millis())
      {
        // Se esse loop ainda está ativo 1000ms após a conexão ser estabelecida, algo está errado
        break;  // sai do loop, indicandoq ue algo está errado
      }


      if (client.available())
      { // Se há byte para ler do cliente
        char c = client.read();             // leia um byte, então
        Serial.print(c);
        HTTP_req += c;  // guarda o pedido HTTP um caractere por vez
        // A última linha do pedido do cliente é vazia e termina com \n
        // Responder ao cliente somente após a última linha ter sido lida
        if (c == '\n' && currentLineIsBlank)
        {
          //Envia um cabeçalho padrão de resposta HTTP
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: keep-alive");
          client.println();
          // Pedido AJAX
          if (HTTP_req.indexOf("ajax_switch") > -1)
          {
            // Envia um texto apropriado
            GetState(client);
            SetStates(client, HTTP_req);
          }
          else
          { // HTTP request for web page Pedido HTTP para página web
            // Envia a página Web, contém Javascript com AJAX
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<title>Casa Inteligente - Sistemas Embarcados</title>");         
            client.println("<script>");
            client.println("var led1, x1, led2, x2, led3, x3, led4, x4, led5, x5, led6, x6, vent, x7, mot, x8;");
            client.println("function GetState() {");
            client.println("nocache = led1 + led2 + led3 + led4 + led5 + led6 + vent + mot;");
            client.println("var request = new XMLHttpRequest();");
            client.println("request.onreadystatechange = function() {");
            client.println("if (this.readyState == 4) {");
            client.println("if (this.status == 200) {");
            client.println("if (this.responseText != null) {");
            client.println("if (this.responseText.indexOf(\"LED1ON\") > -1){document.getElementById(\"LED1\").style.fill = \"red\";}");
            client.println("else {document.getElementById(\"LED1\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"LED2ON\") > -1){document.getElementById(\"LED2\").style.fill = \"red\";}");
            client.println("else {document.getElementById(\"LED2\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"LED3ON\") > -1){document.getElementById(\"LED3\").style.fill = \"red\";}");
            client.println("else {document.getElementById(\"LED3\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"LED4ON\") > -1){document.getElementById(\"LED4\").style.fill = \"red\";}");
            client.println("else {document.getElementById(\"LED4\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"LED5ON\") > -1){document.getElementById(\"LED5\").style.fill = \"red\";}");
            client.println("else {document.getElementById(\"LED5\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"LED6ON\") > -1){document.getElementById(\"LED6\").style.fill = \"red\";}");
            client.println("else {document.getElementById(\"LED6\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"VENTON\") > -1){document.getElementById(\"VENT\").style.fill = \"red\";}");
            client.println("else {document.getElementById(\"VENT\").style.fill = \"white\";}");
            client.println("if (this.responseText.indexOf(\"MOTON\") > -1){document.getElementById(\"MOT\").style.fill = \"red\";}");
            client.println("else {document.getElementById(\"MOT\").style.fill = \"white\";}");
            client.println("}}}}");
            client.println("request.open(\"GET\", \"ajax_switch\" + nocache, true);");
            client.println("request.send(null);");
            client.println("setTimeout('GetState()', 1000);");
            client.println("}");
            client.println("function SetStates(num){");
            client.println("switch(num){");
            client.println("case 1: if(x1==1){led1=\"&LED1ON\";x1=0;}else{led1=\"&LED1OFF\";x1=1;}break;");
            client.println("case 2: if(x2==1){led2=\"&LED2ON\";x2=0;}else{led2=\"&LED2OFF\";x2=1;}break;");
            client.println("case 3: if(x3==1){led3=\"&LED3ON\";x3=0;}else{led3=\"&LED3OFF\";x3=1;}break;");
            client.println("case 4: if(x4==1){led4=\"&LED4ON\";x4=0;}else{led4=\"&LED4OFF\";x4=1;}break;");
            client.println("case 5: if(x5==1){led5=\"&LED5ON\";x5=0;}else{led5=\"&LED5OFF\";x5=1;}break;");
            client.println("case 6: if(x6==1){led6=\"&LED6ON\";x6=0;}else{led6=\"&LED6OFF\";x6=1;}break;");
            client.println("case 7: if(x7==1){vent=\"&VENTON\";x7=0;}else{vent=\"&VENTOFF\";x7=1;}break;");
            client.println("case 8: if(x8==1){mot=\"&MOTON\";x8=0;}else{mot=\"&MOTOFF\";x8=1;}break;");
            client.println("case 0: led1=\"&LED1OFF\";x1=1; led2=\"&LED2OFF\";x2=1; led3=\"&LED3OFF\";x3=1; led4=\"&LED4OFF\";x4=1;led5=\"&LED5OFF\";x5=1; led6=\"&LED6OFF\";x6=1; vent=\"&VENTOFF\";x7=1; mot=\"&MOTOFF\";x8=1;break;");
            client.println("}}");
            client.println("</script>");
            client.println("</head>");
            client.println("<body onload=\"GetState();SetStates(0);\"><center>"); 
            client.println("<h1>Casa Inteligente - Sistemas Embarcados</h1>");
            client.println("<p><button type=\"L1\" onclick=\"SetStates(1);\">Lampada Sala 1</button>&nbsp;");
            client.println("<button type=\"L2\" onclick=\"SetStates(2);\">Lampada Sala 2</button>&nbsp;");
            client.println("<button type=\"L3\" onclick=\"SetStates(3);\">Lampada Quarto Lateral</button>&nbsp;");
            client.println("<button type=\"L4\" onclick=\"SetStates(4);\">Lampada Banheiro</button>&nbsp;");
            client.println("<button type=\"L5\" onclick=\"SetStates(5);\">Lampada Quarto Fundo 1</button>&nbsp;");
            client.println("<button type=\"L6\" onclick=\"SetStates(6);\">Lampada Quarto Fundo 2</button></p>&nbsp;");
            client.println("<p><center>");
            client.println("<svg width=\"75\" height=\"60\"><circle id=\"LED1\" cx=\"50\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"75\" height=\"60\"><circle id=\"LED2\" cx=\"45\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"75\" height=\"60\"><circle id=\"LED3\" cx=\"37\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"75\" height=\"60\"><circle id=\"LED4\" cx=\"33\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"75\" height=\"60\"><circle id=\"LED5\" cx=\"33\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"75\" height=\"60\"><circle id=\"LED6\" cx=\"33\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg></p></p>");
            client.println("<button type=\"VT\" onclick=\"SetStates(7);\">Ventilador</button>&nbsp;");
            client.println("<button type=\"MT\" onclick=\"SetStates(8);\">Portao</button>&nbsp;</p>");
            client.println("<svg width=\"75\" height=\"60\"><circle id=\"VENT\" cx=\"29\" cy=\"20\" r=\"10\"stroke=\"red\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"75\" height=\"60\"><circle id=\"MOT\" cx=\"20\" cy=\"20\" r=\"10\"stroke=\"red\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("</center></p>");
            client.println("</center></body>");
            client.println("</html>");
          }
          // Mostra o pedido HTTP recebido na porta serial
          Serial.print(HTTP_req);
          HTTP_req = "";            // Pedido terminado, string vazia
          break;
        }
        // Toda linha de texto recebida do cliente acaba com \r\n
        if (c == '\n')
        {
          // Último caracter recebido na linha
          // Começa nova linha com caractere lido
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          // Um texto foi recebido do cliente
          currentLineIsBlank = false;
        }
      }
    }
    // Fecha a conexão
    client.stop();
    Serial.println("client disonnected");
  }
}

// Envia o estado dos sensores para a página web
void GetState(EthernetClient cl)
{
  if (!digitalRead(MOV)) {
    digitalWrite(LED7, LOW);
  }
  else
    digitalWrite(LED7, HIGH);

  if (!digitalRead(LUM)) {
    digitalWrite(LED8, LOW);
  }
  else
    digitalWrite(LED8, HIGH);
}

void SetStates(EthernetClient cl, String HTTP_req)
{

  if (HTTP_req.indexOf("LED1ON") > -1)
  {
    digitalWrite(LED1, HIGH);
    cl.println("LED1ON");
  }
  else
  {
    digitalWrite(LED1, LOW);
    cl.println("LED1OFF");
  }

  if (HTTP_req.indexOf("LED2ON") > -1)
  {
    digitalWrite(LED2, HIGH);
    cl.println("LED2ON");
  }
  else
  {
    digitalWrite(LED2, LOW);
    cl.println("LED2OFF");
  }

  if (HTTP_req.indexOf("LED3ON") > -1)
  {
    digitalWrite(LED3, HIGH);
    cl.println("LED3ON");
  }
  else
  {
    digitalWrite(LED3, LOW);
    cl.println("LED3OFF");
  }

  if (HTTP_req.indexOf("LED4ON") > -1)
  {
    digitalWrite(LED4, HIGH);
    cl.println("LED4ON");
  }
  else
  {
    digitalWrite(LED4, LOW);
    cl.println("LED4OFF");
  }

  if (HTTP_req.indexOf("LED5ON") > -1)
  {
    digitalWrite(LED5, HIGH);
    cl.println("LED5ON");
  }
  else
  {
    digitalWrite(LED5, LOW);
    cl.println("LED5OFF");
  }

  if (HTTP_req.indexOf("LED6ON") > -1)
  {
    digitalWrite(LED6, HIGH);
    cl.println("LED6ON");
  }
  else
  {
    digitalWrite(LED6, LOW);
    cl.println("LED6OFF");
  }

  if (HTTP_req.indexOf("VENTON") > -1)
  {
    digitalWrite(VENT, HIGH);
    cl.println("VENTON");
  }
  else
  {
    digitalWrite(VENT, LOW);
    cl.println("VENTOFF");
  }

  if (HTTP_req.indexOf("MOTON") > -1)
  {
    myservo.write(0);
    cl.println("MOTON");
  }
  else
  {
    myservo.write(90);
    cl.println("MOTOFF");
  }
}


void serialEvent() {
  while (Serial.available())
  {
    // Pega o novo byte
    char inChar = (char)Serial.read();
  }
}

void printEthernetData()
{
  // Imprime o endereço IP
  Serial.println();
  Serial.println("IP Address Information:");
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address:\t");
  Serial.println(ip);

  // Imprime o endereço MAC

  IPAddress subnet = Ethernet.subnetMask();
  Serial.print("NetMask:\t");
  Serial.println(subnet);

  // Imprime o endereço de gateway
  IPAddress gateway = Ethernet.gatewayIP();
  Serial.print("Gateway:\t");
  Serial.println(gateway);

  // imprime o endereço dns
  IPAddress dns = Ethernet.dnsServerIP();
  Serial.print("DNS:\t\t");
  Serial.println(dns);

}
