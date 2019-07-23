#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

DHT dht;
long previousTime;
long minTime;
unsigned long currentTime;
const char* mqttServer = "postman.cloudmqtt.com";
const int mqttPort = 17679;
const char* mqttUser = "elpixqob";
const char* mqttPassword = "mGjfSKZV51cd";
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
 
  //Serial.print("Message arrived in topic: ");
  //Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } 
  payload[length] = '\0';
  String payloadAsString = String((char*)payload);
  if(payloadAsString == "Encender"){
    digitalWrite(D8,!digitalRead(D8));
    client.publish("esp/test", "Ready");
  }
  Serial.println();
  Serial.println("-----------------------");
}

void setup()
{
  previousTime = 0;
  minTime = dht.getMinimumSamplingPeriod();
  pinMode(D8,OUTPUT);
  digitalWrite(D8,LOW);
  Serial.begin(9600);
  Serial.println();
  WiFi.begin("BORO", "123811976");
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
    //client.publish("esp/test", "Hello from ESP8266");
    client.subscribe("esp/test");
    client.setCallback(callback);
}
  // Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");
  dht.setup(D15);
}

void loop()
{
  client.loop(); 
  // delay(dht.getMinimumSamplingPeriod());
  unsigned long currentMillis = millis();
  currentTime = millis();
  if(currentMillis - previousTime >= minTime){
    Serial.print("entro al if");
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    // Serial.println(temperature);
    char result[8];
    char resultH[8];
    dtostrf(temperature, 6, 2, result);
    dtostrf(humidity, 6, 2, resultH);
    String str(result);
    String str2(resultH);
    String TH = str+str2;
    Serial.println(TH);
    char pload[50];
    TH.toCharArray(pload,50);
    client.publish("esp/test", pload);
    previousTime = previousTime + minTime;  
    }
  //String humidityTemperature = result + '&' +resultH;
  //Serial.print(humidityTemperature)
  //Serial.print(dht.getStatusString());
  /*Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1); */
  //Serial.println(result);
  // Serial.println(resultH);
  
}
