#include <Arduino.h>
#include"DHT.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h> 
#include<Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define DHT_PIN 32
#define DHT_TYPE DHT11

#define SCREEN_WIDTH 128 		//設定OLED螢幕的寬度
#define SCREEN_HEIGHT 64  		//設定OLED螢幕的高度
#define OLED_RESET -1 			//Reset pin如果OLED上沒有RESET腳位,設置為-1

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

const char* topic = "cps/esp32/dht11";  // MQTT topic
String msg_payload;
int32_t mqtt_index = 0;

void callback(char* topic, byte* message, unsigned int length){
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  msg_payload = messageTemp;
  Serial.println();
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
  display.clearDisplay();           //清除緩衝區資料
  display.setTextSize(2);           // 設定文字大小
  display.setTextColor(1);  

  // 分割 Temperature 和 Humidity 子字串
  String tempstr = msg.substring(13, 18);
  String humidstr = msg.substring(29, 34);

  display.setCursor(0,0);          
  display.println("Temp: ");  
  display.setCursor(58,0);
  display.println(tempstr); 

  display.setCursor(0,25);          
  display.println("Humid: ");
  display.setCursor(0,45);          
  display.println(humidstr + "%");
  
  display.display();                
  delay(1000);
}

void setup() {
  Serial.begin(9600);

  dht.begin();  //dht初始化
  InitWiFi();
  InitMQTT();

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3c)) {     	  //設定位址為 0x3c
    Serial.println(F("SSD1306 allocation falled"));		    //F(字串):將字串儲存在fash並非在RAM
    OLEDStatus = false;  								                  //開啟OLED失敗
  }
}

void loop() {
  if(!client.connected()){
    reconnect();
  }

  client.loop();

  vTaskDelay(2000 / portTICK_PERIOD_MS); //暫停2秒
  float h = dht.readHumidity(); // Read humidity 
  float t = dht.readTemperature(); // Read temperature 

  // Check if reading was successful
  if(isnan(h) || isnan(t)){ 
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String value;
  String temperatureString = String(t, 2);
  String humidityString = String(h, 2);
  value = "Temperature: " + temperatureString + " Humidity: " + humidityString;

  client.publish("cps/esp32/dht11", value.c_str());

  oled_display(msg_payload);

}
