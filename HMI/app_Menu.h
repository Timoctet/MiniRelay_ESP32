/**********************************************************************************
 **********************************************************************************
        @date       10/07/2021
        @Dev        Duclos Timothe
        @file       app_Menu.h
*/

#ifndef APP_MENU_H
#define APP_MENU_H


 
/********************************************************************************** 
 *  Déclaration des types privés
 */
/**
  *   @brief    Liste des Pages
  */
typedef enum
{
  Page_Menu = 0,
  Page_Monitor,

  nb_Pages
  
}PagesListe_e;

 
/********************************************************************************** 
 *  Déclaration des fonctions privées
 */
void Page_Draw        (void);
void Page_HandleIndex (void);

PagesListe_e Page_GetCurrent(void);

#endif /* APP_MENU_H */

/****************** END OF FILE ***************************************/
