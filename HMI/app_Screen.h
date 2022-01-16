/**********************************************************************************
 **********************************************************************************
        @date       10/07/2021
        @Dev        Duclos Timothe
        @file       App_Screen.h
*/

#ifndef APP_SCREEN_H
#define APP_SCREEN_H


 
/********************************************************************************** 
 *  Déclaration de la classe
 */
 /**
  *   @brief    Définition d'une page
  */
class Screen {

    public:
        Screen  ( uint8_t IndexMax,  
                  void (*pf_DrawScreen)(uint32_t *pParamter),
                  bool    TypeOfIndex
                );


        
        void    HandleIndex(void);
        uint8_t getIndex();
        void    setIndex(uint8_t IndexValue);

        void    Configure_pIndex(uint8_t *pIndex, uint8_t IndexMax);
        
        void    RaZScreen();

        void      (*pf_DrawScreen)(uint32_t *pParamter);

        

        bool IsInit;
        

    private:
        uint8_t Index;      // Index des objets à l'interieur de la page
        uint8_t IndexMax;   // Valeur max que prendre l'index


        uint8_t *pIndex;

        uint32_t  *pParameter;

        
        bool TypeOfIndex;
        
        
};
 



#endif /* APP_SCREEN_H */

/****************** END OF FILE ***************************************/
