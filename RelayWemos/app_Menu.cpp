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
#include "drv_DS3231.h"
#include "app_Alarm.h"


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
  Index_Menu_SetTime = 0,
  Index_Menu_SetAlarmON,
  Index_Menu_SetAlarmOFF,
  Index_Menu_Monitor,

  nb_Index_Menu
  
}IndexMenu_Liste_e;


typedef enum
{
    Index_SetTime_SetHour,
    Index_SetTime_SetMinutes,
    Index_SetTime_SetSeconds,
    
    nb_Index_SetTime
    
}Index_SetTime_e;

 
/********************************************************************************** 
 *  Déclaration des fonctions privées
 */
static void Screen_MenuDraw           (uint32_t *pParam);
static void Screen_Monitor            (uint32_t *pParam);
static void Screen_SetTimeDraw        (uint32_t *pParam);
static void Screen_SetAlarmOnDraw     (uint32_t *pParam);
static void Screen_SetAlarmOffDraw    (uint32_t *pParam);

/********************************************************************************** 
 *  Définition des variables privée
 */
static DS3231_Time_s ModifyTime;    // Variable temporaire qui servira à modifier l'heure

 
static Screen Pages[nb_Pages] {
                                  // Page_Menu
                                  Screen(nb_Index_Menu-1, &Screen_MenuDraw, true),

                                  Screen(nb_Index_SetTime, &Screen_SetTimeDraw,     false),
                                  Screen(nb_Index_SetTime, &Screen_SetAlarmOnDraw,  false),
                                  Screen(nb_Index_SetTime, &Screen_SetAlarmOffDraw, false),

                                  // Page_Remplir_Remplissage
                                  Screen(0, &Screen_Monitor, true),
};



static void Screen_SetTimeDraw(uint32_t *pParam)
{
    char message[30] = {0};

    if(Pages[Page_SetTime].IsInit == false)
    {
        DS3231_get(&ModifyTime);
        Pages[Page_SetTime].IsInit = true;
    }
    
    // Effacement du display
    SSD1306_ClearDisplay();

    // Ecriture du titre
    SSD1306_DrawString( (char *)"Set Time", 45, 0, 1);

    snprintf(message, 9, "%02d:%02d:%02d", ModifyTime.hour, ModifyTime.min, ModifyTime.sec);
    
    // Ecriture des menus
    SSD1306_DrawString( message, 0, 25, 2);

    // Refresh du display
    SSD1306_Display();
    
}



static void Screen_SetAlarmOnDraw     (uint32_t *pParam)
{
    char message[30] = {0};

    if(Pages[Page_SetAlarmON].IsInit == false)
    {
        GetAlarmON(&ModifyTime);
        Pages[Page_SetAlarmON].IsInit = true;
    }
    
    // Effacement du display
    SSD1306_ClearDisplay();

    // Ecriture du titre
    SSD1306_DrawString( (char *)"Al oN", 45, 0, 1);

    snprintf(message, 9, "%02d:%02d:%02d", ModifyTime.hour, ModifyTime.min, ModifyTime.sec);
    
    // Ecriture des menus
    SSD1306_DrawString( message, 0, 25, 2);

    // Refresh du display
    SSD1306_Display();
}

static void Screen_SetAlarmOffDraw    (uint32_t *pParam)
{
    char message[30] = {0};

    if(Pages[Page_SetAlarmOFF].IsInit == false)
    {
        GetAlarmOFF(&ModifyTime);
        Pages[Page_SetAlarmOFF].IsInit = true;
    }
    
    // Effacement du display
    SSD1306_ClearDisplay();

    // Ecriture du titre
    SSD1306_DrawString( (char *)"Al oFF", 45, 0, 1);

    snprintf(message, 9, "%02d:%02d:%02d", ModifyTime.hour, ModifyTime.min, ModifyTime.sec);
    
    // Ecriture des menus
    SSD1306_DrawString( message, 0, 25, 2);

    // Refresh du display
    SSD1306_Display();
}




 
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
    static uint8_t old_CurrentPage = 0;

    // Check si on a besoin de configurer l'index
    if  (     (CurrentPage == Page_SetTime)
          ||  (CurrentPage == Page_SetAlarmON)
          ||  (CurrentPage == Page_SetAlarmOFF)
        )
    {
          if(Pages[CurrentPage].getIndex() == Index_SetTime_SetHour)
          {
              Pages[CurrentPage].Configure_pIndex(&ModifyTime.hour, 23);
          }
          else
          {
              Pages[CurrentPage].Configure_pIndex(&ModifyTime.min, 59);
          }
    }


    // Encodeur
    Pages[CurrentPage].HandleIndex();


    // Switch
    if(Encodeur_get_SW_State() == EncodeurSW_State_FallingEdge)
    {

        switch(CurrentPage) 
        {
            //-----------------------------------------
            case Page_Menu:

                  switch(Pages[CurrentPage].getIndex())
                  {
                      case Index_Menu_SetTime:
                          CurrentPage = Page_SetTime;
                          break;
                          
                      case Index_Menu_SetAlarmON:
                          CurrentPage = Page_SetAlarmON;
                          break;
                          
                      case Index_Menu_SetAlarmOFF:
                          CurrentPage = Page_SetAlarmOFF;
                          break;
                          
                      case Index_Menu_Monitor:
                          CurrentPage = Page_Monitor;
                          break;
                  }
                  
                  break;
                  
            //-----------------------------------------
            case Page_SetTime:

                
                  switch(Pages[CurrentPage].getIndex())
                  {
                      case Index_SetTime_SetHour:
                          Pages[CurrentPage].setIndex(Index_SetTime_SetMinutes);
                          break;
                          
                      case Index_SetTime_SetMinutes:
                          Pages[CurrentPage].setIndex(Index_SetTime_SetSeconds);
                          break;
                          
                      case Index_SetTime_SetSeconds:
                          DS3231_set(&ModifyTime);
                          CurrentPage = Page_Menu;
                          break;
                  }
                  
                  break;

                  
                  
            //-----------------------------------------
            case Page_SetAlarmON:

                  switch(Pages[CurrentPage].getIndex())
                  {
                      case Index_SetTime_SetHour:
                          Pages[CurrentPage].setIndex(Index_SetTime_SetMinutes);
                          break;
                          
                      case Index_SetTime_SetMinutes:
                          Pages[CurrentPage].setIndex(Index_SetTime_SetSeconds);
                          break;
                          
                      case Index_SetTime_SetSeconds:
                          SetAlarmON(&ModifyTime);
                          CurrentPage = Page_Menu;
                          break;

                      default:
                          Pages[CurrentPage].setIndex(Index_SetTime_SetHour);
                          break;
                  }
                  
                  break;

            //-----------------------------------------
            case Page_SetAlarmOFF:

                  switch(Pages[CurrentPage].getIndex())
                  {
                      case Index_SetTime_SetHour:
                          Pages[CurrentPage].setIndex(Index_SetTime_SetMinutes);
                          break;
                          
                      case Index_SetTime_SetMinutes:
                          Pages[CurrentPage].setIndex(Index_SetTime_SetSeconds);
                          break;
                          
                      case Index_SetTime_SetSeconds:
                          SetAlarmOFF(&ModifyTime);
                          CurrentPage = Page_Menu;
                          break;

                      default:
                          Pages[CurrentPage].setIndex(Index_SetTime_SetHour);
                          break;
                  }
                  
                  break;

            //-----------------------------------------      
            case Page_Monitor:
                  CurrentPage = Page_Menu;
                  break;
        }
    }


    if(old_CurrentPage != CurrentPage)
    {
      Pages[old_CurrentPage].RaZScreen();
      old_CurrentPage = CurrentPage;
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
                        "Set time",
                        "alarm ON",
                        "alarm OFF",
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
    
    SSD1306_DrawString( (char *)"Monitor", 45, 0, 1);

    // Refresh du display
    SSD1306_Display();
  
}





/****************** END OF FILE ***************************************/
