
#include "Switches.h"

int Ticks[4] = {DebounceTicks,DebounceTicks,DebounceTicks,DebounceTicks};
int TicksHold[4] = {DebounceTicksHold};


SWnState Sw1 = {0,RELEASED}, Sw2 = {0,RELEASED}, Sw3 = {0,RELEASED}, Sw4 = {0,RELEASED};


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


void Pin_Init(void){
    
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
    
}


void debouncingSwn(SWnState *pSwn, int n){
     

if(_Swn(n)== 0){
    if((*pSwn).KeyState == RELEASED){
        if(--Ticks[n-1] < 1){
            (*pSwn).KeyState = PRESSED; 
            (*pSwn).StateChange = 1;
            Ticks[n-1] = DebounceTicks; 
    }   }
    else if ((*pSwn).KeyState == PRESSED){
        if(--TicksHold[n-1] < 1){
            (*pSwn).KeyState = HOLD; 
            (*pSwn).StateChange = 1;
            TicksHold[n-1] = DebounceTicksHold; 
    }   }
}
else if ((*pSwn).KeyState == PRESSED || (*pSwn).KeyState == HOLD){
    if(--Ticks[n-1]){
        (*pSwn).KeyState = RELEASED; 
        (*pSwn).StateChange = 1;
        Ticks[n-1] = DebounceTicks;
     }
}
else{
    Ticks[n-1] = DebounceTicks;
    TicksHold[n-1] = DebounceTicksHold;
 
}
 
}

