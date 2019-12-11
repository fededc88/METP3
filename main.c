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

#include <stdlib.h>
#include <stdio.h>
#include "../stdbool.h"

#include "xc.h"
#include "Configuration.h"
#include "UART.h"
#include "Switches.h"




 // <editor-fold defaultstate="collapsed" desc="Definición de variables Globales">//

 
char buff[60] = {0};

int ReceivedChar = 0;

int debug = 1; // To see serial print 

extern SWnState Sw1, Sw2, Sw3, Sw4;
  
 
 // </editor-fold>
 


int main(void) {
    
    Configuration_Bit_Settings();
    Clock_Init();
    UART1_Init(9600);
    Timer1_Init(1000);
    Pin_Init();
    Interrupts();
    
   // sprintf(buff,"Hola Mundo \r\n");
    SendStringPolling("Hola Mundo \r\n");
    
    while(1){
        if(Sw1.StateChange == 1 && Sw1.KeyState == PRESSED){
            Sw1.StateChange = 0;
            sprintf(buff,"Sw1() PUSSHH!! \r\n");
            SendStringPolling(buff);
        }
        if(Sw1.KeyState == HOLD && Sw1.StateChange == 1 ){
            Sw1.StateChange = 0;
            sprintf(buff,"Sw1() HOLD \r\n");
            SendStringPolling(buff);
        }
        
         if(Sw2.StateChange == 1 && Sw2.KeyState == PRESSED){
            Sw2.StateChange = 0;
            sprintf(buff,"Sw2() PUSSHH!! \r\n");
            SendStringPolling(buff);
        }
        
        if(Sw2.KeyState == HOLD && Sw2.StateChange == 1 ){
            Sw2.StateChange = 0;
            sprintf(buff,"Sw2() HOLD \r\n");
            SendStringPolling(buff);
        }
        
        if(Sw3.StateChange == 1 && Sw3.KeyState == PRESSED){
            Sw3.StateChange = 0;
            sprintf(buff,"Sw3() PUSSHH!! \r\n");
            SendStringPolling(buff);
        }
        if(Sw3.KeyState == HOLD && Sw3.StateChange == 1 ){
            Sw3.StateChange = 0;
            sprintf(buff,"Sw3() HOLD \r\n");
            SendStringPolling(buff);
        }
        
        if(Sw4.StateChange == 1 && Sw4.KeyState == PRESSED){
            Sw4.StateChange = 0;
            sprintf(buff,"Sw4() PUSSHH!! \r\n");
            SendStringPolling(buff);
        }
        if(Sw4.KeyState == HOLD && Sw4.StateChange == 1 ){
            Sw4.StateChange = 0;
            sprintf(buff,"Sw4() HOLD \r\n");
            SendStringPolling(buff);
        }
       
 //  SerialPrintSw(debug, buff);     
//        
//        if (IFS0bits.U1RXIF == 1){
//                sprintf(buff,"INTERRUMPI! \r\n");
//                SendStringPolling(buff);
//                IFS0bits.U1RXIF = 0; //Clean UART1RX interrupt Flag
//        }

    
    }      
    return 0;
}


    
    void _IRQ _U1TXInterrupt(void){
    IFS0bits.U1TXIF = 0; //Clean UART1RX interrupt Flag
        
    }
    
    void _IRQ _U1RXInterrupt(void){
      
        
        /*Si o si, dentro de el handler de la interrupción hay que leer el Buffer de recepción 
        para baciarlo.*/
                
        ReceivedChar = U1RXREG;
        sprintf(buff,"%c\r\n",ReceivedChar);
        SendStringPolling(buff);
    
    IFS0bits.U1RXIF = 0; //Clean UART1RX interrupt Flag
       
    }
        
    void _IRQ _T1Interrupt(void){
    

/* Interrupt Service Routine code goes here */

      debouncingSwn(&Sw1,1);
      debouncingSwn(&Sw2,2);
      debouncingSwn(&Sw3,3);
      debouncingSwn(&Sw4,4);
       

IFS0bits.T1IF = 0; //Reset Timer1 interrupt flag and Return from ISR
    }
    
    void _IRQ _OC1Interrupt(void ){
 /* Interrupt Service Routine code goes here */       
        
 
        
IFS0bits.OC1IF = 0; //clear OC1 IRQ Flag
    
    }
    
    
    