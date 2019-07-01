#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

DHT dht;
const char* mqttServer = "postman.cloudmqtt.com";
const int mqttPort = 17679;
const char* mqttUser = "elpixqob";
const char* mqttPassword = "mGjfSKZV51cd";
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  WiFi.begin("Movistar_7DB53C", "24FF22732F");
    Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
    client.publish("esp/test", "Hello from ESP8266");
    client.subscribe("esp/test");
}
  // Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");
  dht.setup(D15);
}

void loop()
{
  // client.loop(); 
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.println(temperature);
  char result[8];
  dtostrf(temperature, 6, 2, result);
  //Serial.print(dht.getStatusString());
  /*Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1); */
  Serial.println(result);
  client.publish("esp/test", result);
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
