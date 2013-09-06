#include "bmptk.h"
#include "LPC11xx.h"
#include <iostream>

void mcp23017_put( 
   bmptk::hardware::i2c_master &bus, 
   unsigned char a, 
   unsigned char d 
){
   bus.put_start();
   bus.put_byte( 0x4E );
   bus.get_ack();
   bus.put_byte( a );
   bus.get_ack();
   bus.put_byte( d );
   bus.get_ack();
   bus.put_stop();   
}
		
int main( void ){	
	
	auto scl  = bmptk::chip::pin( 0, 4 );
	auto sda  = bmptk::chip::pin( 0, 5 );  
   
   bmptk::i2c_master bus( scl, sda );
   mcp23017_put( bus, 0x01, 0x00 );
   
   for(;;){
      mcp23017_put( bus, 0x15, 0x00 );    
      bmptk::wait( 500 * bmptk::ms );
      mcp23017_put( bus, 0x15, 0xFF );    
      bmptk::wait( 500 * bmptk::ms );
   }         
}