/*
 * File:   main.c
 * Author: Federico David Ceccarelli (C-6241/3) 
 * 
 * Proyecto de Adscripción de la cátedra Sistemas y dispositivos Electrónicos II
 * de la carrera de Ingeniería electrónica de la Universidad Nacional de Rosario.
 * 
 * Hardware y Software desarrollado bajo la cátedra de Sistemas Digitales IV
 * 
 * Puede contactarse a fededc88@gmail.com para mayor información
 *  
 * Created on 29 de julio de 2018, 16:33
 */
#include "ConfigurationsBits.h"   // PIC24FJ64GA002 Configuration Bit Settings

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "../stdbool.h"
#include "dsp.h"

#include "HardwareDef.h"

#include "Configuration.h"
#include "UART.h"
#include "Switches.h"
#include "PWM.h"
#include "PWM_Extern.h"
#include "ADC.h"
#include "application.h"

void Peripherals_Init(void);

extern SWnState Sw1, Sw2, Sw3, Sw4;
extern short int f_contar;

_Bool clk;

#ifdef DEBUG2
    extern volatile int ANOvalues[LEN_SIN];
#endif

char buff[60];
int ReceivedChar = 0;

int main(void) {
    
#ifdef DEBUG1 || DEBUG2
    int i;
#endif
    //Inicialización módulos y periféricos
    Peripherals_Init();

    while (1) {

        //   app_proces();
        contador(N_PERIODOS);
        Sw_app();

        // <editor-fold defaultstate="collapsed" desc="DEBUG code">
#ifdef DEBUG1
        if (f_debug_sin == TRUE) {
            if (_Sw1 == PRESSED) {
                SendStringPolling("START step_array[j]\r\n");
                for (i = 0; i < LEN_SIN; i++) {
                    SendFloatPolling(step_array[i]);
                    SendStringPolling("\r\n");
                }
                SendStringPolling("END step_array[j++]\r\n");
                f_debug_sin = FALSE;

                SendStringPolling("\n\rSTART OC1R_array[j]\r\n");
                for (i = 0; i < LEN_SIN; i++) {
                    SendIntPolling(OC1R_array[i]);
                    SendStringPolling("\r\n");
                }
                SendStringPolling("END OC1R_array[j++]\r\n");
                f_debug_sin = FALSE;
            }
        }
#endif

#ifdef DEBUG2
        if (_Sw2 == PRESSED) {
            SendStringPolling("START ANOvalues[i]\r\n");
            for (i = 0; i < LEN_SIN; i++) {
                SendIntPolling(ANOvalues[i]);
                SendStringPolling("\r\n");
            }
        }
#endif

        //asd// </editor-fold>

    }
    return 0;
}

 //Inicialización módulos y periféricos
void Peripherals_Init(void) {

    // PIC24FJ64GA002 Clock Setting Init
    Clock_Init();

    UART1_Init(9600);
    Timer1_Init(100);

    Sw_Pin_Init();
    Sw_Init();

    //  Interrupts();

    // Timer2 function as Time Base for PWM
    Timer2_Init(f_PWM);
    PWM1_Init(0);
    
    //ADC1 Init
    AD_Init();

    //Sin wave Output Init and imput
    sin_Init(SIN_FREQ);
    Cuadrator_Pin_Init();

    // Timer3 function as Time Base for Fr
    Timer3_Init(FR_FREQ);
    sin_freq = FR_FREQ;

#ifdef DEBUG0
    SendStringPolling("\r\nStarted! \r\n");
#endif

    return;
}

//   UART1TX ISR - Disabled:
void _IRQ _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; //Clean UART1RX interrupt Flag
    return;

}

//   UART1RX ISR - Disabled:
void _IRQ _U1RXInterrupt(void) {

    /*Si o si, dentro de el handler de la interrupción hay que leer el Buffer de recepción 
    para baciarlo.*/

    ReceivedChar = U1RXREG;
    //TODO: Rehacer el ECO de la UART por IRQ!  
    //        sprintf(buff,"%c\r\n",ReceivedChar);
    //        SendStringPolling(buff);

    IFS0bits.U1RXIF = 0; //Clean UART1RX interrupt Flag
    return;

}

//   Timer1 ISR - SW Handler Interrupt:
void _IRQ _T1Interrupt(void) {

    /* Interrupt Service Routine code goes here */

    debouncingSwn(&Sw1, 1);
    debouncingSwn(&Sw2, 2);
    debouncingSwn(&Sw3, 3);
    debouncingSwn(&Sw4, 4);


    IFS0bits.T1IF = 0; //Reset Timer1 interrupt flag and Return from ISR
    return;
}

//   Timer2 ISR - PWM Handler Interrupt:
void _IRQ _T2Interrupt(void) {
    /* Interrupt Service Routine code goes here */
    static long i = 0;
    static int indice = 0;
#ifdef DEBUG1
    static int j;
#endif
    //    Toogle pin 18 to mesure interrupt time    
    //    _LATB9 ^= 1;

    //Load PWM buffer with duty cicle
    OC1RS = Sin[indice];

    //Make a step on Sin array
    i += sin_step;

    //circular buffer control
    if (i >= LEN_SIN_FP)
        i -= LEN_SIN_FP;
    
    // Divido por 100 para obtener la parte entera del indice i
    indice = __builtin_divsd(i, PARTE_DECIMAL);

#ifdef DEBUG1
    if (!f_debug_sin) {
        if (j < LEN_SIN)
            OC1R_array[j] = OC1R;
        step_array[j++] = i;

        if (!(j < LEN_SIN)) {
            f_debug_sin = TRUE;
            j = 0;
        }
    }
#endif

    IFS0bits.T2IF = 0; //clear T2 IRQ Flag
    return;
}

//   Timer3 ISR - Fr:
void _IRQ _T3Interrupt(void) {
    
  extern volatile long decada_contadora;
  
  //toogle signal
  //  _LATB8 ^=1;
    if(f_contar == 1)
        decada_contadora++;  
 IFS0bits.T3IF = 0; //clear T3 IRQ Flag
}

//   ADC1 ISR - Quadrator
void _IRQ _ADC1Interrupt(void) {
#ifdef DEBUG2   
    static int i;
#endif
 //   extern volatile int AN0value, AN1value;
 // Togle pin16 RB6 to time mesure    
 //   _LATB6 ^= 1;
    
    //Recupero los valores del buffer del periférico.
//    AN0value = ADC1BUF0;
//    AN1value = ADC1BUF1;
    
    //Recupero los valores del buffer del periférico y los cuadro.
    app_proces();
    
#ifdef DEBUG2   
    ANOvalues[i++] = ADC1BUF0;
    if (i>=LEN_SIN){
        i=0;
    }
#endif
    // Clear A/D conversion interrupt Flag.   
    IFS0bits.AD1IF = 0;
    return;
}
