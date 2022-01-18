/**********************************************************************************
 **********************************************************************************
        @date       10/07/2021
        @Dev        Duclos Timothe
        @file       app_Alarm.c
*/
#ifndef APP_ALARM_H
#define APP_ALARM_H

#include "drv_DS3231.h"



/********************************************************************************** 
 *  Defines
 */


/********************************************************************************** 
 *  Exported function declaration
 */
void GetAlarmON (DS3231_Time_s *pTime);
void GetAlarmOFF(DS3231_Time_s *pTime);
void SetAlarmON (DS3231_Time_s *pTime);
void SetAlarmOFF(DS3231_Time_s *pTime);




#endif // APP_ALARM_H

/****************** END OF FILE ***************************************/
