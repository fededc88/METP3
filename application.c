#include <xc.h>
#include <stdio.h>
#include "application.h"
#include "UART.h"
#include "PWM.h"
#include "PWM_Extern.h"

short int dco_ste_0, dco_ste_1;
short int f_signo;
short int f_contar;
extern int AN0value, AN1value;
long display_cuentas;
long decada_contadora;
short int f_contar;


void app_proces(void){
          
    _LATB9 =  !cuadrador(&ADC1BUF0,SIN_CERO, HISTERESIS, dco_ste_0);  // AN0 -> pin18 RB9
    _LATB6 =  cuadrador(&ADC1BUF1,512, HISTERESIS, dco_ste_1);  // AN1 -> pin15 RB6
    
 return;   
}

void contador(int eNe_veces) {

    static char STATE_CONT;
    static int n;
    int display_cuentas_ent, display_cuentas_dec;
    
    switch (STATE_CONT) {
        case IDLE:
            if (_RB9 == !DOWN){
                STATE_CONT = FF;
            } else
                break;
            //Idle Case, waiting for counter to start
        case FF:
            if (_RB9 == !UP) {
                _LATB7 = UP;
                STATE_CONT = CONTANDO;
                f_contar = 1;
            }
//            else if (_RB6 == !UP) {
//                STATE_CONT = CONTANDO;
//                f_signo = 1;
//                IEC0bits.T3IE = 1; //Enable Timer3 Interrupt
////                IFS0bits.T2IF = 1; // Up Timer3 flag
//            }
            else
                break;

            //contando AN1 atrasa AN0   
        case CONTANDO:
            //decada_contadora++; //Contando en la interrupción del Timer3
            if ( _RB6 == !UP) {
                _LATB7 = DOWN;
                STATE_CONT = VECES;
                f_contar = 0;
            } 
//            else if (_RB9 == !UP && f_signo == 1) {
//                STATE_CONT = VECES;
//     //           IFS0bits.T2IF = 0; // Clear Timer3 flag
//                IEC0bits.T3IE = 0; //Disable Timer3 Interrupt
//            }
            else 
                break;

        case VECES:
            
            ++n;
            STATE_CONT = IDLE;
            if (n >= eNe_veces) 
                STATE_CONT = LATCH;
            else
                break;
            //Latch value to display!     
        case LATCH: //DISPLAY:
                           // calcular_angulo(long Nx, int N_muestras, float sin_freq, long fr_freq )
            display_cuentas = calcular_angulo(decada_contadora, N_PERIODOS, sin_freq, FR_FREQ );
            
            display_cuentas_ent = display_cuentas/100;
            display_cuentas_dec = display_cuentas % 100;

            sprintf(buff,"%d.%02d Deg\r", display_cuentas_ent, display_cuentas_dec);
            SendStringPolling(buff);
            
            //Sin break
        case RESET:
            decada_contadora = 0;
            n = 0;
            f_signo = 0;
                STATE_CONT = IDLE;
            break;
    }

    return;

}

long calcular_angulo(long Nx, int N_muestras, float sin_freq, long fr_freq ){
    
    float N_cuentas_f;
    float angulo;
    
    N_cuentas_f = (float) Nx;
    
    angulo = (N_cuentas_f /fr_freq /N_muestras) * (360 * sin_freq );
    
//    if(angulo > 180){
//        angulo -= 180;
//    }
    
    //Multiplico por 100 para poder recuperar la parte decimal
    return (long) (angulo * 100);
}

int cuadrador(volatile uint16_t *pVal, int Cero, int histeresis, int DC_STATE ) {

    int ANx_value;
    
    ANx_value = *pVal;

    switch (DC_STATE) {

        case UP:
            if( ANx_value < (Cero - histeresis) )
                DC_STATE = DOWN;
            break;

        case DOWN:
            if( ANx_value > (Cero + histeresis))
                DC_STATE = UP;
            
            break;

    }
    return DC_STATE;

}

void Cuadrator_Pin_Init(void){
               
    TRISBbits.TRISB9 = 0; // Set pin18 RB9 as output
    TRISBbits.TRISB6 = 0; // Set pin15 RB6 as output
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0; 
    return;
}
