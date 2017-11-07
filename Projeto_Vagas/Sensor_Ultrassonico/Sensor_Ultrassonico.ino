// Inclusão da biblioteca do sensor
// Essa biblioteca não vem por padrão com o arduino e 
// precisa ser instalada pelo gerenciador de bibliotecas
#include <Ultrasonic.h>

//Conexão de internet
#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>

// Inicializa o objeto do sensor ultrasônico
// Usando as portas 12 e 13 para trigger e echo
Ultrasonic ultrasonic(7, 6);

int ledRed = 3;
int ledGreen = 4;
int ledYellow = 5;

int estadoAtual = 0;
int estadoAnterior = 0;

void feedbackmsg(){
  digitalWrite (ledYellow, LOW);
  delay (100);
  digitalWrite (ledYellow, HIGH);
  delay (100);
  digitalWrite (ledYellow, LOW);
  delay (100);
  digitalWrite (ledYellow, HIGH);
  delay (100);
}

// Update these with values suitable for your network.
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xF1, 0x64};

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;

// Dados do MQTT Cloud
PubSubClient client("m14.cloudmqtt.com", 14064, callback, ethClient);

long lastReconnectAttempt = 0;

void callback(char* topic, byte* payload, unsigned int length)
{
  char* payloadAsChar = payload;
  payloadAsChar[length]=0;
  String msgRecebida = String (payloadAsChar);

  Serial.println(msgRecebida);
  Serial.println();
  Serial.println(topic);
  delay(100);
}

boolean reconnect() {
  if (client.connect("arduino", "arduino", "ard123")) {
    // Once connected, publish an announcement...
    client.publish("teste","hello world");
    // ... and resubscribe
//    client.subscribe("recebido");
  }
  Serial.println("Conectado MQTT");
  digitalWrite(ledYellow,HIGH);
//  switch (estadoAtual){
//    case 0:
//    client.publish("teste","0");
//    case 1:
//    client.publish("teste","1");
//    }

  return client.connected();
  }

void setup() {
  pinMode (ledRed, OUTPUT);
  pinMode (ledGreen, OUTPUT);
  pinMode (ledYellow, OUTPUT);

  
  // Inicializa a porta Serial
  Serial.begin(9600);
  Serial.println("Conectando...");
  delay(50);
  Ethernet.begin(mac);

  // Faz a conexão no cloud com nome do dispositivo, usuário e senha respectivamente
    if (client.connect("sensor", "sensor", "sensor"))
  {
    // Envia uma mensagem para o cloud no topic
    client.publish("teste", "hello world");
  
//   
    Serial.println("Conectado MQTT");
    delay(50);
    digitalWrite(ledYellow,HIGH);
}
}
void loop() {
  

if (!client.connected()) {
    long now = millis();
    digitalWrite(ledYellow,LOW);
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      Serial.println("Reconectando...");
      if (reconnect()) {
        lastReconnectAttempt = 0;
        
      }
    }
  } else {
    // Client connected
  client.loop();
  }
  
  // Lê o valor do sensor
  int distancia = ultrasonic.distanceRead();
  
//  Escreve o valor da distância no painel Serial
  Serial.print("Distance in CM: ");
  Serial.println(distancia);

  
// teste de distância + mandar msg MQTT
  if (distancia <= 10 && estadoAtual != estadoAnterior){
      Serial.println("Vaga ocupada");
      client.publish("teste" , "0");
      feedbackmsg();
      delay(100);
      digitalWrite(ledRed,HIGH);
      digitalWrite(ledGreen,LOW);
      estadoAtual = 0;
      }
   
   else if (distancia > 10 && estadoAtual == estadoAnterior){ 
    Serial.println("Vaga desocupada");
    client.publish("teste" , "1");
    feedbackmsg();
    delay(100);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledRed,LOW);
    estadoAtual = 1;
   }
}
