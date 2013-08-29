#include "bmptk.h"

#include "LPC122x.h"

   // return the number of microseconds since startup 
   // (or rather, first call of this function) 
   // using the SysTick timer, running from the 12 MHz clock
unsigned long long int bmptk::time_since_startup(){
   static unsigned long long int last_low = 0;
   static unsigned long long int high = 0;
   
   static bool init = true;
   if( init ){
      init = false;
	   
      SysTick->CTRL  = 0;         // stop the timer
      SysTick->LOAD  = 0xFFFFFF;  // use its as a 24-bit timer
      SysTick->VAL   = 0;         // clear the timer
      SysTick->CTRL  = 1;         // start the timer
   }  

   // the timer ticks down, but we want an up counter
   unsigned int low = 0xFFFFFF - ( SysTick->VAL & 0xFFFFFF );
   if( low < last_low ){
   
      // the timer rolled over, so increment the high part
      high += 0x1ULL << 24;
   }
   last_low = low;

   // the timer ticks at 12 Mhz, so divide by 12
   return low | high;
}

/* UART line status register (LSR) bit definitions */
#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define SystemCoreClock ( 12 * 1000 * 1000 )

void bmptk::uart_init( unsigned int u32BaudRate ){
	uint32_t Fdiv;
	uint32_t regVal;

	/* Not using interrupts */
	// NVIC_DisableIRQ(UART_IRQn);
	
	// enable IO config
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     //enable IOCON	
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);     //enable IOCON	

	/* UART I/O config */
	LPC_IOCON->PIO0_1 = 2;  /* UART RXD */
	LPC_IOCON->PIO0_2 = 2;  /* UART TXD */

	/* Enable UART clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
	// LPC_SYSCON->UARTCLKDIV = 0x1;     /* divided by 1 */

	LPC_UART0->LCR = 0x83;             /* 8 bits, no Parity, 1 Stop bit */
	// regVal = LPC_SYSCON->UARTCLKDIV;
	Fdiv = (((SystemCoreClock/LPC_SYSCON->SYSAHBCLKDIV)/regVal)/16)/u32BaudRate ;	/*baud rate */

	LPC_UART0->DLM = Fdiv / 256;

	LPC_UART0->DLL = Fdiv % 256;
	LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
	LPC_UART0->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

	/* Read to clear the line status. */
	regVal = LPC_UART0->LSR;

	/* Ensure a clean start, no data in either TX or RX FIFO. */
	while ( (LPC_UART0->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
	while ( LPC_UART0->LSR & LSR_RDR )
	{
		regVal = LPC_UART0->RBR;	/* Dump data from RX FIFO */
	} 
	
}

void bmptk::uart_send( char c ){
   while(( LPC_UART0->LSR & ( 1 << 6 )) == 0 );
   LPC_UART0->THR = c;
}
