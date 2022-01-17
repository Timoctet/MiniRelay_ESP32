

#include "app_Menu.h"
#include "drv_SSD1306.h"
#include "drv_DS3231.h"
#include "drv_I2C.h"
#include "drv_Encodeur.h"

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "drv_BTN.h"


#include "drv_AT24C256.h"

AT24C256 eeprom (0x50);

const char* ssid     = "TimoTest";
const char* password = "12345678";

AsyncWebServer server(80);



#define PUSHBUTTON_PIN  17
#define PUSHBUTTON_ISUSEISR  true
#define PUSHBUTTON_LOGIC  false

 Push_Button PushButton1(PUSHBUTTON_PIN, PUSHBUTTON_ISUSEISR, PUSHBUTTON_LOGIC);
static void BTN_Interrupt();


static void BTN_Interrupt()
{
  PushButton1.ISR_Func();
}


#define PIN_BTN      17 // 17
#define PIN_RELAY    27 // 27

String Relay_ON(void)
{
  digitalWrite(PIN_RELAY, LOW);
  
  return String("ON");

}

String Relay_OFF(void)
{
  digitalWrite(PIN_RELAY, HIGH);
  
  return String("OFF");

}


void GetAlarmON(DS3231_Time_s *pTime)
{
  eeprom.read(0, (uint8_t*)pTime, sizeof(pTime));
}

void GetAlarmOFF(DS3231_Time_s *pTime)
{
  eeprom.read(10, (uint8_t*)pTime, sizeof(pTime));
}



void SetAlarmON(DS3231_Time_s *pTime)
{
  eeprom.write(0, (uint8_t*)pTime, sizeof(pTime));
}

void SetAlarmOFF(DS3231_Time_s *pTime)
{
  eeprom.write(10, (uint8_t*)pTime, sizeof(pTime));
}



void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_BTN ,   INPUT_PULLUP);
  pinMode(PIN_RELAY , OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PUSHBUTTON_PIN), BTN_Interrupt, CHANGE);

  Encodeur_init(33, 26, 18);
  
  Serial.begin(9600);
  I2C_init();


  DS3231_init(0x68, DS3231_CONTROL_INTCN, false);

  SSD1306_init();
  SSD1306_ClearDisplay();
  SSD1306_Display();

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();

  server.on (   "/ON", HTTP_GET, [](AsyncWebServerRequest *request)  
                { request->send_P(200, "text/plain", Relay_ON().c_str()); }
            );

  server.on (   "/OFF", HTTP_GET, [](AsyncWebServerRequest *request)  
                { request->send_P(200, "text/plain", Relay_OFF().c_str()); }
            );

/* server.on ("/RelayON", Relay_ON);

  server.on ("/RelaySOFF", Relay_OFF);*/

  server.begin();
}

void loop()
{
   Page_Draw();
   Page_HandleIndex();
   Encodeur_SW_main();
   DS3231_update();

  if(PushButton1.GetEdge() == PushButton_Edge_RisingEdge)
  {
      digitalWrite((PIN_RELAY), digitalRead(PIN_RELAY) == true ? false : true);
  }

  


  delay(2);
}
