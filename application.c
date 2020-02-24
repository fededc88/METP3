#include <xc.h>
#include "application.h"
#include "UART.h"

short int dco_ste_0, dco_ste_1;
extern int AN0value, AN1value;

void app_proces(void){
        
    //Cuadro la senoide y escribo el registro correspondiente al pin de salida
    _LATB9 = cuadrador(&AN0value, HISTERESIS, dco_ste_0);  // AN0 -> pin18 RB9
    _LATB6 = cuadrador(&AN1value, HISTERESIS-3, dco_ste_1);  // AN1 -> pin15 RB6
    
 return;   
}

void contador(void){
    
   static char STATE_CONT;
   
   switch(STATE_CONT){
       
       //Idle Case, waiting for counter to start
       case IDLE:
           break;
       
       //contando AN1 atrasa AN0   
       case ATRASO:
           break;
       
       //contando AN1 Adelanta AN0
       case ADELANTO:
           break; 
           
       //Latch value to display!     
       case LATCH:
           //Sin break
       case RESET:
           break;
           
           
           
           
   }
   
   
    
    
    
}


int cuadrador(int *pVal, int histeresis, int DC_STATE ) {

    int ANx_value;
    
    ANx_value = *pVal;

    switch (DC_STATE) {

        case UP:
            if( ANx_value < (SIN_CERO - histeresis) )
                DC_STATE = DOWN;
            break;

        case DOWN:
            if( ANx_value > (SIN_CERO + histeresis))
                DC_STATE = UP;
            
            break;

    }
    return DC_STATE;

}

void Cuadrator_Pin_Init(void){
               
    TRISBbits.TRISB9 = 0; // Set pin18 RB9 as output
    TRISBbits.TRISB6 = 0; // Set pin15 RB6 as output
    
    return;
}
