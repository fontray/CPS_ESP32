#include <Arduino.h>
#include"DHT.h"

#define DHT_PIN 32
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE); //宣告建立DHT類別物件

void setup() {
  Serial.begin(9600);
  dht.begin(); //dht初始化

}

void loop() {
  vTaskDelay(2000 / portTICK_PERIOD_MS); //暫停2秒
  float h = dht.readHumidity(); // Read humidity as Celsius (the default)
  float t = dht.readTemperature(); // Read temperature as Celsius (the default)

  Serial.printf("temperature: %f ", t);
  Serial.printf("humidity: %f \r\n", h);

  if(isnan(h) || isnan(t)){ //Not a Number 
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

}
