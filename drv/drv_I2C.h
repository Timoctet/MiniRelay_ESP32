/**********************************************************************************
 **********************************************************************************
 *       @date       23/07/2021
 *       @Dev        Duclos Timothe
 *       @file       drv_I2C.h
*/

#ifndef DRV_I2C_H
#define DRV_I2C_H

 
/********************************************************************************** 
 *  Includes
 */
#include "Arduino.h"
#include <Wire.h>


/********************************************************************************** 
 *  Déclaration des fonctions publics
 */
void I2C_init(void);

void    I2C_Write (uint8_t I2C_ID, uint8_t addr, uint8_t val);
int8_t  I2C_Read  (uint8_t I2C_ID, uint8_t addr);

void I2C_Register_SetBits  (uint8_t I2C_ID, uint8_t Register_Adrss, uint8_t Mask_BitToSet);
void I2C_Register_ResetBits(uint8_t I2C_ID, uint8_t Register_Adrss, uint8_t Mask_BitToReset);


 

#endif /* DRV_I2C_H */


/****************** END OF FILE ***************************************/
