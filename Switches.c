
#include "Switches.h"

SWnState Sw1, Sw2, Sw3, Sw4;

void Sw_app(void){
    
    char buff[60];

    if (Sw1.StateChange == 1 && Sw1.LastState == PRESSED) {
        Sw1.StateChange = 0;
        sprintf(buff, "Sw1() PUSSHH!! \r\n");
        SendStringPolling(buff);
    }
    if (Sw1.LastState == HOLD && Sw1.StateChange == 1) {
        Sw1.StateChange = 0;
        sprintf(buff, "Sw1() HOLD \r\n");
        SendStringPolling(buff);
    }

    if (Sw2.StateChange == 1 && Sw2.LastState == PRESSED) {
        Sw2.StateChange = 0;
        sprintf(buff, "Sw2() PUSSHH!! \r\n");
        SendStringPolling(buff);
    }

    if (Sw2.LastState == HOLD && Sw2.StateChange == 1) {
        Sw2.StateChange = 0;
        sprintf(buff, "Sw2() HOLD \r\n");
        SendStringPolling(buff);
    }

    if (Sw3.StateChange == 1 && Sw3.LastState == PRESSED) {
        Sw3.StateChange = 0;
        sprintf(buff, "Sw3() PUSSHH!! \r\n");
        SendStringPolling(buff);
    }
    if (Sw3.LastState == HOLD && Sw3.StateChange == 1) {
        Sw3.StateChange = 0;
        sprintf(buff, "Sw3() HOLD \r\n");
        SendStringPolling(buff);
    }

    if (Sw4.StateChange == 1 && Sw4.LastState == PRESSED) {
        Sw4.StateChange = 0;
        sprintf(buff, "Sw4() PUSSHH!! \r\n");
        SendStringPolling(buff);
    }
    if (Sw4.LastState == HOLD && Sw4.StateChange == 1) {
        Sw4.StateChange = 0;
        sprintf(buff, "Sw4() HOLD \r\n");
        SendStringPolling(buff);
    }
    
    
}


bool _Swn(int SelectedSw){
    
    switch (SelectedSw){
        case 1: 
             return PORTAbits.RA2;
             break;
        case 2: 
            return PORTAbits.RA3;
            break;
        case 3: 
            return PORTBbits.RB4;
            break;
        case 4:
            return PORTAbits.RA4;
            break;
        default:
            return 1;
            break; 
    }
    return 0;
}


void Sw_Pin_Init(void){
    
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
    
    return;
    
}


void debouncingSwn(SWnState *pSwn, int n){
     
//TODO: Convertir en Debouncing en una máquina de estado con Switch!

if(_Swn(n)== 0){
    if((*pSwn).LastState == RELEASED){
        if(--pSwn->Ticks < 1){
            pSwn->LastState = PRESSED; 
            pSwn->StateChange = 1;
            pSwn->Ticks = DebounceTicks; 
    }   }
    else if ((*pSwn).LastState == PRESSED){
        if(--pSwn->TicksHold < 1){
            pSwn->LastState = HOLD; 
            pSwn->StateChange = 1;
            pSwn->TicksHold = DebounceTicksHold; 
    }   }
}
else if ((*pSwn).LastState == PRESSED || (*pSwn).LastState == HOLD){
    if(--pSwn->Ticks){
        pSwn->LastState = RELEASED; 
        pSwn->StateChange = 1;
        pSwn->Ticks = DebounceTicks;
     }
}
else{
    pSwn->Ticks = DebounceTicks;
    pSwn->TicksHold = DebounceTicksHold;
 
} 

return;

}

void Sw_Init(void){
    
    Sw1.LastState = RELEASED;
    Sw1.StateChange = 0;
    Sw1.Ticks = DebounceTicks;
    Sw1.TicksHold = DebounceTicksHold;
            
    Sw2.LastState = RELEASED;
    Sw2.StateChange = 0;
    Sw2.Ticks = DebounceTicks;
    Sw2.TicksHold = DebounceTicksHold;
    
    Sw3.LastState = RELEASED;
    Sw3.StateChange = 0;
    Sw3.Ticks = DebounceTicks;
    Sw3.TicksHold = DebounceTicksHold;
    
    Sw4.LastState = RELEASED;
    Sw4.StateChange = 0;
    Sw4.Ticks = DebounceTicks;
    Sw4.TicksHold = DebounceTicksHold;
}