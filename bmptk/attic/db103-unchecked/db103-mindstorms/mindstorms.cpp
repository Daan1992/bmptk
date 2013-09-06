#include "bmptk.h"
#include "LPC11xx.h"
#include <iostream>
   
class mindstorms_master {
private:
   bmptk::i2c_bus &bus;
public:
   mindstorms_master( bmptk::i2c_bus &bus ): bus( bus ){}
   
   void get( unsigned char a, unsigned char address, unsigned char n, char *data ){
   
      // const unsigned char a = 0xA2;
      // bus.put_bit( 1 );
   
      // write the start address from which we want to read
      bus.put_start();
      bus.put_byte( a );
      bool ack = bus.get_ack();
      std::cout << "ack = " << ack << "\n";
      bus.put_byte( address );
      (void) bus.get_ack();
      bus.put_stop();
      
      bmptk::wait( 10 * bmptk::ms );
      
      bus.put_bit( 1 );
      
      // read the n bytes
      bus.put_start();
      bus.put_byte( a + 1 );
      for( int i = 0; i < n; i++ ){
         bus.get_ack();
         *data++ = bus.get_byte();  
      }
      bus.put_nack();
      bus.put_stop();
   }   
};
		
int main( void ){	

   bmptk::uart_init( 38400 );
   bmptk::cout_putc = bmptk::uart_send;
   std::cout << "\nMindstorms I2C sensor readout\n";

	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     //enable IOCON
	LPC_IOCON->R_PIO1_0 |= 0x01;
	LPC_IOCON->R_PIO1_1 |= 0x01;
	LPC_IOCON->R_PIO1_2 |= 0x01;
	LPC_IOCON->SWDIO_PIO1_3 |= 0x01;
	
	auto scl  = bmptk::chip::pin( 0, 4 );
	auto sda  = bmptk::chip::pin( 0, 5 );  
   
   bmptk::i2c_bus bus( scl, sda );
   // bmptk::i2c_bus bus( sda, scl );
   mindstorms_master ms( bus );
   char version[ 20 ];
   // ms.get( 0, 8, version );
   if(0) for( int a = 0; a < 256; a += 2 ){
      ms.get( a, 0, 8, version );   
      std::cout << a << "\n";
      if( version[ 0 ] != 0xFF ){
         std::cout << version << "\n";
         for( int i = 0; i < 8; i++ ){
            std::cout << i << "      [" << std::hex << (int) version[ i ] << "]\n";
         }
      }
   }
   ms.get( 2, 8, 8, version );   
   std::cout << version << "\n";
   for( int i = 0; i < 8; i++ ){
      std::cout << i << "[" << std::hex << (int) version[ i ] << "]\n";
   }
   for(;;);
   
}