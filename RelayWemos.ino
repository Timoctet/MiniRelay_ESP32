

#include "app_Menu.h"
#include "drv_SSD1306.h"
#include "drv_DS3231.h"
#include "drv_I2C.h"

#define PIN_BTN      17 // 17
#define PIN_RELAY    27 // 27

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_BTN ,   INPUT_PULLUP);
  pinMode(PIN_RELAY , OUTPUT);
  Serial.begin(9600);
  I2C_init();

  DS3231_init(0x68, DS3231_CONTROL_INTCN, false);

  SSD1306_init();
  SSD1306_ClearDisplay();
  SSD1306_Display();
}

void loop()
{
  DS3231_Time_s*  Time;
  char message[30] = {0};
  
   Page_Draw();
    Page_HandleIndex();

     DS3231_update();
  Time = DS3231_get(NULL);
       snprintf(message, 9, "%02d:%02d:%02d", Time->hour, Time->min, Time->sec);
  Serial.println(message);
  
  if(digitalRead(PIN_BTN) == LOW)
  {
      digitalWrite(PIN_RELAY, LOW);
  }
  else
  {
      digitalWrite(PIN_RELAY, HIGH);
  }

  delay(2);
}
