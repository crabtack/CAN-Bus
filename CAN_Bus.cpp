/*
 * CAN_Bus.cpp
 *
 * Created: 11.06.2015 18:11:41
 *  Author: okozießa
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "UART.h"

mcp2515::CanMessage message1;

volatile mcp2515::CanMessage receivedMessage;

void InterruptInit();

int main(void)
{
	
	message1.ID = 0x0123;
	message1.rtr = 0;
	message1.lenght = 2;
	message1.data[0] = 'o';
	message1.data[1] = 'k';
		
	
	SPI::Init();
	mcp2515::Init(0x02);//Loopback mode
	lcd_init(LCD_DISP_ON);
	UART::Init();
	lcd_clrscr();
	lcd_puts("Init Fertig");
	_delay_ms(1000);
	
	InterruptInit();
	sei();
	
	mcp2515::SendMessage(&message1);
	
    while(1)
    {
		
        
    }
}

void InterruptInit()
{
	MCUCR |= (1 << ISC01); //Fallende Flanke an INT0
	GICR  |= (1 << INT0);
	 
}

ISR(INT0_vect)
{
	mcp2515::GetMessage(&receivedMessage);
	lcd_clrscr();
	lcd_putc(receivedMessage.data[0]);
	lcd_putc(receivedMessage.data[1]);
	lcd_putc(' ');
	lcd_putc(receivedMessage.lenght + 48); //Länge ausgeben
	
	UART::Transmit((uint8_t)(receivedMessage.ID >> 8));
	UART::Transmit((uint8_t)(receivedMessage.ID & 0x00FF));
	
	UART::Transmit(receivedMessage.lenght);
	UART::Transmit(receivedMessage.data[0]);
	UART::Transmit(receivedMessage.data[1]);
}
