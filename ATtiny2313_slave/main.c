#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "base64.h"

#define UART_BAUD 38400
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

void USART_Init( uint16_t ubrr);;
void Usart_Transmit(unsigned char data);
void Send_clause(char * napis);
unsigned char USART_Receive( void );
void int_to_char (int l, char* tab);
void Read_enc ();
void Find_Zero();

volatile char Address = '1';

volatile int licznik = 0;

volatile uint8_t chapter = 0;
volatile uint8_t ZeroAll = 0;

int main(void)
{
	USART_Init(__UBRR);

	DDRB |= (1<<PB0); 	// Transceiver
	PORTB &= ~(1<<PB0); // PB0 = 1 -> nadawanie

	DDRB |= (1<<PB4) | (1<<PB3); 	// Debug LED

	DDRD &= ~(1<<PD2); // Wejscie enkodera (caly obrot)
	PORTD &= ~(1<<PD2);

	DDRD &= ~(1<<PD3);  // kana³ A enkodera
	DDRD &= ~(1<<PD4);  // kana³ B enkodera
	PORTD |= (1<<PD3);
	PORTD |= (1<<PD4);

	MCUCR = (1<<ISC11) | (1<<ISC10) | (1<<ISC00) | (1<<ISC01);
	chapter = 0;
	Find_Zero();
	_delay_ms(500);

	PORTB &= ~(1<<PB3);
	PORTB &= ~(1<<PB4);

	licznik = 0;

	while(1)
	{
		Read_enc();
	} // end while
} // end main

void USART_Init( unsigned int ubrr)
{
		UBRRH = (unsigned char)(ubrr>>8);
		UBRRL = (unsigned char)ubrr;
		UCSRB = (1<<TXEN) | (1<<RXEN);
}

void Usart_Transmit(unsigned char data)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
	}

unsigned char USART_Receive( void )
{
/* Wait for data to be received */
while ( !(UCSRA & (1<<RXC)))
;
/* Get and return received data from buffer */
return UDR;
}

void Send_clause(char * napis)
{
	while(*napis)
		Usart_Transmit(*napis++);
}


ISR(INT1_vect) // obsluga przerwania zewnetrznego
{
	if(licznik > 499)
		licznik = 0;

	if(licznik == -1)
		licznik = 499;

	if(PIND & (1<<PD4))
		{
		if(licznik >= 0)
			licznik--;
		}
		else
		{
			if(licznik <= 500)
				licznik++;
		}
}

ISR (INT0_vect)
{
	if(chapter == 0)
	{
		PORTB |= (1<<PB4);
		_delay_ms(10);
		PORTB |= (1<<PB0);
		_delay_ms(25);

		Send_clause("\n0Z\t");

		_delay_ms(10);
		PORTB &= ~(1<<PB0);
		licznik = 0;

		GIMSK = (1<<INT1);

		chapter = 1;
	}
}


void int_to_char (int l, char* tab)
{
	int i =0;
	char c_array[4];

	if(l==0)
	{
		tab[i++] = '0';
	}else
	{
		while (l != 0)
		{
		     int rem = l % 10;
		     c_array[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
		     l = l/10;
		}

		int p =0;
		for(int j = i-1; j>=0; j--)
		{
			tab[p]=c_array[j];
			p++;
		}

	}
	tab[i]='\0';
}


void Read_enc ()
{
	PORTB &= ~(1<<PB0);

	unsigned char pom;

	while(1)
	{
		unsigned char rec_char = USART_Receive();

		if(rec_char == 'E' && pom == '1')
		{
			PORTB |= (1<<PB4);
			ZeroAll = 0;
			break;
		}

		if(pom == 'Z' && rec_char == 'A')
		{
			ZeroAll = 1;
			break;
		}

		pom = rec_char;
	}
	/// Komunikat \n Address yyyy \t
	if(ZeroAll == 0)
	{
		PORTB |= (1<<PB0);

		char c_licznik[4];

		int_to_char(licznik, c_licznik);

		const unsigned int tab[3] = {c_licznik[0], c_licznik[1], c_licznik[2]};

		unsigned int size_a = NELEMS(tab);

		unsigned int out_size_a = b64e_size(size_a) + 1;

		unsigned char out_a[4];

		out_size_a = b64_encode((const unsigned int*)tab,size_a, out_a);

		_delay_ms(10);

		Send_clause(out_a);

		_delay_ms(5);

		Usart_Transmit('\t');

		_delay_ms(5);

		PORTB &= ~(1<<PB4);
	}

	if(ZeroAll == 1)
	{
		PORTB |= (1<<PB3);
		licznik = 0;
		ZeroAll = 0;
	}
}

void Find_Zero()
{
	unsigned char p;
	while(chapter == 0)
	{
		if(chapter != 0)
			break;

		unsigned char r = USART_Receive();

		if(r == 'S' && ((p == '1' && Address == '1') || (p == '2' && Address == '2') || (p == '3' && Address == '3') ))
		{
			GIMSK = (1<<INT0);
			sei();
			PORTB |= (1<<PB0);
			_delay_ms(10);

			Send_clause("\n0N\t");

			_delay_ms(10);
			PORTB &= ~(1<<PB0);
		}

		p = r;
	}
}

