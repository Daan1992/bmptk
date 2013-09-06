#include "bmptk.h"
#include "LPC11xx.h"
#include <iostream>

int main( void ){	

	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     //enable IOCON
	LPC_IOCON->R_PIO1_0 |= 0x01;
	LPC_IOCON->R_PIO1_1 |= 0x01;
	LPC_IOCON->R_PIO1_2 |= 0x01;
	LPC_IOCON->SWDIO_PIO1_3 |= 0x01;

   bmptk::uart_init( 38400 );
   bmptk::cout_putc = bmptk::uart_send;
	
	auto trigger  = bmptk::chip::pin( 1, 0 );
	auto echo     = bmptk::chip::pin( 1, 1 );  
	
	bmptk::sr04 sr04( trigger, echo );
   
   std::cout << "\nSR04 ultrasonic sensor\n";
   for(;;){
      bmptk::wait( 200 * bmptk::ms );
      std::cout << sr04.get_distance() << " mm\n";
   }      
   
}