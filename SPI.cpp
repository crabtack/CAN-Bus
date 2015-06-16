/* 
* SPI.cpp
*
* Created: 11.06.2015 18:13:18
* Author: okozießa
*/


#include "SPI.h"

// default constructor
SPI::SPI()
{
} //SPI

// default destructor
SPI::~SPI()
{
} //~SPI

void SPI::Init()

{
	DDRB |= (1 << PB7) | (1 << PB5) | (1 << PB4); //MOSI, SCK und CS Ausgang
	DDRB &= ~(1 << PB6); //MISO Eingang
	PORTB &= 0x0F; //SPI Pins auf low
	
		//SPE = 1 (SPI enable); DORD = 0 (MSB first); MSTR = 1 (Master mode); CPOL = 0 (SCK low Idle); CPHA (Data sampling bei 2. flanke); 
	SPCR |= (1 << SPE) | (1 <<MSTR);
	SPSR |= (1 << SPI2X); //fosc /2 (8MHz)
	
	SPI::CsHigh();
	
}

uint8_t SPI::Transmit(uint8_t data)
{
	SPDR = data;
	
	while(!(SPSR & (1 << SPIF)));
	
	return SPDR;
}

void SPI::CsHigh()
{
	PORTB |= (1 << PB4);
}

void SPI::CsLow()
{
	PORTB &= ~(1 << PB4);
}