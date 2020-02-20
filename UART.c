/*
 * File:   UART.c
 * Author: Fededc88
 *
 * Created on 2 de agosto de 2018, 20:26
 */

#include <xc.h>
#include "UART.h"

#include <stdlib.h>
#include <stdio.h>
#include "HardwareDef.h"


/******************************************************************************
 * Function: UART1_Init(int BaudRate);
 * 
 * Input: Int Baudrate -> Define BaudRate, 0 to 9600 as default
 * Output: none
 * 
 * Description: Init UART 1 module.  9600 8n1 
 *              Assign U1RX To Pin RP13
 *              Assign U1TX To Pin RP14
 ******************************************************************************/

void UART1_Init(int BaudRate){
    
 /*-------------UART 1 - Remapeo de Pines ------------------------------------*/
    
__builtin_write_OSCCONL(OSCCON & 0xBF); // Unlock Registers

    // Configure Input Functions (Table 10-2))
    
    RPINR18bits.U1RXR = 13; // Assign U1RX To Pin RP13
    
    // Configure Output Functions (Table 10-3)

    RPOR7bits.RP14R = 3 ; // Assign U1TX To Pin RP14

__builtin_write_OSCCONL(OSCCON | 0x40);     // Lock Registers

 /*-------------UART 1 - FIN Remapeo de Pines --------------------------------*/



AD1PCFGbits.PCFG10 = 1; //Pin for corresponding analog channel is configured in Digital mode; I/O port read is enabled
TRISBbits.TRISB14 = 0;

AD1PCFGbits.PCFG11 = 1; //Pin for corresponding analog channel is configured in Digital mode; I/O port read is enabled
TRISBbits.TRISB13 = 1; 


    
    //Set UART TX Interrupt Priority
        IPC3bits.U1TXIP &= 0b101;
       
    //Set UART RX Interrupt Priority    
        IPC2bits.U1RXIP &= 0b101; 
    
        
/*---------------------- Setting up the UART1---------------------------------*/
    
    U1MODEbits.UARTEN = 0;
    
    U1MODEbits.BRGH = 0; //BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
    U1MODEbits.PDSEL = 0b00; //8-bit data, no parity
    U1MODEbits.STSEL = 0; //One Stop bit
    
    
//Set Baud Rate
    if(!BaudRate)
        U1BRG = (int) ((Fcy/16/9600)-1); //U1BRG = 416
    else
        U1BRG = (int) ((Fcy/16/BaudRate)-1); //U1BRG = 416
        
  U1MODEbits.UARTEN = 1; //enable module UART1
  U1STAbits.UTXEN = 1; //Enable Transmit
  
 
  IFS0bits.U1TXIF = 0; //Clean UART1RX interrupt Flag
  IEC0bits.U1TXIE = 1; //Enable Transmit Interrupt  
  
  IFS0bits.U1RXIF = 0; //Clean UART1RX interrupt Flag
  IEC0bits.U1RXIE = 1; //Enable Receive Interrupt
  


  U1TXREG;
 // U1TXREG = (char) 'b';
  
 }


/******************************************************************************
 * Function: SendStringPolling(char* str);
 * 
 * Input: char* str -> Pointer to String to be send
 * Output: none
 * 
 * Description: Send string Throug UART1 until find NUL character
 * 
 * ATTENTION!! Function blocks program! 
 ******************************************************************************/

void SendStringPolling(char* str) {
    
  while((*str)!='\0') {
      if(U1STAbits.UTXBF != 1){
          U1TXREG = *str;
          str++;
        }
    }
  return;
}

/******************************************************************************
 * Function: SendStringPolling(char* str);
 * 
 * Input: int Entero -> Integer Value
 * Output: none
 * 
 * Description: Send Integer Throug UART1
 * 
 * ATTENTION!! Function blocks program! 
 ******************************************************************************/

void SendIntPolling(int Entero){
    
    char buff[6];
    
    sprintf(buff, "%d", Entero );
    SendStringPolling(buff);
    
    return;
}

