/*
 * File:   Configuration.c
 * Author: Fededc88
 *
 * Created on 29 de julio de 2018, 18:06
 */


#include "Configuration.h"




void Configuration_Bit_Settings(void) {

// PIC24FJ64GA002 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSECME           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCDIV           // Oscillator Select (Fast RC Oscillator with Postscaler (FRCDIV))
#pragma config SOSCSEL = LPSOSC         // Sec Oscillator Select (Low Power Secondary Oscillator (LPSOSC))
#pragma config WUTSEL = LEG             // Wake-up timer Select (Legacy Wake-up Timer)
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = OFF              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = ON              // JTAG Port Enable (JTAG port is enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
}

void Clock_Init(void){

    

//Set new oscillator selection

    CLKDIVbits.RCDIV = 0b000;
    
    DELAY_105uS;
    

            asm volatile( 
         
                "MOV #OSCCONH, w1\n"
                "MOV.b #0x01, w0\n"
                "MOV #0x78, w2\n"
                "MOV #0x9A, w3\n"
                "MOV.b w2, [w1]\n"
                "MOV.b w3, [w1]\n"
         
                "MOV.b w0, [w1]\n"
         
                "MOV #OSCCONL,w1\n"
                "MOV.b #0x01, w0\n"
                "MOV #0x46, w2\n"
                "MOV #0x57, w3\n"
                "MOV.b w2, [w1]\n"
                "MOV.b w3, [w1]\n"

                "MOV.b w0, [w1]\n"
                                  ); Nop();
            
    DELAY_105uS;

            
while(OSCCONbits.OSWEN != 0){}
while(OSCCONbits.COSC != 0b001){}
 

 
}

void Interrupts(void){
    
    //INTCON1bits.NSTDIS = 1;
    
    SRbits.IPL = 1; //CPU Interrupt priority level is 1
       
    
}

void Timer1_Init(int Timer1_f){
    
    //Timer1_f Timer 1 frecuency on Hz 
//    
//    PR1 = 0x3E80;
//    T1CONbits.TCKPS = 0; // 1:1 Clock prescaler
    
//    
    if((Fcy/Timer1_f)<=65536){
        PR1 = (int) (Fcy/Timer1_f);
        T1CONbits.TCKPS = 0; // 1:1 Clock prescaler
    }
    else if (Fcy/Timer1_f/8 <= 65536 ){
        PR1 = (int) ((Fcy/8)/Timer1_f);
        T1CONbits.TCKPS = 1; // 1:8 Clock prescaler
    }
    else if (Fcy/Timer1_f/64 <= 65536 ){
        PR1 = (int) ((Fcy/64)/Timer1_f);
        T1CONbits.TCKPS = 2; // 1:64 Clock prescaler
    }
    else if (Fcy/Timer1_f/256 <= 65536 ) {
        PR1 = (int) ((Fcy/256)/Timer1_f);
        T1CONbits.TCKPS = 3; // 1:64 Clock prescaler
    }
    
    
    T1CONbits.TON = 1; //Enable Timer 1 
    
    
    
    IFS0bits.T1IF = 0; // Clear Interrupts T1 flag 
    IEC0bits.T1IE = 1; //Timer1 Interrupt Enable
      
     
}

