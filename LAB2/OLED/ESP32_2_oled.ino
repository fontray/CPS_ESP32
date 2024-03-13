#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h> 
#include<Wire.h>

#define SCREEN_WIDTH 128 		//設定OLED螢幕的寬度像素
#define SCREEN_HEIGHT 64  		//設定OLED螢幕的高度像素
#define OLED_RESET -1 			//Reset pin如果OLED上沒有RESET腳位,設置為-1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   	//OLED顯示器使用I2C連線並宣告名為display物件
bool OLEDStatus = true;

void testdrawchar(void) {
  display.clearDisplay();           // 清除緩衝區資料
  display.setTextSize(2);           // 文字大小
  display.setTextColor(1);          

  display.setCursor(0,16);          // 起始座標
  display.println("Student ID");    // 顯示字串,存入SSD1306 RAM 
  display.setCursor(26,40);         
  display.print("N00000000");         

  display.display();                // 文字顯示
  delay(1000);

}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    if(!display.begin(SSD1306_SWITCHCAPVCC,0x3c)) {     	  //設定位址為 0x3c
      Serial.println(F("SSD1306 allocation falled"));		    //F(字串):將字串儲存在fash並非在RAM
      OLEDStatus = false;  								                  //開啟OLED失敗
    }
  
  if(OLEDStatus == true)
    testdrawchar();       //Draw characters of the default font

}

void loop() {
}
