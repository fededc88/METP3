

#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "xc.h"
#include "UART.h"




#define DebounceTicks 20  // 1 Tick = 1 mS
#define DebounceTicksHold 1500 // 2,5 S
 

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