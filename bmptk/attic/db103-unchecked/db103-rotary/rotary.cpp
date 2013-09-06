#include "bmptk.h"
#include "LPC11xx.h"
#include <iostream>

class rotary_encoder {
   private:
      bmptk::input_pin &pin1;
      bmptk::input_pin &pin2; 
      unsigned char last;
      
      unsigned char read(){
         unsigned char result = 0;
         if( pin1.get() ){ result |= 0x01; }
         if( pin2.get() ){ result |= 0x02; }
         return result;
      }
      
   public:   
   
      int counter;
      
      rotary_encoder( 
         bmptk::input_pin &pin1,
         bmptk::input_pin &pin2 
      ): 
         pin1( pin1 ), 
         pin2( pin2 ), 
         counter( 0 )
      {
         pin1.direction_set_input(),
         pin2.direction_set_input();
      }
         
      void poll(){
         unsigned char step = ( last << 4 ) | read();
         if( step == 0x01 ){ counter--; }
         if( step == 0x10 ){ counter++; }
         if( step == 0x23 ){ counter++; }
         if( step == 0x32 ){ counter--; }
         last = step & 0x0F;
         //std::cout << "rot=" << std::hex << (int) last << "\n";
         //std::cout << " " << pin1.get() << " " << pin2.get() << "\n";
         //bmptk::wait( 500 * bmptk::ms );
      }
};
 
int main( void ){	

   bmptk::input_output_uart uart( 38400 );
   bmptk::output_channel_polled_buffer< char, 100 > buffer( uart );
   bmptk::cout_attach( buffer );
   
   std::cout << "\nrotary encoder readout\n";

	auto rot1  = bmptk::chip::pin( 0, 4 );
	auto rot2  = bmptk::chip::pin( 0, 5 );  
	auto encoder = rotary_encoder( rot1, rot2 );
      
   int last = 10;
   for(;;){
      encoder.poll();
      buffer.poll();
      int next = encoder.counter;
      if( last != next ){
         std::cout << next << "\n";
         last = next;
      }
   }
   
}