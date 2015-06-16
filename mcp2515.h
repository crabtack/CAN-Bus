/* 
* mcp2515.h
*
* Created: 11.06.2015 18:33:56
* Author: okozießa
*/


#ifndef __MCP2515_H__
#define __MCP2515_H__

#include "SPI.h"
#include "mcp2515_defs.h"

class mcp2515
{
//variables
public:
typedef struct
{
uint16_t ID;
uint8_t rtr;
uint8_t lenght;
uint8_t data[8];	
}CanMessage;


protected:
private:

//functions
public:
	
	static void		WriteRegister(uint8_t address, uint8_t data);
	static uint8_t  ReadRegister(uint8_t address);
	static void		BitModify(uint8_t address, uint8_t mask, uint8_t data);
	
	static void Init(uint8_t mode);// 0Normal, 1Sleep, 2Loopback, 3ListenOnly, 4Configuration
	static void SendMessage(CanMessage *msg);
	static uint8_t ReadRxStatus();
	static uint8_t GetMessage(volatile CanMessage *msg);

	mcp2515();
	~mcp2515();
protected:
private:
	mcp2515( const mcp2515 &c );
	mcp2515& operator=( const mcp2515 &c );

}; //mcp2515

#endif //__MCP2515_H__
