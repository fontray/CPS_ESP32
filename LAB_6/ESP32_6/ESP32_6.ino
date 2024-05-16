#include <Arduino.h>
#include"DHT.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h> 
#include<Wire.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>  // Provide the token generation process info
#include <addons/RTDBHelper.h>   // Provide the RTDB payload printing info and other helper functions

extern "C"{
  #include "ntlib.h"
}

// to do
// declare the struct of crypto algorithm (RSA, Paillier, Elgamal)



#define DHT_PIN 32
#define DHT_TYPE DHT11

#define WIFI_SSID "ICLab"
#define WIFI_PASSWORD "ICLab41208"

#define SCREEN_WIDTH 128 		
#define SCREEN_HEIGHT 64  		
#define OLED_RESET -1 			

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   	
bool OLEDStatus = true;

// DHT Sensor
DHT dht(DHT_PIN, DHT_TYPE); 

// Define Firebase project API key, data URL, data path
#define API_KEY "AIzaSyCq2BHa32J3ICPMgr_PUpjAwCuE6PQVXQ4"
#define DATABASE_URL "https://esp32firebase-fafaf-default-rtdb.firebaseio.com/"
#define DATABASE_TEMP_PATH "dht11/temperature"
#define DATABASE_HUMID_PATH "dht11/humidity"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// check if sucess connect server
bool isSignok = false;

void InitWiFi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    vTaskDelay(200 / portTICK_PERIOD_MS);
    Serial.print(".");
  }
  Serial.println("連線成功");
  Serial.println(WiFi.localIP());
}

void oled_display(uint64_t t, uint64_t h) {
  display.clearDisplay();           
  display.setTextSize(2);           
  display.setTextColor(1); 

  Serial.printf("解密濕度 %d\n", t);
  Serial.printf("解密溫度 %d\n", h);

  display.setCursor(0,0);          
  display.println("Temp: ");  
  display.setCursor(58,0);
  display.println(t); 

  display.setCursor(0,25);          
  display.println("Humid: ");
  display.setCursor(70,25);          
  display.print(h);

  display.print("%\n");
  
  display.display();                
  delay(1000);
}

// Initial firebase
void InitFireBase(){
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("SignUp OK!");
    isSignok = true;
  }
  else{
    Serial.printf("%s\r\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

// Write "string" to firebase
void UpdateRTDB(char* Path, String message){
  if(Firebase.RTDB.setString(&fbdo, Path, message)){
      Serial.print("--Successfully save to" + fbdo.dataPath());
      Serial.println("");
    }
  else{
    Serial.println("FAILED:" + fbdo.errorReason());
  }
}

// Read data from firebase and return "string"
String ReadRTDB(char* Path){
  String message = "";
  if(Firebase.RTDB.getString(&fbdo, Path)){
    // Serial.println("Read data from" + fbdo.dataPath());
    // Serial.println(fbdo.stringData());
    message = fbdo.stringData();
    return message;
  }
  else{
    // Serial.println("FAILED:" + fbdo.errorReason());
    message = "FAILED:" + fbdo.errorReason();
  }
  return message;
}

void setup() {
  Serial.begin(9600);
                        
  dht.begin();  //dht setup
  InitWiFi();
  InitFireBase();

  // to do
  // Initial your crypto algorithm
  

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3c)) {     	  
      Serial.println(F("SSD1306 allocation falled"));		    
      OLEDStatus = false;  								                  
    }

}

void loop() {
  if(Firebase.ready() && isSignok){

    // Read humidity as Celsius (the default)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    Serial.printf("temperature: %f ", t);
    Serial.printf("Humidity: %f  \r\n", h);

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    /*------------------------------------------------------------------------------*/
    // to do
    // Encrypt temperature/humidity based on your algorithm


    /*------------------------------------------------------------------------------*/

    // Transport data to firebase realtime database
    UpdateRTDB(DATABASE_TEMP_PATH, String(m_temp));
    UpdateRTDB(DATABASE_HUMID_PATH, String(m_humid));

    vTaskDelay(10 / portTICK_PERIOD_MS);

    Serial.println("讀取溫度" + ReadRTDB(DATABASE_TEMP_PATH));
    Serial.println("讀取濕度" + ReadRTDB(DATABASE_HUMID_PATH));
    Serial.println("-----------------------------------------------------------");

    /*------------------------------------------------------------------------------*/
    // to do
    // Decrypt temperature/humidity based on your algorithm
    



    /*------------------------------------------------------------------------------*/

    oled_display(, );

  }

  vTaskDelay(5000 / portTICK_PERIOD_MS);
}