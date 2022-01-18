/**********************************************************************************
 **********************************************************************************
        @date       10/07/2021
        @Dev        Duclos Timothe
        @file       app_Alarm.c
*/


#include "app_Alarm.h"
#include "drv_AT24C256.h"


/********************************************************************************** 
 *  Defines
 */
 #define EEPROM_MEMORY_I2C_ADRESS     0x50



/********************************************************************************** 
 *  Private variable
 */
// EEPROM
static AT24C256 eeprom (EEPROM_MEMORY_I2C_ADRESS);   // Memoire EEPROM 



/********************************************************************************** 
 *  Exported function definition
 */

 /**
  * @brief    Lecture de 
  */
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



/****************** END OF FILE ***************************************/
