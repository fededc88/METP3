
#include <xc.h>
#include "PWM.h"
#include "HardwareDef.h"

void PWM1_Init(int F_pwm1){ 
    
/*---------------- OC - PWM - Remapeo de Pines ------------------------------*/
    
    __builtin_write_OSCCONL(OSCCON & 0xBF); // Unlock Registers
  
    // Configure Output Functions (Table 10-3)

    RPOR2bits.RP5R = 18; // Assign OC1 To Pin RP5    
//    RPOR1bits.RP2R = 19; // Assign OC2 To Pin RP2 //Not implemented for MEII
//    RPOR1bits.RP3R = 20; // Assign OC3 To Pin RP3 //Not implemented for MEII
        
    __builtin_write_OSCCONL(OSCCON | 0x40);     // Lock Registers

 /*-------------OC- PWM - FIN Remapéo de Pines --------------------------------*/
    
    // The following code example will set the Output Compare 1 module

    OC1CON = 0x0000; // Turn off Output Compare 1 Module
    
    OC1R = 0x0080; // Initialize Compare Register1 50%
    OC1RS = 0x0080; // Initialize Secondary Compare Register1 50%
    
    //Timer2 in initialized and enabled on Timer2_Init()
   // PR2 = 0x3E80; // Initialize PR2 with 16000 counts = 1Khz
    
    IPC0bits.OC1IP = 0b001; // Setup Output Compare 1 interrupt for
    IFS0bits.OC1IF = 0;     // Clear Output Compare 1 interrupt flag
    IEC0bits.OC1IE = 0;     // Enable Output Compare 1 interrupts
    
    //START!! OC1
    OC1CON = 0x0006; // Load new compare mode to OC1CON

}
