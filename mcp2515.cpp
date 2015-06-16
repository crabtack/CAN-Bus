/* 
* mcp2515.cpp
*
* Created: 11.06.2015 18:33:56
* Author: okozießa
*/


#include "mcp2515.h"

// default constructor
mcp2515::mcp2515()
{
} //mcp2515

// default destructor
mcp2515::~mcp2515()
{
} //~mcp2515

void mcp2515::WriteRegister(uint8_t address, uint8_t data)
{
	SPI::CsLow();
	
	SPI::Transmit(SPI_WRITE); //Kommando um Wert in Register zu schreiben
	SPI::Transmit(address);
	SPI::Transmit(data);
	
	SPI::CsHigh();
	
}

uint8_t mcp2515::ReadRegister(uint8_t address)
{
	uint8_t data;
	SPI::CsLow();
	
	SPI::Transmit(SPI_READ);
	SPI::Transmit(address);
	data = SPI::Transmit(0xFF);
	
	SPI::CsHigh();
	
	return data;
}

void mcp2515::BitModify(uint8_t address, uint8_t mask, uint8_t data)
{
	SPI::CsLow();
	
	SPI::Transmit(SPI_BIT_MODIFY);
	SPI::Transmit(address);
	SPI::Transmit(mask);
	SPI::Transmit(data);
	
	SPI::CsHigh();
	
}

void mcp2515::Init(uint8_t mode)
{
	SPI::CsLow();
	SPI::Transmit(SPI_RESET); //Reset für Konfigurationsmodus
	_delay_ms(1);
	SPI::CsHigh();
	
	_delay_ms(10);


	mcp2515::WriteRegister(CNF1, 0x03); //Timing einstellen, werte von Kampi geklaut. Hoffe auf 1Mbps
	mcp2515::WriteRegister(CNF2, 0xA0);
	mcp2515::WriteRegister(CNF3, 0x02);
	
	mcp2515::WriteRegister(CANINTE, (1<< RX0IE) | (1 << RX1IE)); //Interrupt, wenn Empfangsbuffer voll (An manuelles rücksetzen von CANINTF denken)
	
	mcp2515::WriteRegister(RXB0CTRL, (1 << RXM1) | (1 << RXM0)); //Alle nachrichten Empfangen, unabhängig von filter (Sowohl standard als auch extended)
	mcp2515::WriteRegister(RXB1CTRL, (1 << RXM1) | (1 << RXM0));
	
	mcp2515::WriteRegister(BFPCTRL, 0); //RXnBF Pins deaktivieren
	mcp2515::WriteRegister(TXRTSCTRL, 0); //TX Pins als Inputs schalten
	
	mcp2515::WriteRegister(TXB0CTRL, (1 << TXP1) | (1 << TXP0)); //Sendebuffer 0 Höchste Priorität
	
	mcp2515::BitModify(CANCTRL,0xE0,mode << 5); //Modus Auswählen
}


void mcp2515::SendMessage(CanMessage *msg)
{
		mcp2515::WriteRegister(TXB0SIDH, (uint8_t) (msg->ID >> 3)); //ID Eintragen (Standard ID)
		mcp2515::WriteRegister(TXB0SIDL, (uint8_t) (msg->ID << 5));
		
		mcp2515::WriteRegister(TXB0DLC, (msg->lenght & 0x0F) | msg->rtr << 6); //Länge und RTR einstellen
		
		
		SPI::CsLow();
			
		SPI::Transmit(SPI_WRITE); //Kommando um Wert in Register zu schreiben
		SPI::Transmit(TXB0D0);
		
		for (uint8_t i = 0; i < msg->lenght; i++)
		{
			SPI::Transmit(msg->data[i]);	
		}
		SPI::CsHigh();
		
		_delay_us(10);
		
		SPI::CsLow();
		SPI::Transmit(SPI_RTS | 0x01); //Sende Buffer 0
		SPI::CsHigh();
}

uint8_t mcp2515::ReadRxStatus()
{
	uint8_t data;
	
	SPI::CsLow();
	
	SPI::Transmit(SPI_RX_STATUS); //RX Register auswerten
	data = SPI::Transmit(0xFF);
	
	SPI::Transmit(0xFF); //Daten werden widerhohlt gesendet und verworfen.
	
	SPI::CsHigh();
	
	return data;
}

uint8_t mcp2515::GetMessage(volatile CanMessage * msg)
{
	uint8_t status = mcp2515::ReadRxStatus();
	
	if(status & (1 << 6)) //Message in RXB0
	{
		msg->ID = ((uint16_t)((uint16_t)mcp2515::ReadRegister(RXB0SIDH)<<3) | ((uint16_t)((uint16_t)mcp2515::ReadRegister(RXB0SIDL)>>5)  ));
		
		msg->rtr = ((mcp2515::ReadRegister(RXB0SIDL) & (1 << 4)) >> 4); //Könnte falsch sein, steckt vlt im DLC register
		msg->lenght = mcp2515::ReadRegister(RXB0DLC) & 0x0F;
		
		SPI::CsLow();
		SPI::Transmit(SPI_READ);
		SPI::Transmit(RXB0D0);
		
		for(int i = 0; i < msg->lenght; i++)
		{
			msg->data[i] = SPI::Transmit(0xFF);
		}
		SPI::CsHigh();
	}
	else if(status & (1 << 7)) //Message in RXB1
	{
			msg->ID = ((uint16_t)((uint16_t)mcp2515::ReadRegister(RXB1SIDH)<<3) | ((uint16_t)((uint16_t)mcp2515::ReadRegister(RXB1SIDL)>>5)  ));
			
			msg->rtr = ((mcp2515::ReadRegister(RXB1SIDL) & (1 << 4)) >> 4); //Könnte falsch sein, steckt vlt im DLC register
			msg->lenght = mcp2515::ReadRegister(RXB1DLC) & 0x0F;
			
			SPI::CsLow();
			SPI::Transmit(SPI_READ);
			SPI::Transmit(RXB1D0);
			
			for(int i = 0; i < msg->lenght; i++)
			{
				msg->data[i] = SPI::Transmit(0xFF);
			}
			SPI::CsHigh();
	}
	
	mcp2515::BitModify(CANINTF, 0x03, 0x00); //Interrupt Flags löschen
	
	return (status & 0x07); //gibt die Nummer des Durchlassenden Filters zurück
}