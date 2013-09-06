#include "bmptk.h"
#include "LPC11xx.h"
#include <iostream>
  
/*   
01	#include <msp430x20x2.h>
02	#include "USI_I2CMaster.h"
03	 
04	int StatusCallback(unsigned char c)
05	{
06	return TI_USI_EXIT_LPM; // Exit active for next transfer
07	}
08	 
09	void main( void )
10	{
11	// Stop watchdog timer to prevent time out reset
12	WDTCTL = WDTPW + WDTHOLD;
13	 
14	BCSCTL1 = CALBC1_16MHZ; // Set range
15	DCOCTL = CALDCO_16MHZ; // SMCLK = DCO = 1MHz
16	 
17	/* Initialize USI module, clock ~ SMCLK/128 
18	TI_USI_I2C_MasterInit(USIDIV_7+USISSEL_2+USICKPL, StatusCallback);
19	 
20	unsigned char TxDataConfig[5] = {0x2E,0xBA,0x10,0x10,0xC0};
21	 
22	/*Write data, configuration register
23	TI_USI_I2CWrite(SlaveAddress, Length, Multi, TxData) 
24	__disable_interrupt();
25	TI_USI_I2CWrite(0x60, 5, 0,TxDataConfig);
26	__bis_SR_register(LPM0_bits + GIE);
27	 
28	TI_USI_I2CStop();
29	 

*/

      class tea5767 {
      private:
      
         //! private assignment, prevents assigning
         void operator=( tea5767 & rhs );
         
         bmptk::i2c_master &bus;
         
         unsigned char settings[ 5 ];
         
      public:
         //! construct 
         tea5767( 
            bmptk::i2c_master &bus
         ): 
            bus( bus )
         {
            settings[ 0 ] = 0x40;
            settings[ 1 ] = 0x00;
            settings[ 2 ] = 0xF0;
            settings[ 3 ] = 0x10;
            settings[ 4 ] = 0x00;
    
if(1){            
    settings[0] =0x2A;
    settings[1] =0xB6;
    settings[2] =0x41;
    settings[3] =0x11;
    settings[4] =0x40; 
}            
         }         
         
         void init(){
            bus.write( 0x60, settings, 5 );
         }
         
         unsigned int pll_get(){
            unsigned char d[ 5 ];
            bus.read( 0x60, d, 5 );
            return (( d[0] & 0x3F ) << 8 ) + d[ 1 ];
         }
            
         void search_up(){
            unsigned int pll = pll_get();
            std::cout << "PLL=" << pll << "\n";
            pll += 100;
            settings[0] = 0x40 + ( pll >> 8 );
            settings[1] = pll & 0xFF;
            init(); 
         }

      };   

int main( void ){	

   // create an initialized uart object
   bmptk::input_output_uart uart( 38400 );
   
   // attach it to cout
   bmptk::cout_attach( uart );
   
   std::cout << "\nTEA5767 FM radio\n";

	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     //enable IOCON
	LPC_IOCON->R_PIO1_0 |= 0x01;
	LPC_IOCON->R_PIO1_1 |= 0x01;
	LPC_IOCON->R_PIO1_2 |= 0x01;
	LPC_IOCON->SWDIO_PIO1_3 |= 0x01;
	
	auto scl  = bmptk::chip::pin( 0, 4 );
	auto sda  = bmptk::chip::pin( 0, 5 );  
   
   bmptk::i2c_master bus( scl, sda );
   tea5767 radio( bus );
   radio.init();
   
   for(;;){
      bmptk::wait( 5000 * bmptk::ms );
      radio.search_up();
   }
   
}