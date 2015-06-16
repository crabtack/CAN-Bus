/* 
* UART.cpp
*
* Created: 11.06.2015 22:14:07
* Author: okozießa
*/


#include "UART.h"

// default constructor
UART::UART()
{
} //UART

// default destructor
UART::~UART()
{
} //~UART

void UART::Init()
{
	UCSRB |= (1 << TXEN);
	UCSRC  = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	UBRRL = 103; //9600 Baud
	UBRRH = 0;
}

void UART::Transmit(uint8_t data)
{
	while(!(UCSRA & (1 << UDRE)));
	UDR = data;
}