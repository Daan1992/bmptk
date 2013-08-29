#ifndef _BMPTK_LPC1114FN28_H_
#define _BMPTK_LPC1114FN28_H_

#include "bmptk.h"

namespace bmptk {

   class output_pin_lpc1: public bmptk::output_pin {
   private:
      volatile int *set_pointer;
      volatile int *clr_pointer;
      int pin;
   
      static volatile int *gpioreg( unsigned int port, unsigned int offset = 0 ){
         return (volatile int *)( 0x50000000 + port * 0x10000 + offset );
      }	
   
   public: 
      output_pin_lpc1( unsigned int port, unsigned int pin ): pin(pin) { 
    	   *gpioreg( port, 0x020 ) |= 1 << pin;	 
   	     set_pointer = gpioreg( port, 0x0C );   	  
   	     clr_pointer = gpioreg( port, 0x10 );   	  
      }
   
      void set( bool x ){ 
         if(x){ 
            *set_pointer = 1 << pin;
         } else {            
            *clr_pointer = 1 << pin;
         }            
      }
      
      void direction_set_output(){
      }
   };

   class lpc1114 {
   public:
      output_pin_lpc1 pin( unsigned int port, unsigned int pin ){
         return output_pin_lpc1( port, pin );
      }
   };	

   class chip {
   public:
      static output_pin_lpc1 pin( unsigned int port, unsigned int pin ){
         return output_pin_lpc1( port, pin );
      }
   };	
   
   void uart_init( unsigned int u32BaudRate );   
   void uart_send( char c );   

};

#endif // #ifdef _BMPTK_LPC1114FN28_H_ 
