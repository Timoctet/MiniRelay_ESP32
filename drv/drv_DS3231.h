/**********************************************************************************
 **********************************************************************************
 *       @date       23/07/2021
 *       @Dev        Duclos Timothe
 *       @file       drv_DS321.h
*/

#ifndef DRV_DS321_H
#define DRV_DS321_H

 
/********************************************************************************** 
 *  Includes
 */
#include <Arduino.h>

 
/********************************************************************************** 
 *  Defines
 */


/********************************************************************************** 
 *  Définition des types
 */
 
typedef enum {

  DS3231_CONTROL_A1IE     = 0x01,
  DS3231_CONTROL_A2IE     = 0x02,
  DS3231_CONTROL_INTCN    = 0x04,
  DS3231_CONTROL_RS1      = 0x08,
  DS3231_CONTROL_RS2      = 0x10,
  DS3231_CONTROL_CONV     = 0x20,
  DS3231_CONTROL_BBSQW    = 0x40,
  DS3231_CONTROL_EOSC     = 0x80
  
}DS3231_REG_CONTROL_e;

typedef enum {

  DS3231_STATUS_A1F     = 0x01,
  DS3231_STATUS_A2F     = 0x02,
  DS3231_STATUS_BUSY    = 0x04,
  DS3231_STATUS_EN32KHZ = 0x08,
  DS3231_STATUS_OSF     = 0x80
  
}DS3231_REG_STATUS_e;


 
typedef struct {
 
    uint8_t sec;         /* seconds */
    uint8_t min;         /* minutes */
    uint8_t hour;        /* hours */
    uint8_t wday;        /* day of the week */


    uint8_t mday;        /* day of the month */

    uint8_t mon;         /* month */

    uint8_t year_s;      /* year in short notation*/
    int16_t year;        /* year */
    
    
    
}__attribute__((packed, aligned(1))) DS3231_Time_s;


typedef struct 
{
   DS3231_Time_s   Time;
   uint8_t         I2C_ID;
}DS3231_t;

/********************************************************************************** 
 *  Définition de la classe
 */
// constructors:


// Public methods
void DS3231_init(uint8_t I2C_ID, uint8_t Control_Register, bool IsUseExterna32KHz);

void            DS3231_set(DS3231_Time_s *p_Time);
DS3231_Time_s*  DS3231_get(DS3231_Time_s *p_Time);

void DS3231_update(void);



#endif /* DRV_DS321_H */


/****************** END OF FILE ***************************************/
