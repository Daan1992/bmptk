#ifndef _uart_hw_
#define _uart_hw_

// document everything in this file
//! \file

//! initialize the UART
//
//! This call initializes the UART and sets it to the 
//! bmptk download baudrate.
void uart_init( void );

//! set UART baudrate
//
//! This call set UART baudrate to the specified value.
void uart_baudrate_set( unsigned int baud );

//! return true iff a uart_write_char will not block
//
//! This call returns true if (and only if) a next uart_write_call 
//! does not have
//! to wait until the UART is ready to receive a next character.
unsigned char uart_write_wont_block();

//! write a character to the UART to be transmitted
//
//! This call writes a character to the UARt to be transmitted.
//! If necessary, his call will wait until the UART is ready
//! to accept the character.
void uart_write_char( char c );

//! return true iff a uart_read_char will not block
//
//! This call returns true if (and only if) a next uart_read_call 
//! does not have
//! to wait until the UART receives a next character.
unsigned char uart_read_wont_block();

//! return a character received by the UART
//
//! This call waits until the UART has a received character
//! available, then returns that character. 
char uart_read_char( void );

#endif