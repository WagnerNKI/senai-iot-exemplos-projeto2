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

int estadoAtual = 2;


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
PubSubClient client("192.168.3.186", 1883, callback, ethClient);

long lastReconnectAttempt = 0;

void callback(char* topic, byte* payload, unsigned int length)
{
}

boolean reconnect(int estadoAtual) {
  if (client.connect("","","","vagas/25",0,true,"")) {
    // Once connected, publish an announcement...
    client.publish("teste","hello world");
//    publica último estado
    if (estadoAtual == 2){
        client.publish("vagas/25" , "1", true);
    }
      else if (estadoAtual == 3){
        client.publish("vagas/25" , "0", true);
      }
    // ... and resubscribe
//    client.subscribe("recebido");
  }
  digitalWrite(ledYellow,HIGH);
  Serial.println("Conectado MQTT");
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
    if (client.connect("","","","vagas/25",0,true,""))
  {
    // Envia uma mensagem para o cloud no topic
    client.publish("vagas/25", "teste");
    client.publish("vagas/25" , "1", true);
    estadoAtual = 2;
    
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
      if (reconnect(estadoAtual)) {
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


  
// teste de distância + mandar msg MQTT
// 2 é livre, 3 é ocupado
  if (distancia <= 5 && estadoAtual == 2){
      Serial.println("Vaga ocupada");
      client.publish("vagas/25" , "0", true);
      feedbackmsg();
      delay(100);
      digitalWrite(ledRed,HIGH);
      digitalWrite(ledGreen,LOW);
      estadoAtual = 3;
      Serial.print("Distance in CM: ");
      Serial.println(distancia);  
      }
   
   else if (distancia > 5 && estadoAtual == 3){ 
    Serial.println("Vaga desocupada");
    client.publish("vagas/25" , "1", true);
    feedbackmsg();
    delay(100);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledRed,LOW);
    estadoAtual = 2;
    Serial.print("Distance in CM: ");
    Serial.println(distancia);  
   }
}
