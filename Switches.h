

#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>
#include "xc.h"


#include "UART.h"




#define DebounceTicks 20  // 1 Tick = 1 mS
#define DebounceTicksHold 1500 // 2,5 S


 

void Pin_Init(void);

typedef enum{ IDLE, PRESSED, HOLD, RELEASED } _KeyState;

typedef struct 
{
    _Bool StateChange;
    _KeyState KeyState;
 } SWnState; 
 
_Bool _Swn(int);

 void debouncingSwn(SWnState *,int);

