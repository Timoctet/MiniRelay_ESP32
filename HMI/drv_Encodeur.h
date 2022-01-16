/**********************************************************************************
 **********************************************************************************
 *       @date       23/07/2021
 *       @Dev        Duclos Timothe
 *       @file       drv_Encodeur.h
*/

#ifndef DRV_ENCODEUR_H
#define DRV_ENCODEUR_H

 
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
 
 /**
  *   @brief    Définition des états possibles du Switch de l'encodeur
  */
 typedef enum {
  
  EncodeurSW_State_NotPressed = 0,
  
  EncodeurSW_State_RisingEdge,
  EncodeurSW_State_Pressed,
  EncodeurSW_State_FallingEdge,

  nb_EncodeurSW_State
  
}EncodeurSW_State_e;

typedef struct 
{
        uint8_t Pin_CLK;
        uint8_t Pin_DT;
        uint8_t Pin_SW;

        EncodeurSW_State_e EncodeurSW_State;

        bool      IsTimerStarted;
        uint32_t  startTimer_ms;

        int32_t   Count;


        //Filter
        int32_t   oldCount;
        uint32_t  Filter;
        
}Encodeur_s;


/********************************************************************************** 
 *  Déclaration des fonctions privées
 */


void Encodeur_init(uint8_t Pin_CLK, uint8_t Pin_DT, uint8_t Pin_SW);

bool                Encodeur_SW_main    (void);

EncodeurSW_State_e  Encodeur_get_SW_State        (void);
uint32_t            Encodeur_get_SW_PressedTime  (void);

int32_t Encodeur_get_Counter          (void);
int32_t Encodeur_get_CounterFiltered  ();
        





#endif /* DRV_ENCODEUR_H */


/****************** END OF FILE ***************************************/
