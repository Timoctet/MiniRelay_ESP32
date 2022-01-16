/**********************************************************************************
 **********************************************************************************
 *       @date       23/07/2021
 *       @Dev        Duclos Timothe
 *       @file       drv_SSD1306.cpp
 *       @brief      
*/

 
/********************************************************************************** 
 *  Includes
 */
#include "drv_SSD1306.h"

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>

/**
 *  @brief    Resolution de l'écran
 */
#define SCREEN_WIDTH    128   // OLED display width, in pixels
#define SCREEN_HEIGHT   64    // OLED display height, in pixels

/********************************************************************************** 
 *  Define
 */

/********************************************************************************** 
 *  Private variable
 */
static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
/********************************************************************************** 
 *  Définition des fonctions publics
 */

/**
 *   @brief    Initialisation du module écran
 */
void SSD1306_init(void)
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

/**
 *   @brief    Effacement du display
 */
void SSD1306_ClearDisplay(void)
{
  display.clearDisplay();
}

/**
 *   @brief    Dessin d'une chaine de charactère
 *   
 *   @param[in]   *pString      Pointeur vers la chaine de charactère
 *   @param[in]   Pos_x         Position sur l'axe des absices
 *   @param[in]   Pos_y         Position sur l'axe des ordonnées
 *   @param[in]   Size          Taille de la police
 */
void SSD1306_DrawString (   char    *pString,
                            uint8_t Pos_x,
                            uint8_t Pos_y,
                            uint8_t Size
                        )
{
    if(pString != NULL)
    {

       display.setCursor    (Pos_x, Pos_y);
       display.setTextColor (SSD1306_WHITE);  
       display.setTextSize  (Size);
       display.print        (pString);
    }
}





/**
 *   @brief    Dessin d'un cercle
 *   
 *   @param[in]   Pos_x         Position sur l'axe des absices
 *   @param[in]   Pos_y         Position sur l'axe des ordonnées
 *   @param[in]   Size          Taille du tracé extérieur
 *   @param[in]   IsFilled      TRUE  ->  Cercle rempli
 *                              FALSE ->  Cercle vide
 */
void SSD1306_DrawCirle  (   uint8_t Pos_x,
                            uint8_t Pos_y,
                            uint8_t Size, 
                            bool    IsFilled
                        )
{
    if(IsFilled)
    {
        display.fillCircle(Pos_x, Pos_y, Size, SSD1306_WHITE);
    }
    else
    {
        display.drawCircle(Pos_x, Pos_y, Size, SSD1306_WHITE);
    }
}

/**
 *   @brief    Dessin d'une ligne série de petit cercle
 *             Avec l'un d'entre eux qui est plein.
 *   
 *   @param[in]   nbElement         Nombre de cercle
 *   @param[in]   IndexHighlight    Indice du cercle plein
 */
void SSD1306_CircleMenu(uint8_t nbElement, uint8_t IndexHighlight)
{
    if(nbElement <= 10)
    {
        for(uint8_t i=0; i<nbElement; i++)
        {
            SSD1306_DrawCirle((80-7*nbElement)+(i*10), 56, 2, i == IndexHighlight);
        }
    }
}



/**
 *   @brief    Dessin d'un rectangle
 *   
 *   @param[in]   Pos_x         Position sur l'axe des absices
 *   @param[in]   Pos_y         Position sur l'axe des ordonnées
 *   @param[in]   Height        Hauteur du rectangle
 *   @param[in]   Length        Largeur du rectangle
 *   @param[in]   IsFilled      TRUE  ->  Cercle rempli
 *                              FALSE ->  Cercle vide
 */
void SSD1306_DrawRect  (    uint8_t Pos_x,
                            uint8_t Pos_y,
                            uint8_t Height, 
                            uint8_t Length, 
                            bool    IsFilled
                        )
{
    if(IsFilled)
    {
        display.fillRect(Pos_x, Pos_y, Length, Height, SSD1306_WHITE);
    }
    else
    {
        display.drawRect(Pos_x, Pos_y, Length, Height, SSD1306_WHITE);
    }
}


 /**
  *   @brief    Dessin d'une jauge de remplissage horizontal
  *   
  *   @param[in]    Jauge_Level_pr100     Niveau de remplissage en %
  *   @param[in]    Jauge_X               Coordonné en X du départ de la jauge (point en haut à droite)
  *   @param[in]    Jauge_Y               Coordonné en Y du départ de la jauge (point en haut à droite)
  *   @param[in]    Jauge_Height          Hauteur du dessin de la jauge (rectangle exterieur)
  *   @param[in]    Jauge_Length          Longueur du dessin de la jauge (rectangle exterieur)
  */
void SSD1306_DrawJauge  (   uint8_t Jauge_Level_pr100,
                            uint8_t Jauge_X,  
                            uint8_t Jauge_Y,
                            uint8_t Jauge_Height,
                            uint8_t Jauge_Length
                        )
{
    // Reglage de l'épaisseur
    // (distance entre le rectangle intérieur et exterieur définissant la jauge)
    static const uint8_t Jauge_Epaisseur = 4;

    // Rectangle exterieur
    SSD1306_DrawRect  (    Jauge_X,      Jauge_Y,       // x, y
                           Jauge_Height, Jauge_Length,  // Height, Length,
                           false                        // Filled or Not filled
                       );

    // Rectangle interieur
    SSD1306_DrawRect  (    Jauge_X+Jauge_Epaisseur,          Jauge_Y+Jauge_Epaisseur,                                        // x, y
                           Jauge_Height-(Jauge_Epaisseur*2), ( (Jauge_Length-(Jauge_Epaisseur*2)) * Jauge_Level_pr100/100),  // Height, Length,
                           true                                                                                              // Filled or Not filled
                       );
}


/**
 *   @brief    Mise à jour du display
 *             A appeler en dernier.
 */
void SSD1306_Display(void)
{
  display.display();
}





/****************** END OF FILE ***************************************/
