#include "bmptk.h"

   void configure_as_gpio( int pin ){
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);      // enable GPIO
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);      // enable switchmatrix

      switch( pin ){
         case 0x0000: LPC_SWM->PINENABLE0 |= ( 0x01 << 0 ); break;
         case 0x0001: LPC_SWM->PINENABLE0 |= ( 0x01 << 1 ) | ( 0x01 << 7 ); break;
         case 0x0002: LPC_SWM->PINENABLE0 |= ( 0x01 << 3 ); break;
         case 0x0003: LPC_SWM->PINENABLE0 |= ( 0x01 << 2 ); break;
         case 0x0004: break;
         case 0x0005: LPC_SWM->PINENABLE0 |= ( 0x01 << 6 ); break;
      }           
   }   

   volatile int *gpioreg( int offset ){
      return (volatile int *)( 0xA0001000 + 0 * 0x10000 + offset );
   }

void pin_configure_as_input( int pin ){
   configure_as_gpio( pin );
   *gpioreg( 0x1000 ) &= ~ ( 0x01 << pin );  
}

void pin_configure_as_output( int pin ){
   configure_as_gpio( pin );
   *gpioreg( 0x1000 ) |= ( 0x01 << pin );
}

void pin_set( int pin, unsigned char x ){
   *gpioreg( 0x04 * pin ) = x ? -1 : 0;  
}

unsigned char pin_get( int pin ){
   return LPC_GPIO_PORT->PIN0 & ( 0x01 << pin ); 
}

void wait_us( int n ){
   while( n-- > 0 ){
      asm( "nop" );
      asm( "nop" );
      asm( "nop" );
   }
}
