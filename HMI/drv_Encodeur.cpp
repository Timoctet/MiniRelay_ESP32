/**********************************************************************************
 **********************************************************************************
 *       @date       23/07/2021
 *       @Dev        Duclos Timothe
 *       @file       drv_Encodeur.cpp
*/

 
/********************************************************************************** 
 *  Includes
 */
#include "drv_Encodeur.h"


 
/********************************************************************************** 
 *  Defines
 */
#define ENCODEUR_FILTER_VALUE_MS    250


/********************************************************************************** 
 *  Définition des types
 */



/********************************************************************************** 
 *  Déclaration des variables privées
 */
static Encodeur_s Encodeur;

 
 /********************************************************************************** 
 *  Déclaration des fonctions privées
 */
static void ISR_Func(void);
 

/********************************************************************************** 
 *  Définition des fonctions publics
 */
 
/**
 *  @brief  Constructeur
 */
void Encodeur_init(uint8_t Pin_CLK, uint8_t Pin_DT, uint8_t Pin_SW)
{
    pinMode(Pin_CLK,  INPUT);
    pinMode(Pin_DT,   INPUT);

    pinMode(Pin_SW,   INPUT);

    

    Encodeur.Pin_CLK = Pin_CLK;
    Encodeur.Pin_DT  = Pin_DT;

    Encodeur.Pin_SW = Pin_SW;

    attachInterrupt(digitalPinToInterrupt(Pin_CLK), ISR_Func, CHANGE); 
}




/**
 *  @brief  
 */
bool Encodeur_SW_main(void)
{
    static uint8_t Etape = 0;

    switch(Etape) {
      
        // Attente appuie
        case 0:
            if(digitalRead(Encodeur.Pin_SW) == false)
            {
                Encodeur.EncodeurSW_State = EncodeurSW_State_RisingEdge;
                Etape = 1;
            }
            break;
          
         // Attente relaché
         case 1:
            if(!Encodeur.IsTimerStarted)
            {
                Encodeur.IsTimerStarted  = true;
                Encodeur.startTimer_ms   = millis();
            }
         
            if(digitalRead(Encodeur.Pin_SW) == true)
            {
                Encodeur.EncodeurSW_State = EncodeurSW_State_FallingEdge;
                Encodeur.IsTimerStarted   = false;
                
                Etape = 0;
                
            }
            break;
    }

    return true;
}

/**
 *  @brief  
 */
EncodeurSW_State_e Encodeur_get_SW_State(void)
{
    EncodeurSW_State_e rtrn = Encodeur.EncodeurSW_State;
    
    if(Encodeur.EncodeurSW_State == EncodeurSW_State_FallingEdge)
    {
        Encodeur.EncodeurSW_State = EncodeurSW_State_NotPressed;
    }
    else if(Encodeur.EncodeurSW_State == EncodeurSW_State_RisingEdge)
    {
        Encodeur.EncodeurSW_State = EncodeurSW_State_Pressed;
    }

    return rtrn;
}

/**
 *  @brief  
 */
uint32_t Encodeur_get_PressedTime(void)
{
    uint32_t rtrn_ms = 0;
    
    if(Encodeur.IsTimerStarted == true)
    {
        rtrn_ms = (millis() - Encodeur.startTimer_ms);
    }

    return rtrn_ms;
}

/**
 *  @brief  
 */
int32_t Encodeur_get_Counter(void)
{
    return Encodeur.Count;
}

/**
 *  @brief  Permet un filtrage du comptage de l'encodeur
 *          En effet, sur un seul increment/decrement plusieurs lectures d'impulsions
 *          peuveut avoir lieu.
 *          On va donc utiliter la fonction millis() pour filtrer la lecture.
 *          
 *          La valeur de filtrage est ENCODEUR_FILTER_VALUE_MS.
 *          
 *          Lorsque le filtrage est en cours, on jette les increments/decrements comptés.
 *          
 *  @retval   Ecart de comptage depuis la dernière lecture.
 */
int32_t Encodeur_get_CounterFiltered()
{
    int32_t rtrn = 0;
    
    if (       ( Encodeur.Count != Encodeur.oldCount)
           &&  (millis() - Encodeur.Filter >= ENCODEUR_FILTER_VALUE_MS) 
        )
    {
        Encodeur.Filter   = millis();

        rtrn              = Encodeur.Count - Encodeur.oldCount;
        Encodeur.oldCount = Encodeur.Count;
    }
    else
    {
        Encodeur.Count = Encodeur.oldCount;
    }
    
    return rtrn;
}



/********************************************************************************** 
 *  Définition des fonctions privées
 */
static void ISR_Func(void)
{
    static bool old_PhaseA  = false;
           bool PhaseA      = digitalRead(Encodeur.Pin_CLK);


    // Means the knob is rotating
    if (PhaseA != old_PhaseA)
    { 
        // if the knob is rotating, we need to determine direction
        // We do that by reading pin B.
        
        // Means pin A Changed first - We're Rotating Clockwise
        if (digitalRead(Encodeur.Pin_DT) != PhaseA) 
        {  
            Encodeur.Count ++;
        } 
        
        // Otherwise B changed first and we're moving CCW
        else 
        {
            Encodeur.Count--;
        }
        
        old_PhaseA = PhaseA;
    } 
    
}


/****************** END OF FILE ***************************************/
