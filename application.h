/* 
 * File:   application.h
 * Author: Fededc88
 *
 * Created on 21 de febrero de 2020, 19:24
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

#ifdef	__cplusplus
extern "C" {
#endif

//Cuadrator State machine states.
#define DOWN 1
#define UP   0
    
//Valor medio, en muestras, del buffer del ADC    
#define SIN_CERO    515 //En muestras
    
#define HISTERESIS  1   //En muestras

    
void app_proces(void);
int cuadrador(int *pVal, int histeresis, int DCO_STATE );
void Cuadrator_Pin_Init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APPLICATION_H */

