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

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "../stdbool.h"

#include "HardwareDef.h"

#include "Configuration.h"
#include "UART.h"
#include "Switches.h"
#include "PWM.h"
#include "ADC.h"

extern SWnState Sw1, Sw2, Sw3, Sw4;
int AN0value, AN1value;

// <editor-fold defaultstate="collapsed" desc="Definición de variables Globales">//

//
//char buff[60] = {0};

int ReceivedChar = 0;
//
//int debug = 1; // To see serial print 

// </editor-fold>

int main(void) {


    // PIC24FJ64GA002 Configuration Bit Settings
    Configuration_Bit_Settings();

    // PIC24FJ64GA002 Clock Setting Init
    Clock_Init();

    UART1_Init(9600);
    Timer1_Init(1000);

    Sw_Pin_Init();
    Sw_Init();
    Interrupts();

    // Timer2 function as Time Base for PWM
    Timer2_Init(62500);
    PWM1_Init(0);

    AD_Init();

    // sprintf(buff,"Hola Mundo \r\n");
    SendStringPolling("Started! \r\n");

    while (1) {

        Sw_app();


    }
    return 0;
}

void _IRQ _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; //Clean UART1RX interrupt Flag
    return;

}

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
    IFS0bits.T2IF = 0; //clear T2 IRQ Flag
    return;
}

void _IRQ _OC1Interrupt(void) {
    /* Interrupt Service Routine code goes here */


    IFS0bits.OC1IF = 0; //clear OC1 IRQ Flag

}

void _IRQ _ADC1Interrupt(void) {
    // Clear A/D conversion interrupt.   
    AN0value = ADC1BUF0;
    AN1value = ADC1BUF1;
    IFS0bits.AD1IF = 0;
    return;
}
