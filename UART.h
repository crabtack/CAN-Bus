/* 
* UART.h
*
* Created: 11.06.2015 22:14:07
* Author: okozießa
*/


#ifndef __UART_H__
#define __UART_H__
#include "mcp2515.h"

class UART
{
//variables
public:
protected:
private:

//functions
public:
	static void  Init();
	static void Transmit(uint8_t data);
	
	UART();
	~UART();
protected:
private:
	UART( const UART &c );
	UART& operator=( const UART &c );

}; //UART

#endif //__UART_H__
