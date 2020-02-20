
#include <xc.h>
#include "HardwareDef.h"
#include "PWM.h"
#include <stdio.h>
#include <dsp.h>
#include <math.h>
#include "UART.h"


//PWM Variables definitions 
float Sin[LEN_SIN]; //SIN Buffer
float sin_step;
#ifdef DEBUG1
float step_array[LEN_SIN];
int   OC1R_array[LEN_SIN];
char f_debug_sin; //Flag inidicates buffer is ready. For debuging level 1
#endif


extern char buff[60];

/******************************************************************************
 * Function: PWM1_Init(int F_pwm1)
 * 
 * Input: int F_pwm1 -> unimplemented
 * Output: none
 * 
 * Description: Initialize Output Compare Module as PWM

 ******************************************************************************/
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
        
    //START!! OC1
    OC1CON = 0x0006; // Load new compare mode to OC1CON
    
    //interrupt is managed by T2
    return; 
    
}

/******************************************************************************
 * Function: sin_Init(float frecuencia);
 * 
 * Input: float frecuencia -> initial Sin Output Frecuency (in Hz)
 * Output: none
 * 
 * Description: Initialize Sin Array 

 ******************************************************************************/
void sin_Init(float f_sin_init) {
    int i;
    
    sin_step = set_sin_step(f_sin_init);
    
    for (i = 0; i < LEN_SIN; i++)
        Sin[i] =( (1 + cosf(2 * PI / LEN_SIN * i)) * (vm_PWM/2) ); //Senoide de paso 0.5 grados
        
#ifdef DEBUG1
    SendStringPolling("\r\nSTART Sin[i]:\r\n");
    for (i = 0; i < LEN_SIN; i++){
        SendFloatPolling(Sin[i]);
        SendStringPolling("\r\n");
    }
    SendStringPolling("END Sin[i]\r\n");
#endif
    return;
}

/******************************************************************************
 * Function: sin_set_step(float f_sin)f
 * 
 * Input: float f_sin -> Sin Output Frecuency (in Hz)
 * Output: none
 * 
 * Description: Set PWM1 Sin Output Frecuency (in Hz)

 ******************************************************************************/
float set_sin_step(float f_sin){
    
    float paso; 
    
    paso = LEN_SIN / ( f_PWM / f_sin );
    
    return paso;
}