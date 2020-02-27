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
    
#define HISTERESIS  -2   //En muestras
#define HIST_AN0
#define HIST_AN1
    
//Estados del Contador

#define IDLE        0
#define CONTANDO    1
#define VECES       2 
#define LATCH       3
#define RESET       4    
#define FF          5
    
void app_proces(void);
int cuadrador(volatile uint16_t *pVal, int histeresis, int DC_STATE );
void Cuadrator_Pin_Init(void);
void contador(int eNe_veces);
long calcular_angulo(long N_cuentas);

extern char buff[60];
#ifdef	__cplusplus
}
#endif

#endif	/* APPLICATION_H */

