//#include "bmptk.h"
#include "LPC11xx.h"
#include "uart-hw.h"

// UART line status register (LSR) bit definitions 
const unsigned int LSR_RDR   = 0x01;
const unsigned int LSR_OE    = 0x02;
const unsigned int LSR_PE    = 0x04;
const unsigned int LSR_FE    = 0x08;
const unsigned int LSR_BI    = 0x10;
const unsigned int LSR_THRE  = 0x20;
const unsigned int LSR_TEMT  = 0x40;
const unsigned int LSR_RXFE  = 0x80;

//! set the UART baudrate to baud
void uart_baudrate_set( unsigned int baud ){
   uint32_t Fdiv;
   LPC_SYSCON->UARTCLKDIV     = 0x1;     // divided by 1 
   Fdiv = ((BMPTK_XTAL/LPC_SYSCON->SYSAHBCLKDIV)/16)/baud;
   LPC_UART->DLM = Fdiv / 256;
   LPC_UART->DLL = Fdiv % 256;    
}
	
//! initialize the UART	
void uart_init( void ){

      
   // Not using interrupts
   NVIC_DisableIRQ(UART_IRQn);
         
   // enable IO config
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     //enable IOCON  
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);     //enable UART 
       
   // UART I/O config
   LPC_IOCON->PIO1_6 &= ~0x07;
   LPC_IOCON->PIO1_6 |= 0x01;     // UART RXD
   LPC_IOCON->PIO1_7 &= ~0x07;
   LPC_IOCON->PIO1_7 |= 0x01;     // UART TXD 
       
   // Enable UART clock 
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
       
   LPC_UART->LCR = 0x83;             // 8 bits, no Parity, 1 Stop bit 
   uart_baudrate_set( BMPTK_BAUDRATE );
   LPC_UART->LCR = 0x03;   // DLAB = 0 
   LPC_UART->FCR = 0x07;   // Enable and reset TX and RX FIFO. 
       
   // Read to clear the line status. 
   (void)LPC_UART->LSR;
       
   // Ensure a clean start, no data in either TX or RX FIFO. 
   while ( (LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
   while ( LPC_UART->LSR & LSR_RDR ){
      (void)LPC_UART->RBR; // Dump data from RX FIFO 
   } 
}

unsigned char uart_write_wont_block(){
   return ( LPC_UART->LSR & ( 0x01 << 6 )) != 0;
}

void uart_write_char( char c ){
   while(( LPC_UART->LSR & ( 0x01 << 6 )) == 0 );
   LPC_UART->THR = c;
}

unsigned char uart_read_wont_block(){
   return ( LPC_UART->LSR & ( 0x01 << 0 )) != 0;
}

char uart_read_char(){
   while(( LPC_UART->LSR & ( 0x01 << 0 )) == 0 );
   return LPC_UART->RBR;
}