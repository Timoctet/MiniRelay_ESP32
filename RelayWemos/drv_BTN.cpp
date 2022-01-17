/**********************************************************************************
 **********************************************************************************
        @date       10/07/2021
        @Dev        Duclos Timothe
        @file       drv_BTN.c
*/


#include "drv_BTN.h"



/********************************************************************************** 
 *  Defines
 */
static Push_Button *pISR1_BTN;
 
/********************************************************************************** 
 *  Déclaration des fonctions privées
 */
void ISR_Func(void);


/********************************************************************************** 
 *  Définition des fonctions publics
 */
Push_Button::Push_Button(uint8_t PinID, bool ISR, bool Logic)
{
	this->PinID = PinID;
	this->Logic = Logic;
	
	pinMode(this->PinID, INPUT_PULLUP);
	
	if(ISR)
	{
	  //attachInterrupt(digitalPinToInterrupt(PinID), this->ISR_Func, CHANGE);
    pISR1_BTN = this;
	}
}

void Push_Button::ISR_Func(void)
{

    if  (     (digitalRead(this->PinID) == false && (this->Logic == false) )
          ||  (digitalRead(this->PinID) == true  && (this->Logic == true) )
        )
    {
      this->Edge             = PushButton_Edge_RisingEdge;
      this->IsTimerStarted   = true;
      this->startTimer_ms    = millis();
      
    }
    
    else
    {
      this->Edge           = PushButton_Edge_FallingEdge;
      this->IsTimerStarted = false;
      this->startTimer_ms  = 0;
    }
  
}


bool Push_Button::IsPressed()
{
	bool State = digitalRead(this->PinID);
	
	if(!this->Logic)
	{
		State = ( (this->Edge == PushButton_Edge_RisingEdge) || (this->Edge == PushButton_Edge_Pressed) )  ? false : true;
	}
	
	return State;
}

uint8_t Push_Button::GetEdge()
{
	PushButton_Edge_e rtrn = this->Edge;
    
    if(this->Edge == PushButton_Edge_FallingEdge)
    {
        this->Edge = PushButton_Edge_NotPressed;
    }
    else if(this->Edge == PushButton_Edge_RisingEdge)
    {
        this->Edge = PushButton_Edge_Pressed;
    }

    return rtrn;
}


uint32_t Push_Button::GetPressedTime()
{
	uint32_t rtrn_ms = 0;
    
    if(this->IsTimerStarted == true)
    {
        rtrn_ms = (millis() - this->startTimer_ms);
    }

    return rtrn_ms;
}


/********************************************************************************** 
 *  Définition des fonctions privées
 */



/****************** END OF FILE ***************************************/
