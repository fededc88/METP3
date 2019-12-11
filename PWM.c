

#include "PWM.h"

void PWM_Init (int F_pwm1){
    
    /*F_pwm correspond to PWM1 frecuency 
     
     Fcy = 1/2*Fos ; Fos = 32 Mhz*/
    
    
    
/*---------------- OC - PWM - Remapeo de Pines ------------------------------*/
    
    __builtin_write_OSCCONL(OSCCON & 0xBF); // Unlock Registers
  
    // Configure Output Functions (Table 10-3)

    RPOR2bits.RP5R = 18; // Assign OC1 To Pin RP5    
    RPOR1bits.RP2R = 19; // Assign OC2 To Pin RP2 
    RPOR1bits.RP3R = 20; // Assign OC3 To Pin RP3
        

    __builtin_write_OSCCONL(OSCCON | 0x40);     // Lock Registers

 /*-------------OC- PWM - FIN Remapéo de Pines --------------------------------*/
    
    OC1CON = 0x0000; // Turn off Output Compare 1 Module
    
    PR3 = (int) (Fcy / F_pwm1);
    
    OC1RS = 128/2; // 50% Duty Cycle
    OC1R = 128/2; 
    
    OC1CONbits.OCM = 0b110;
    OC1CONbits.OCTSEL = 1; //Timer 3 in the clock source for Output Compare 1
    
    
    IFS0bits.OC1IF = 0; //clear OC1 IRQ Flag
    IEC0bits.OC1IE = 1; // Enable OC1 IRQ Flag
       
}
