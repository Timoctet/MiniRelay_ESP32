/**********************************************************************************
 **********************************************************************************
 *       @date       23/07/2021
 *       @Dev        Duclos Timothe
 *       @file       drv_SSD1306.h
*/

#ifndef drv_SSD1306
#define drv_SSD1306





/********************************************************************************** 
 *  Includes
 */
#include "Arduino.h"


/********************************************************************************** 
 *  Déclaration des types
 */





/********************************************************************************** 
 *  Déclaration des fonctions publics
 */
void SSD1306_init(void);

void SSD1306_DrawString (char *pString, uint8_t Pos_x, uint8_t Pos_y, uint8_t Size);

void SSD1306_DrawCirle  (uint8_t Pos_x, uint8_t Pos_y, uint8_t Size, bool IsFilled);
void SSD1306_CircleMenu (uint8_t nbElement, uint8_t IndexHighlight);

void SSD1306_DrawRect   (uint8_t Pos_x, uint8_t Pos_y, uint8_t Height, uint8_t Length, bool    IsFilled);
void SSD1306_DrawJauge  (uint8_t Jauge_Level_pr100, uint8_t Jauge_X, uint8_t Jauge_Y, uint8_t Jauge_Height, uint8_t Jauge_Length);

void SSD1306_ClearDisplay (void);
void SSD1306_Display      (void);






#endif /* drv_SSD1306 */


/****************** END OF FILE ***************************************/
