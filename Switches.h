

#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "xc.h"
#include "UART.h"




#define DebounceTicks 20  // 1 Tick = 1 mS
#define DebounceTicksHold 1500 // 2,5 S
 
#define _Sw1 Sw1.LastState 
#define _Sw2 Sw2.LastState 
#define _Sw3 Sw3.LastState 
#define _Sw4 Sw4.LastState

typedef enum{
    IDLE = 0,
    PRESSED,
    HOLD,
    RELEASED
}_KeyState;

typedef struct 
{
    _Bool StateChange;
    _KeyState LastState;
    int Ticks;
    int TicksHold;
    
 } SWnState; 
 
 //Public Functions Declarations
 void Sw_app(void);
 _Bool _Swn(int);
 void debouncingSwn(SWnState *,int);
 void Sw_Pin_Init(void);
 void Sw_Init(void);

#endif