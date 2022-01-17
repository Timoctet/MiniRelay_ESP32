/**********************************************************************************
 **********************************************************************************
        @date       10/07/2021
        @Dev        Duclos Timothe
        @file       drv_BTN.c
*/



/********************************************************************************** 
 *  Includes
 */
#include <Arduino.h>


 
/********************************************************************************** 
 *  Déclaration des types
 */
 typedef enum {
  
  PushButton_Edge_NotPressed = 0,
  
  PushButton_Edge_RisingEdge,
  PushButton_Edge_Pressed,
  PushButton_Edge_FallingEdge,

  nb_PushButton_Edge
  
}PushButton_Edge_e;

 
/********************************************************************************** 
 *  Déclaration des fonctions publics
 */
class Push_Button {
	
	public:
	
		Push_Button	( uint8_t PinID, bool ISR, bool Logic );
		
		bool 	    IsPressed 	    ();
		uint32_t  GetPressedTime	();
    uint8_t   GetEdge         (); 
    void      ISR_Func        ();
		
	private:
  
		uint8_t             PinID;
		bool				        IsTimerStarted;
		uint32_t 			      startTimer_ms;		// Retour de millis lorsque le bouton est préssé
		bool				        Logic;
		PushButton_Edge_e 	Edge;
};

/****************** END OF FILE ***************************************/
