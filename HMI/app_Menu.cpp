/**********************************************************************************
 **********************************************************************************
        @date       10/07/2021
        @Dev        Duclos Timothe
        @file       app_Menu.h
*/

#include "Arduino.h"
#include "app_Menu.h"
#include "app_Screen.h"

#include "drv_Encodeur.h"
#include "drv_SSD1306.h"

/********************************************************************************** 
 *  Définition des constantes privées
 */
#define MENU_NOM_TAILLE_MAX   15




 
/********************************************************************************** 
 *  Définition des types privés
 */



/**
  *   @brief    Liste des Menus
  */
typedef enum
{
  Index_Menu_ = 0,
  Index_Monitpr,

  nb_Index_Menu
  
}IndexMenu_Liste_e;


 
/********************************************************************************** 
 *  Déclaration des fonctions privées
 */
static void Screen_MenuDraw           (uint32_t *pParam);
static void Screen_Monitor            (uint32_t *pParam);

/********************************************************************************** 
 *  Définition des variables privée
 */
static Screen Pages[nb_Pages] {
                                  // Page_Menu
                                  Screen(nb_Index_Menu-1, &Screen_MenuDraw, true),

                                  // Page_Remplir_Remplissage
                                  Screen(0, &Screen_Monitor, true),
};


 
static PagesListe_e CurrentPage = Page_Menu;



/********************************************************************************** 
 *  Définition des fonctions publics
 */

 /**
  *   @brief    Lecture de la page en cours
  *   @retval   PagesListe_e
  */
PagesListe_e Page_GetCurrent(void)
{
  return CurrentPage;
}

 /**
  *   @brief    Dessin de la page en cours
  */
void Page_Draw(void)
{
    Pages[CurrentPage].pf_DrawScreen(NULL);
}





/**
  *   @brief    Dessin de la page en cours
  */
void Page_HandleIndex(void)
{

    // Encodeur
    Pages[CurrentPage].HandleIndex();


    // Switch
    if(Encodeur_get_SW_State() == EncodeurSW_State_FallingEdge)
    {

        switch(CurrentPage) 
        {
            //-----------------------------------------
            case Page_Menu:

                  break;


            //-----------------------------------------      
            case Page_Monitor:
                  break;
        }
    }
}







/********************************************************************************** 
 *  Définition des fonctions privée
 */

 /**
  *   @brief    Dessin de la page menu
  *             Liste des menus
  */
static void Screen_MenuDraw(uint32_t *pParam)
{
    /**
     * @brief   Liste et nom des menus
     */
    static const char Menu_Name[nb_Index_Menu][MENU_NOM_TAILLE_MAX] = {
                        "Remplir", 
                        "Monitor" 
    };

    // Effacement du display
    SSD1306_ClearDisplay();

    // Ecriture du titre
    SSD1306_DrawString( (char *)"Menu", 45, 0, 1);
    
    // Ecriture des menus
    SSD1306_DrawString( (char *)Menu_Name[Pages[Page_Menu].getIndex()], 0, 25, 2);

    SSD1306_CircleMenu(nb_Index_Menu, Pages[Page_Menu].getIndex());

    // Refresh du display
    SSD1306_Display();
}




 /**
  *   @brief    
  */
static void Screen_Monitor            (uint32_t *pParam)
{
    // Effacement du display
    SSD1306_ClearDisplay();
    
    // Ecriture des menus

    // Refresh du display
    SSD1306_Display();
  
}





/****************** END OF FILE ***************************************/
