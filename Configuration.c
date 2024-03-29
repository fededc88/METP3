/*
 * File:   Configuration.c
 * Author: Fededc88
 *
 * Created on 29 de julio de 2018, 18:06
 */


#include "Configuration.h"
#include "HardwareDef.h"


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
                                  );
            Nop();
            
    DELAY_105uS;
    
while(OSCCONbits.OSWEN != 0){}
while(OSCCONbits.COSC != 0b001){}
 
}

void Interrupts(void){
    
    //INTCON1bits.NSTDIS = 1;
    
    SRbits.IPL = 1; //CPU Interrupt priority level is 1
       
    
}

void Timer1_Init(long f_Hz){
    
    //Timer1_f Timer 1 frecuency on Hz 
//    
//    PR1 = 0x3E80;
//    T1CONbits.TCKPS = 0; // 1:1 Clock prescaler
    
//    
    if((Fcy/f_Hz)<=65536){
        PR1 = (int) (Fcy/f_Hz);
        T1CONbits.TCKPS = 0; // 1:1 Clock prescaler
    }
    else if (Fcy/f_Hz/8 <= 65536 ){
        PR1 = (int) ((Fcy/8)/f_Hz);
        T1CONbits.TCKPS = 1; // 1:8 Clock prescaler
    }
    else if (Fcy/f_Hz/64 <= 65536 ){
        PR1 = (int) ((Fcy/64)/f_Hz);
        T1CONbits.TCKPS = 2; // 1:64 Clock prescaler
    }
    else if (Fcy/f_Hz/256 <= 65536 ) {
        PR1 = (int) ((Fcy/256)/f_Hz);
        T1CONbits.TCKPS = 3; // 1:64 Clock prescaler
    }
    
    T1CONbits.TON = 1; //Enable Timer 1
    
    IFS0bits.T1IF = 0; // Clear Interrupts T1 flag 
    IEC0bits.T1IE = 1; //Timer1 Interrupt Enable
      
     
}

void Timer2_Init(long f_Hz){
    
    //Timer1_f Timer 1 frecuency on Hz 
//    
//    PR1 = 0x3E80;
//    T1CONbits.TCKPS = 0; // 1:1 Clock prescaler
    
//    
    if((Fcy/f_Hz)<=65536){
        PR2 = (int) (Fcy/f_Hz);
        T2CONbits.TCKPS = 0; // 1:1 Clock prescaler
    }
    else if (Fcy/f_Hz/8 <= 65536 ){
        PR2 = (int) ((Fcy/8)/f_Hz);
        T2CONbits.TCKPS = 1; // 1:8 Clock prescaler
    }
    else if (Fcy/f_Hz/64 <= 65536 ){
        PR2 = (int) ((Fcy/64)/f_Hz);
        T2CONbits.TCKPS = 2; // 1:64 Clock prescaler
    }
    else if (Fcy/f_Hz/256 <= 65536 ) {
        PR2 = (int) ((Fcy/256)/f_Hz);
        T2CONbits.TCKPS = 3; // 1:64 Clock prescaler
    }
        
    T2CONbits.TON = 1; //Enable Timer 2
    
    IPC1bits.T2IP = 6; // Setup Output Compare 1 interrupt for
    IFS0bits.T2IF = 0; // Clear Output Compare 1 interrupt flag
    IEC0bits.T2IE = 1; // Enable Output Compare 1 interrupts
    
    return;
}


void Timer3_Init(long f_Hz){
    
    //Timer1_f Timer 1 frecuency on Hz 
//    
//    PR1 = 0x3E80;
//    T1CONbits.TCKPS = 0; // 1:1 Clock prescaler
    
//    
    if((Fcy/f_Hz)<=65536){
        PR3 = (int) (Fcy/f_Hz);
        T3CONbits.TCKPS = 0; // 1:1 Clock prescaler
    }
    else if (Fcy/f_Hz/8 <= 65536 ){
        PR3 = (int) ((Fcy/8)/f_Hz);
        T3CONbits.TCKPS = 1; // 1:8 Clock prescaler
    }
    else if (Fcy/f_Hz/64 <= 65536 ){
        PR3 = (int) ((Fcy/64)/f_Hz);
        T3CONbits.TCKPS = 2; // 1:64 Clock prescaler
    }
    else if (Fcy/f_Hz/256 <= 65536 ) {
        PR3 = (int) ((Fcy/256)/f_Hz);
        T3CONbits.TCKPS = 3; // 1:64 Clock prescaler
    }
        
    T3CONbits.TON = 1; //Enable Timer 2
    
    IPC2bits.T3IP = 7; // Setup Timer3 interrupt for
    IFS0bits.T3IF = 0; // Clear Timer3 interrupt flag
    IEC0bits.T3IE = 1; // Disable Timer3 interrupts
    
    return;
}
