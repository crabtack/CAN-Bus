/* 
* SPI.h
*
* Created: 11.06.2015 18:13:18
* Author: okozießa
*/


#ifndef __SPI_H__
#define __SPI_H__

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

class SPI
{
//variables
public:
protected:
private:

//functions
public:
	
	static void		Init();
	static uint8_t	Transmit(uint8_t data);
	
	static void CsLow();
	static void CsHigh();
	
	SPI();
	~SPI();
protected:
private:
	SPI( const SPI &c );
	SPI& operator=( const SPI &c );

}; //SPI

#endif //__SPI_H__
