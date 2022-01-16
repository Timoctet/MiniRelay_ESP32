/**********************************************************************************
 **********************************************************************************
 *       @date       23/07/2021
 *       @Dev        Duclos Timothe
 *       @file       drv_DS321.cpp
*/

 
/********************************************************************************** 
 *  Includes
 */
#include <stdio.h>
#include "drv_i2c.h"
#include "drv_DS3231.h"


 
/********************************************************************************** 
 *  Defines
 */

#define  DS3231_TRANSACTION_TIMEOUT  100 // Wire NAK/Busy timeout in ms


// i2c slave address of the DS3231 chip
#define DS3231_I2C_ADDR             0x68

// timekeeping registers
#define DS3231_TIME_CAL_ADDR        0x00
#define DS3231_ALARM1_ADDR          0x07
#define DS3231_ALARM2_ADDR          0x0B
#define DS3231_CONTROL_ADDR         0x0E
#define DS3231_STATUS_ADDR          0x0F
#define DS3231_AGING_OFFSET_ADDR    0x10
#define DS3231_TEMPERATURE_ADDR     0x11


// status register bits
#define DS3231_STATUS_A1F      0x01   /* Alarm 1 Flag */
#define DS3231_STATUS_A2F      0x02   /* Alarm 2 Flag */
#define DS3231_STATUS_BUSY     0x04   /* device is busy executing TCXO */
#define DS3231_STATUS_EN32KHZ  0x08   /* Enable 32KHz Output  */
#define DS3231_STATUS_OSF      0x80   /* Oscillator Stop Flag */






/* control register 0Eh/8Eh
        bit7 EOSC   Enable Oscillator (1 if oscillator must be stopped when on battery)
        bit6 BBSQW  Battery Backed Square Wave
        bit5 CONV   Convert temperature (1 forces a conversion NOW)
        bit4 RS2    Rate select - frequency of square wave output
        bit3 RS1    Rate select
        bit2 INTCN  Interrupt control (1 for use of the alarms and to disable square wave)
        bit1 A2IE   Alarm2 interrupt enable (1 to enable)
        bit0 A1IE   Alarm1 interrupt enable (1 to enable)
*/

#define __DS3231_set_CTRL_reg(val)    (void)    I2C_Write(DS3231.I2C_ID, DS3231_CONTROL_ADDR, val)
#define __DS3231_get_CTRL_reg()       (uint8_t) I2C_Read (DS3231.I2C_ID, DS3231_CONTROL_ADDR)



/*
 * 
 * status register 0Fh/8Fh
        bit7 OSF      Oscillator Stop Flag (if 1 then oscillator has stopped and date might be innacurate)
        bit3 EN32kHz  Enable 32kHz output (1 if needed)
        bit2 BSY      Busy with TCXO functions
        bit1 A2F      Alarm 2 Flag - (1 if alarm2 was triggered)
        bit0 A1F      Alarm 1 Flag - (1 if alarm1 was triggered)
*/
#define __DS3231_set_STATUS_reg(val)  (void)    I2C_Write(DS3231.I2C_ID, DS3231_STATUS_ADDR, val)
#define __DS3231_get_STATUS_sreg()    (uint8_t) I2C_Read (DS3231.I2C_ID, DS3231_STATUS_ADDR)




 
 /********************************************************************************** 
 *  Déclaration des variables privées
 */
static DS3231_t DS3231;

 
 /********************************************************************************** 
 *  Déclaration des fonctions privées
 */
 
static uint8_t dectobcd (const uint8_t val);
static uint8_t bcdtodec (const uint8_t val);
static uint8_t inp2toi  (char *cmd, const uint16_t seek);

 
/********************************************************************************** 
 *  Définition des fonctions publics
 */
 
/**
 *  @brief  Constructeur
 */
void DS3231_init(uint8_t I2C_ID, uint8_t Control_Register, bool IsUseExterna32KHz)
{
    DS3231.I2C_ID = I2C_ID;

     __DS3231_set_CTRL_reg(Control_Register);

    if(IsUseExterna32KHz)
    {
        I2C_Register_ResetBits (DS3231.I2C_ID, DS3231_STATUS_ADDR, DS3231_STATUS_OSF);
        I2C_Register_SetBits   (DS3231.I2C_ID, DS3231_STATUS_ADDR, DS3231_STATUS_EN32KHZ);
    }
    else
    {
        I2C_Register_ResetBits (DS3231.I2C_ID, DS3231_STATUS_ADDR, DS3231_STATUS_EN32KHZ);
    }
}
 

/**
 *  @brief  Ecriture de l'heure dans la puce DS3231
 *  
 *  @param[in]  *p_Time   Pointeur vers où lire l'heure à écrire
 */
void DS3231_set(DS3231_Time_s *p_Time)
{
    uint8_t TimeDate[7] = { p_Time->sec, p_Time->min, p_Time->hour, p_Time->wday, p_Time->mday, p_Time->mon, p_Time->year_s };

    //----------------------------------------
    // Prepare data
    if (TimeDate[6] >= 2000) 
    {
        TimeDate[5] += 0x80;
        TimeDate[6] -= 2000;
    } 
    else 
    {
        TimeDate[6] -= 1900;
    }

    //----------------------------------------
    // Transmission
    Wire.beginTransmission(DS3231.I2C_ID);
    Wire.write            (DS3231_TIME_CAL_ADDR);
    
    for (uint8_t i = 0; i <= 6; i++) 
    {
        TimeDate[i] = dectobcd(TimeDate[i]);
        Wire.write(TimeDate[i]);
    }
 
    Wire.endTransmission();
}

/**
 *  @brief  Mise à jour de l'heure dans la puce DS3231
 */
void DS3231_update(void)
{
    uint8_t  *pTimeDate = (uint8_t*)&DS3231.Time;        //second,minute,hour,dow,day,month,year
    
    uint8_t  century;
    uint16_t year_full;
    uint32_t start;
    
    uint8_t gotData = false;

    Wire.beginTransmission(DS3231.I2C_ID);
    Wire.write(DS3231_TIME_CAL_ADDR);
    Wire.endTransmission();

    // start timeout
    start = millis(); 
    
    while(  (millis()-start < DS3231_TRANSACTION_TIMEOUT) && (gotData == false) )
    {
        if (Wire.requestFrom(DS3231.I2C_ID, 7) == 7)
        {
            gotData = true;
        }
        else
        {
          delay(2);
        }
    }
    
    if (gotData)
    {
        for (uint8_t i = 0; i <= 6; i++)
        {
            uint8_t DataRead = Wire.read();
            
            if (i == 5) 
            {
                *(pTimeDate+i) = bcdtodec(DataRead & 0x1F);
                
            }
            else
            {
                *(pTimeDate+i) = bcdtodec(DataRead);
                if(i == 6)
                {
                    
                }
                 
            }
        }

        // Century
        (*(pTimeDate+5) & 0x80) >> 7 == 1 ? *(pTimeDate+7) = year_full = 2000 + *(pTimeDate+6) : year_full = 1900 + *(pTimeDate+6);
    }
}

/**
 *  @brief  Lecture de l'heure dans la variable privée
 *  
 *  @param[in]  *p_Time   Pointeur vers où écrire l'heure lue
 *                        NULL si on ne veut pas écrire
 *                        
 *  @retval   Pointeur vers la structure contenant l'heure
 */
DS3231_Time_s* DS3231_get(DS3231_Time_s *p_Time)
{
    if(p_Time != NULL)
    {
        for(uint8_t i=0; i<7; i++)
        {
            *(((uint8_t*)p_Time)+i) = *(((uint8_t*)&DS3231.Time)+i);
        }
    } 

    return &DS3231.Time;
}



/********************************************************************************** 
 *  Définition des fonctions privées
 */
 
static uint8_t dectobcd(const uint8_t val)
{
    return ((val / 10 * 16) + (val % 10));
}

static uint8_t bcdtodec(const uint8_t val)
{
    return ((val / 16 * 10) + (val % 16));
}

static uint8_t inp2toi(char *cmd, const uint16_t seek)
{
    uint8_t rv;
    rv = (cmd[seek] - 48) * 10 + cmd[seek + 1] - 48;
    return rv;
}


/****************** END OF FILE ***************************************/
