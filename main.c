/*
 * File:   main.c
 * Author: Federico David Ceccarelli (C-6241/3) 
 * 
 * Proyecto de Adscripci�n de la c�tedra Sistemas y dispositivos Electr�nicos II
 * de la carrera de Ingenier�a electr�nica de la Universidad Nacional de Rosario.
 * 
 * Hardware y Software desarrollado bajo la c�tedra de Sistemas Digitales IV
 * 
 * Puede contactarse a fededc88@gmail.com para mayor informaci�n
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

extern SWnState Sw1, Sw2, Sw3, Sw4;

#ifdef DEBUG2
    extern volatile int ANOvalues[LEN_SIN];
#endif

char buff[60];
int ReceivedChar = 0;
int contador; 

int main(void) {

    int i;

    // PIC24FJ64GA002 Clock Setting Init
    Clock_Init();

    UART1_Init(9600);
    Timer1_Init(500);

    Sw_Pin_Init();
    Sw_Init();

    //  Interrupts();

    // Timer2 function as Time Base for PWM
    Timer2_Init(f_PWM);
    PWM1_Init(0);

    AD_Init();

    sin_Init(Seno_f_Ini);
    
    Cuadrator_Pin_Init();
    
    // Timer3 function as Time Base for Fr
    Timer3_Init(100000);

#ifdef DEBUG0
    SendStringPolling("Started! \r\n");
#endif

    while (1) {

        
        app_proces();
        Sw_app();

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
    }
    return 0;
}

void _IRQ _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; //Clean UART1RX interrupt Flag
    return;

}

void _IRQ _U1RXInterrupt(void) {

    /*Si o si, dentro de el handler de la interrupci�n hay que leer el Buffer de recepci�n 
    para baciarlo.*/

    ReceivedChar = U1RXREG;
    //TODO: Rehacer el ECO de la UART por IRQ!  
    //        sprintf(buff,"%c\r\n",ReceivedChar);
    //        SendStringPolling(buff);

    IFS0bits.U1RXIF = 0; //Clean UART1RX interrupt Flag
    return;

}

void _IRQ _T1Interrupt(void) {

    /* Interrupt Service Routine code goes here */

    debouncingSwn(&Sw1, 1);
    debouncingSwn(&Sw2, 2);
    debouncingSwn(&Sw3, 3);
    debouncingSwn(&Sw4, 4);


    IFS0bits.T1IF = 0; //Reset Timer1 interrupt flag and Return from ISR
    return;
}

//   Timer2 ISR:

void _IRQ _T2Interrupt(void) {
    /* Interrupt Service Routine code goes here */
    static int i = 0, error;
#ifdef DEBUG1
    static int j;
#endif
 //   _LATB9 ^= 1;

    //Load PWM buffer with duty cicle
    OC1RS = Sin[i];

    //Make a step on Sin array
    i += sin_step;
    ////    should add one count every steps_delta steps
    if (++error == step_delta) {
        i++;
        error = 0;
    }
    //circular buffer control
    if (i >= LEN_SIN)
        i -= LEN_SIN;

#ifdef DEBUG1
    if (!f_debug_sin) {
        if (j < LEN_SIN)
            OC1R_array[j] = OC1R;
        step_array[j++] = a;

        if (!(j < LEN_SIN)) {
            f_debug_sin = TRUE;
            j = 0;
        }
    }
#endif

    IFS0bits.T2IF = 0; //clear T2 IRQ Flag
    return;
}

void _IRQ _T3Interrupt(void) {
    _LATB6 ^=1;
    
    if (_RB6 && _RB9)
        contador++;
    
    IFS0bits.T3IF = 0; //clear T3 IRQ Flag
}

void _IRQ _ADC1Interrupt(void) {
#ifdef DEBUG2   
    static int i;
#endif
    extern volatile int AN0value, AN1value;

 //   _LATB6 ^= 1;
    //Recupero los valores del buffer del perif�rico.
    AN0value = ADC1BUF0;
    AN1value = ADC1BUF1;
    
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
