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


// Update these with values suitable for your network.
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xF1, 0x64};

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;

// Dados do MQTT Cloud
PubSubClient client("m11.cloudmqtt.com", 11084, callback, ethClient);

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
    client.subscribe("recebido");
  }
  Serial.println("Conectado MQTT");
  return client.connected();
  }

void setup() {
  // Inicializa a porta Serial
  Serial.begin(9600);
  Serial.println("Conectando...");
  delay(50);
  Ethernet.begin(mac);

  // Faz a conexão no cloud com nome do dispositivo, usuário e senha respectivamente
    if (client.connect("arduino", "arduino", "ard123"))
  {
    // Envia uma mensagem para o cloud no topic
    client.publish("teste", "v");
        

    // Conecta no topic para receber mensagens
  client.subscribe("recebido");
  
//   
  Serial.println("Conectado MQTT");
  delay(50);
}
}
void loop() {
  

if (!client.connected()) {
    long now = millis();
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
  int timeParked = 0;
  
  // Escreve o valor da distância no painel Serial
  Serial.print("\n");
  Serial.print("Distance in CM: ");
  Serial.println(distancia);

  if (distancia <= 30){
   int now = millis();
    if (now - timeParked > 3000){
      client.publish("teste","0");
      Serial.println("Vaga ocupada");
      timeParked = now;
      }
   }
  delay(1000);
}
