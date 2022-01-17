/**********************************************************************************
 **********************************************************************************
        @date       17/01/2022
        @Dev        Duclos Timothe
        @file       RelayWemos.ino
*/


/********************************************************************************** 
 *  Includes
 */
#include "app_Menu.h"
#include "drv_SSD1306.h"
#include "drv_DS3231.h"
#include "drv_I2C.h"
#include "drv_Encodeur.h"

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "drv_BTN.h"


#include "drv_AT24C256.h"



/********************************************************************************** 
 *  Define
 */
#define PUSHBUTTON_PIN     17
#define PIN_RELAY          27


/********************************************************************************** 
 *  Private function prototype
 */
static void BTN_Interrupt();


/********************************************************************************** 
 *  Variables
 */

 /**
  * @brief  Push Button
  */


AT24C256 eeprom (0x50);   // Memoire EEPROM 

/*
 * Nom et mot de passe du point d'acces
 */
const char* ssid     = "TimoTest";
const char* password = "12345678";

AsyncWebServer server(80);  // Serveur

/**
 * Push button
 */
Push_Button PushButton1 (   PUSHBUTTON_PIN,     // Pin
                            true,   // ISR
                            false   // Logic
                        );






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

  server.begin();
}

/**
 * @brief   Main Loop
 */
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




/********************************************************************************** 
 *  Private function definition
 */
/**
 * @brief Fonction d'interruption du push bouton
 */
static void BTN_Interrupt()
{
  PushButton1.ISR_Func();
}
