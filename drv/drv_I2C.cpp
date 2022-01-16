/**********************************************************************************
 **********************************************************************************
 *       @date       23/07/2021
 *       @Dev        Duclos Timothe
 *       @file       drv_I2C.cpp
 *       @brief      Création du driver I2C arduino
*/

 
/********************************************************************************** 
 *  Includes
 */
#include "drv_I2C.h"

/********************************************************************************** 
 *  Define
 */
#define  I2C_TRANSACTION_TIMEOUT  100     // Wire NAK/Busy timeout in ms

/********************************************************************************** 
 *  Private variable
 */
 static bool IsI2C_init = false;
 
/********************************************************************************** 
 *  Définition des fonctions publics
 */
 
/**
 *  @brief  Init de l'I2C
 */
void I2C_init(void)
{
    if(!IsI2C_init)
    {
        Wire.begin();
        IsI2C_init = true;
    }
}

/**
 *  @brief  Ecriture sur le bus I2C
 *  
 *  @param[in]  I2C_ID    ID I2C de l'équipement avec lequel on veut communiquer
 *  @param[in]  addr      Adress du registre    
 *  @param[in]  Data      Data à transmettre
 *  
 */
void I2C_Write(uint8_t I2C_ID, uint8_t addr, uint8_t Data)
{
    Wire.beginTransmission(I2C_ID);
    Wire.write(addr);
    Wire.write(Data);
    Wire.endTransmission();
}

/**
 *  @brief  Lecture sur le bus I2C
 *  
 *  @param[in]  I2C_ID    ID I2C de l'équipement avec lequel on veut communiquer
 *  @param[in]  addr      Adress du registre    
 *  
 *  @retval     Data lue (0 sinon)
 */
int8_t I2C_Read(uint8_t I2C_ID, uint8_t addr)
{
    uint8_t rv      = 0;
    uint8_t gotData = false;
    
    uint32_t start;

    Wire.beginTransmission(I2C_ID);
    Wire.write(addr);
    Wire.endTransmission();

    // start timeout
    start = millis(); 

    // Attente reception avec Timeout
    while( (millis()-start < I2C_TRANSACTION_TIMEOUT) && (gotData == false) )
    {
        if (Wire.requestFrom(I2C_ID, 1) == 1) 
        {
            gotData = true;
        }
        else
        {
          delay(2);
        }
    }

    // Data reçu
    if (gotData)
    {
        rv = Wire.read();
    }
    
    return rv;
}



/**
 *  @brief    Permet de setter les bits demander dans le registre
 *  
 *  @param[in]  I2C_ID    ID I2C de l'équipement avec lequel on veut communiquer
 *  @param[in]  Register_Adrss    Adress du registre
 *  @param[in]  Mask_BitToSet     Le mask des bits à setter dans le registre
 */
void I2C_Register_SetBits(uint8_t I2C_ID, uint8_t Register_Adrss, uint8_t Mask_BitToSet)
{
    uint8_t RegisterValue = I2C_Read(I2C_ID, Register_Adrss);

    RegisterValue |= Mask_BitToSet;
    I2C_Write(I2C_ID, Register_Adrss, RegisterValue);
}


/**
 *  @brief    Permet de clear les bits demander dans le registre
 *  
 *  @param[in]  I2C_ID            ID I2C de l'équipement avec lequel on veut communiquer
 *  @param[in]  Register_Adrss    Adress du registre
 *  @param[in]  Mask_BitToReset   Le mask des bits clear dans le registre
 */
void I2C_Register_ResetBits(uint8_t I2C_ID, uint8_t Register_Adrss, uint8_t Mask_BitToReset)
{
    uint8_t RegisterValue = I2C_Read(I2C_ID, Register_Adrss);

    RegisterValue &= ~Mask_BitToReset;
    I2C_Write(I2C_ID, Register_Adrss, RegisterValue);
}


/****************** END OF FILE ***************************************/
