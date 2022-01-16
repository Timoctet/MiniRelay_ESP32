/**********************************************************************************
 **********************************************************************************
        @date       10/07/2021
        @Dev        Duclos Timothe
        @file       App_Screen.h
*/

#include "Arduino.h"
#include "APP_Screen.h"

#include "drv_Encodeur.h"
#include "drv_SSD1306.h"

/********************************************************************************** 
 *  Définition des constantes privées
 */


 
/********************************************************************************** 
 *  Définition des types privés
 */


 
/********************************************************************************** 
 *  Déclaration des fonctions privées
 */

/********************************************************************************** 
 *  Définition des variables privée
 */




/********************************************************************************** 
 *  Définition des fonctions publics
 */

/**
 *  @brief    Constructeur
 */
Screen::Screen(uint8_t IndexMax, void (*pf_DrawScreen)(uint32_t *pParamter), bool TypeOfIndex)
{
    this->IndexMax      = IndexMax;
    this->pf_DrawScreen = pf_DrawScreen;
    this->TypeOfIndex   = TypeOfIndex;
}

/**
 *  @brief    Lecture de la valeur d'index de la page
 *  
 *  @return   Valeur de l'index courante.
 */
uint8_t Screen::getIndex()
{
    return this->Index;
}


/**
 *  @brief    Ecriture de la valeur d'index de la page
 *  
 *  @param[in]       IndexValue   Valeur de l'index courante.
 */
void Screen::setIndex(uint8_t IndexValue)
{
    this->Index = IndexValue;
}

/**
 *  @brief    RaZ private screen variable
 */
void Screen::RaZScreen()
{
    this->Index  = 0;
    this->IsInit = false;
}



void Screen::Configure_pIndex(uint8_t *pIndex, uint8_t IndexMax)
{
    this->pIndex   = pIndex;
    this->IndexMax = IndexMax;
}

/**
 *  @brief    Gestion de l'index
 */
void Screen::HandleIndex(void)
{
    int32_t EncodeurCount = Encodeur_get_CounterFiltered();
    
    if(this->TypeOfIndex)
    {
         // Gestion de l'encodeur
         if(EncodeurCount !=0 )
         {
              if(EncodeurCount > 0)
              {
                  if(this->Index < this->IndexMax)
                  {
                      this->Index++;
                  }
                  else
                  {
                      this->Index = 0;
                  }
              }
              else
              {
                  if(this->Index > 0)
                  {
                      this->Index--;
                  }
                  else
                  {
                      this->Index = this->IndexMax;
                  }
              }
         }
    }
    else
    {
        
         if(EncodeurCount !=0 )
         {
              if(EncodeurCount > 0)
              {
                  if(*(this->pIndex) < this->IndexMax)
                  {
                      *(this->pIndex) = *(this->pIndex) + 1;
                  }
                  else
                  {
                      *(this->pIndex) = 0;
                  }
              }
              else
              {
                  if(*(this->pIndex) > 0)
                  {
                      *(this->pIndex) = *(this->pIndex) - 1;
                  }
                  else
                  {
                      *(this->pIndex) = this->IndexMax;
                  }
              }
         }
     }

     
}





/****************** END OF FILE ***************************************/
