/*
    \file   main.c

    \brief  Main file of the project.

    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#define USART0_BAUD_RATE(BAUD_RATE) ((float)(3333333.33333 / (2 * (float)BAUD_RATE)) + 0.5)
#define USART1_BAUD_RATE(BAUD_RATE) ((float)(3333333.33333 / (2 * (float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <avr/interrupt.h>

volatile char x;

void USART0_syncInit(void);
void USART1_syncInit(void);
void USART0_syncWrite(char data);
void USART0_syncWrite(char data);

ISR(USART0_RXC_vect)
{
    x = USART0.RXDATAL;
    /* Set a breakpoint on the NOP below to check the value of x in the debugger */
    asm("NOP");
}

ISR(USART1_RXC_vect)
{
    x = USART1.RXDATAL;
    /* Set a breakpoint on the NOP below to check the value of x in the debugger */
    asm("NOP");
}

void USART0_syncInit(void)
{
    PORTA.DIR &= ~PIN1_bm;
    PORTA.DIR |= PIN0_bm;
    PORTA.DIR |= PIN2_bm;
    
    USART0.CTRLA |= USART_RXCIE_bm;  
    USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;  
    USART0.CTRLC |= USART_CMODE_SYNCHRONOUS_gc;
}

void USART1_syncInit(void)
{
    PORTC.DIR &= ~PIN1_bm;
    PORTC.DIR |= PIN0_bm;
    PORTC.DIR &= ~PIN2_bm;
    
    USART1.CTRLA |= USART_RXCIE_bm; 
    USART1.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;    
    USART1.CTRLC |= USART_CMODE_SYNCHRONOUS_gc; 
}

void USART0_syncWrite(char data)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
    {
        ;
    }
    USART0.TXDATAL = data;
}

void USART1_syncWrite(char data)
{
    while (!(USART1.STATUS & USART_DREIF_bm))
    {
        ;
    }        
    USART1.TXDATAL = data;
}

int main(void)
{   
    USART0_syncInit();
    USART1_syncInit();
    
    /* Enable global interrupts */
    sei();
    
    while (1) 
    {
        USART0_syncWrite('A');
        USART1_syncWrite('B');
    }
}
