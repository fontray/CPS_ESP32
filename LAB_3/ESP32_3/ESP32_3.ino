#include <Arduino.h>
#include"DHT.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h> 
#include<Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

extern "C"{
  #include "ntlib.h"
}
struct RSA rsa;   

#define DHT_PIN 32
#define DHT_TYPE DHT11

#define SCREEN_WIDTH 128 		//OLED width
#define SCREEN_HEIGHT 64  		//OLED height
#define OLED_RESET -1 			//Reset pin set -1

#define WIFI_SSID "ICLab"
#define WIFI_PASSWORD "ICLab41208"
#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_ID "NCKU"

// DHT Sensor
DHT dht(DHT_PIN, DHT_TYPE); 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   
bool OLEDStatus = true;

WiFiClient ESPClient;
PubSubClient client(ESPClient);

const char* topic = "cps/esp32/rsa";  // MQTT topic
String msg_payload;
int32_t mqtt_index = 0;

void callback(char* topic, byte* message, unsigned int length){
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    //Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  msg_payload = messageTemp;
  Serial.println(msg_payload);
}

void reconnect() {
  while(!client.connected()){
    if (client.connect(MQTT_ID)) {
      client.subscribe(topic); // Subscribe to channel.
    }
  }
}

void InitWiFi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    vTaskDelay(200 / portTICK_PERIOD_MS);
    Serial.print(".");
  }
  Serial.println("連線成功");
  Serial.println(WiFi.localIP());
}

void InitMQTT(){
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  vTaskDelay(500 / portTICK_PERIOD_MS);
}

void oled_display(String msg) {
  display.clearDisplay();           
  display.setTextSize(2);           
  display.setTextColor(1); 

  char message[100];
  strcpy(message, msg.c_str()); 

  uint64_t temp, humid; 
  sscanf(message, "Temperature: %llu Humidity: %llu", &temp, &humid);


  int plaintext_humid = rsa.Decrypt(&rsa, humid);
  int plaintext_temp = rsa.Decrypt(&rsa, temp);

  display.setCursor(0,0);          
  display.println("Temp: ");  
  display.setCursor(58,0);
  display.println(plaintext_temp); 

  display.setCursor(0,25);          
  display.println("Humid: ");
  display.setCursor(70,25);          
  display.print(plaintext_humid);

  display.print("%\n");
  
  display.display();                
  delay(1000);
}

void setup() {
  Serial.begin(9600);
                        
  dht.begin();  //dht setup
  InitWiFi();
  InitMQTT();
  rsa_init(&rsa, 523, 541, 199); // input parameter p = 523; q = 541; e = 199

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3c)) {     	 
    Serial.println(F("SSD1306 allocation falled"));		   
    OLEDStatus = false;  								                  
  }

}

void loop() {
   if(!client.connected()){
    reconnect();
  }

  client.loop();

  vTaskDelay(2000 / portTICK_PERIOD_MS);    //stop 2sec
  float h = dht.readHumidity();             // Read humidity 
  float t = dht.readTemperature();          // Read temperature 

  // Check if reading was successful
  if(isnan(h) || isnan(t)){ 
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.printf("加密前溫度 : %d\n", int(t));
  Serial.printf("加密前濕度 : %d\n", int(h));

  char buffer[100];
  uint64_t cipher_humid = rsa.Encrypt(&rsa, int(h));
  uint64_t cipher_temp = rsa.Encrypt(&rsa, int(t));
  Serial.printf("加密後溫度 : %llu\n", cipher_temp);
  Serial.printf("加密後濕度 : %llu\n", cipher_humid);
 
  sprintf(buffer, "Temperature: %llu Humidity: %llu", cipher_temp, cipher_humid);
  //printf("%s \r\n", buffer);
  client.publish("cps/esp32/rsa", buffer);

  oled_display(msg_payload);
  delay(3000);
}
