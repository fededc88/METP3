
#include <p24FJ64GA002.h>
#include "HardwareDef.h"
#include "PWM.h"
#include "ADC.h"

int AN0value, AN1value;

#ifdef DEBUG2
int ANOvalues[LEN_SIN];
#endif

void AD_Init(void){
    
    // Turn off A/D
    AD1CON1bits.ADON = 0;
    
    //Select voltage reference source to match expected range on analog inputs
    AD1CON2bits.VCFG = 0b000; // Vr+ = AVDD Vr- = AVSS
    
    //Select the analog conversion clock to match desired data rate with processor clock
    //19,5 Tad/conv = 4 Sampling Tad + 12 conv Tad.
    // 4 * Tcy/2 *19.5 = 1/205,128kHz kHz muestreo
    //Al ser 2 entradas AN0 y AN1 -> 2015,128/2 = 102,5 KHz / entrada
    //OJO estamos cerca de la frecuencia de aliasing (60kHz) de muestreo, tener presente.
    AD1CON3bits.ADRC = 0; //Clock derived from system clock
    AD1CON3bits.SAMC = 0x07; //7 * Tad Auto-Sample Time bits ~ 1uS
    AD1CON3bits.ADCS = 0x04; // Tad = 4 * Tcy/2 ~ 250 nS - A/D Conversion Clock Select bits
    
    //AN0 set as analog input
    AD1PCFGbits.PCFG0 = 0; //Pin for corresponding analog channel is in Analog mode; port read input disabled, A/D module
                           //samples pin voltage
    TRISAbits.TRISA0 = 1; //If the data direction bit is a ?1?, then the pin

    //Determine how sampling will occur    
    AD1CON1bits.SSRC = 0b111; //Internal counter ends sampling and starts conversion (auto convert)
    AD1CON1bits.ASAM = 1; //Sampling begins immediately after last conversion completes; SAMP bit is automatically set.
    
    //Determine how inputs will be allocated to the S/H channel    
    AD1CHSbits.CH0NA = 0; //Channel 0 negative input is VRbit 
    AD1CSSL = 0x0003; // AN0 & AN1 are selected for sequential scanning on MUX A
    
    //Scan Input Selections for CH0+ S/H Input for MUX A Input Multiplexer Setting bit
    AD1CON2bits.CSCNA = 1; 
    //Select the desired sample/conversion sequence
    
    //Select how conversion results are presented in the buffer
    AD1CON1bits.FORM = 0b00;    //Integer (0000 00dd dddd dddd)
    
    //Select interrupt rate
    AD1CON2bits.SMPI = 0x1;
    
    //Configure A/D interrupt (if required):
    IFS0bits.AD1IF = 0; // Clear A/D conversion interrupt.
    
    // Configure A/D interrupt priority bits (AD1IP<2:0>) here, if
    // required. Default priority level is 4.
    IEC0bits.AD1IE = 1; // Enable A/D conversion interrupt
    
    //    In order to use pins multiplexed with the A/D for digital I/O, the corresponding bits in
//the AD1PCFG register must be set to ë1í, even if the A/D module is turned off.
    AD1PCFG = 0x9FFC;
    
    // Turn on A/D
    AD1CON1bits.ADON = 1; 
   
    return;
    
}
