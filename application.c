#include <xc.h>
#include <stdio.h>
#include "application.h"
#include "UART.h"

short int dco_ste_0, dco_ste_1;
short int f_signo;
short int f_contar;
extern int AN0value, AN1value;
long display_cuentas;
long decada_contadora;
short int f_contar;
void app_proces(void){
          
    _LATB9 = !cuadrador(&ADC1BUF0, HISTERESIS, dco_ste_0);  // AN0 -> pin18 RB9
    _LATB6 =  cuadrador(&ADC1BUF1, HISTERESIS, dco_ste_1);  // AN1 -> pin15 RB6
    
 return;   
}

void contador(int eNe_veces) {

    static char STATE_CONT;
    static int n;

    switch (STATE_CONT) {
        case IDLE:
            if (_RB9 == !DOWN){
                STATE_CONT = FF;
            } else
                break;
            //Idle Case, waiting for counter to start
        case FF:
            if (_RB9 == !UP) {
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
        case LATCH:
            
            display_cuentas = calcular_angulo(decada_contadora);
            
            sprintf(buff,"%ld °\r",display_cuentas);
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

long calcular_angulo(long N_cuentas){
    
    float N_cuentas_f;
    float angulo;
    
    N_cuentas_f = (float) N_cuentas;
    
    angulo = N_cuentas_f * 360 * 100000 / 1000 / 1000;
    
    return (long) angulo;
}

int cuadrador(volatile uint16_t *pVal, int histeresis, int DC_STATE ) {

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
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0; 
    return;
}
