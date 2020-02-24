/* 
 * File:   PWM_Extern.h
 * Author: Fededc88
 *
 * Created on 15 de febrero de 2020, 11:27
 */

#ifndef PWM_EXTERN_H
#define	PWM_EXTERN_H

#ifdef	__cplusplus
extern "C" {
#endif

extern int Sin[LEN_SIN];
extern int sin_step;
extern int step_delta;

#ifdef DEBUG1
extern float step_array[LEN_SIN];
extern int   OC1R_array[LEN_SIN];
extern char f_debug_sin; //Flag inidicates buffer is ready. For debuging level 1
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_EXTERN_H */

