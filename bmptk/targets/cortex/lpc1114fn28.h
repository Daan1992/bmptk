#ifndef _BMPTK_LPC1114FN28_H_
#define _BMPTK_LPC1114FN28_H_

#include "bmptk.h"
#include "LPC11xx.h"
// #include <iostream>

namespace bmptk {

//! an LPC1114fn28 chip
class target {
private:

   //! return the pointer to an IO port related register
   static constexpr volatile int *gpioreg( int port, int offset ){
      return (volatile int *)( 0x50000000 + port * 0x10000 + offset );
   }
   
   //========================================================================
   //
   // configure a pin as GPIO
   //
   //========================================================================
   
   static void pin_gp_conf( volatile uint32_t *p, int value ){
      *p = value | ( *p & ~0x07 );
   }
   
   static void pin_ad_conf( volatile uint32_t *p, int value ){
      *p = value | ( 0x01 << 7 ) | ( *p & ~0x07 );
   }
   
   template< int port, int pin >
   static void configure_as_gpio(){
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     // enable IOCON
      switch( ( port << 8 ) | pin ){
         case 0x0000: pin_gp_conf( &LPC_IOCON->RESET_PIO0_0,  0x01 ); break;
         case 0x0001: pin_gp_conf( &LPC_IOCON->PIO0_1,        0x00 ); break;
         case 0x0002: pin_gp_conf( &LPC_IOCON->PIO0_2,        0x00 ); break;
         case 0x0003: pin_gp_conf( &LPC_IOCON->PIO0_3,        0x00 ); break;
         case 0x0004: pin_gp_conf( &LPC_IOCON->PIO0_4,        0x00 ); break;
         case 0x0005: pin_gp_conf( &LPC_IOCON->PIO0_5,        0x00 ); break;
         case 0x0006: pin_gp_conf( &LPC_IOCON->PIO0_6,        0x00 ); break;
         case 0x0007: pin_gp_conf( &LPC_IOCON->PIO0_7,        0x00 ); break;
         case 0x0008: pin_gp_conf( &LPC_IOCON->PIO0_8,        0x00 ); break;
         case 0x0009: pin_gp_conf( &LPC_IOCON->PIO0_9,        0x00 ); break;
         case 0x000A: pin_gp_conf( &LPC_IOCON->SWCLK_PIO0_10, 0x01 ); break;
         case 0x000B: pin_ad_conf( &LPC_IOCON->R_PIO0_11,     0x01 ); break;
         
         case 0x0100: pin_ad_conf( &LPC_IOCON->R_PIO1_0,      0x01 ); break;
         case 0x0101: pin_ad_conf( &LPC_IOCON->R_PIO1_1,      0x01 ); break;
         case 0x0102: pin_ad_conf( &LPC_IOCON->R_PIO1_2,      0x01 ); break;
         case 0x0103: pin_ad_conf( &LPC_IOCON->SWDIO_PIO1_3,  0x01 ); break;
         case 0x0104: pin_ad_conf( &LPC_IOCON->PIO1_4,        0x00 ); break;
         case 0x0105: pin_gp_conf( &LPC_IOCON->PIO1_5,        0x00 ); break;
         case 0x0106: pin_gp_conf( &LPC_IOCON->PIO1_6,        0x00 ); break;
         case 0x0107: pin_gp_conf( &LPC_IOCON->PIO1_7,        0x00 ); break;
         case 0x0108: pin_gp_conf( &LPC_IOCON->PIO1_8,        0x00 ); break;
         case 0x0109: pin_gp_conf( &LPC_IOCON->PIO1_9,        0x00 ); break;
         default: break;
      }          
   }   
   
   //========================================================================
   //
   // configure (enable or disable) the internal pullup
   //
   //========================================================================
   
   static void pu_conf( volatile uint32_t *p, bool enable ){
      if( enable ){
         *p = ( 0x02 << 3 ) | ( *p & ~ ( 0x03 << 3 ));
      } else {
         *p = ( 0x00 << 3 ) | ( *p & ~ ( 0x03 << 3 ));
      }      
   }   
   
   template< int port, int pin, bool enable >
   static void pullup_configure(){
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     // enable IOCON
      switch( ( port << 8 ) | pin ){
         case 0x0000: pu_conf( &LPC_IOCON->RESET_PIO0_0,  enable ); break;
         case 0x0001: pu_conf( &LPC_IOCON->PIO0_1,        enable ); break;
         case 0x0002: pu_conf( &LPC_IOCON->PIO0_2,        enable ); break;
         case 0x0003: pu_conf( &LPC_IOCON->PIO0_3,        enable ); break;
         case 0x0004: pu_conf( &LPC_IOCON->PIO0_4,        enable ); break;
         case 0x0005: pu_conf( &LPC_IOCON->PIO0_5,        enable ); break;
         case 0x0006: pu_conf( &LPC_IOCON->PIO0_6,        enable ); break;
         case 0x0007: pu_conf( &LPC_IOCON->PIO0_7,        enable ); break;
         case 0x0008: pu_conf( &LPC_IOCON->PIO0_8,        enable ); break;
         case 0x0009: pu_conf( &LPC_IOCON->PIO0_9,        enable ); break;
         case 0x000A: pu_conf( &LPC_IOCON->SWCLK_PIO0_10, enable ); break;
         case 0x000B: pu_conf( &LPC_IOCON->R_PIO0_11,     enable ); break;
         
         case 0x0100: pu_conf( &LPC_IOCON->R_PIO1_0,      enable ); break;
         case 0x0101: pu_conf( &LPC_IOCON->R_PIO1_1,      enable ); break;
         case 0x0102: pu_conf( &LPC_IOCON->R_PIO1_2,      enable ); break;
         case 0x0103: pu_conf( &LPC_IOCON->SWDIO_PIO1_3,  enable ); break;
         case 0x0104: pu_conf( &LPC_IOCON->PIO1_4,        enable ); break;
         case 0x0105: pu_conf( &LPC_IOCON->PIO1_5,        enable ); break;
         case 0x0106: pu_conf( &LPC_IOCON->PIO1_6,        enable ); break;
         case 0x0107: pu_conf( &LPC_IOCON->PIO1_7,        enable ); break;
         case 0x0108: pu_conf( &LPC_IOCON->PIO1_8,        enable ); break;
         case 0x0109: pu_conf( &LPC_IOCON->PIO1_9,        enable ); break;
         default: break;
      }          
   }   
   
   //========================================================================
   //
   // A/D conversion
   //
   //========================================================================
   
   static void ad_conf( volatile uint32_t *p, int value ){
      *p = 
         ( value << 0 ) 
         | ( *p & ~ ( ( 0x07 << 0 ) | ( 0x01 << 7 )));
   }   
   
   template< int port, int pin, int channel >
   static void pin_ad_configure(){
   
         // enable IOCON & A/D clock
      LPC_SYSCON->SYSAHBCLKCTRL |= ( 0x01 << 16 ) | ( 0x01 << 13 );
   
         // enable A/D power
      LPC_SYSCON->PDRUNCFG &= ~( 1 << 4 );      

      switch( ( port << 8 ) | pin ){
         case 0x000B: ad_conf( &LPC_IOCON->R_PIO0_11,    0x02 ); break;
         
         case 0x0100: ad_conf( &LPC_IOCON->R_PIO1_0,     0x02 ); break;
         case 0x0101: ad_conf( &LPC_IOCON->R_PIO1_1,     0x02 ); break;
         case 0x0102: ad_conf( &LPC_IOCON->R_PIO1_2,     0x02 ); break;
         case 0x0103: ad_conf( &LPC_IOCON->SWDIO_PIO1_3, 0x02 ); break;
         case 0x0104: ad_conf( &LPC_IOCON->PIO1_4,       0x01 ); break;
         default: break;
      }          
   }   
   
   template< int channel >
   static unsigned int pin_ad_read(){
      
         // configure the A/D for the pin
      LPC_ADC->CR = ( 1 << channel ) | ( 12 << 8 );
     
         // start the conversion
      LPC_ADC->CR |= ( 1 << 24 );
   
         // wait for the conversion to complete
      while( ( LPC_ADC->GDR & ( 1 << 31 )) == 0 );
     
         // return the A/D result
      return ( LPC_ADC->GDR >> 6 ) & 0x3FF;   
   }
   
   //========================================================================
   //
   // GPIO pin operations
   //
   //========================================================================
   
   static void direction_set_output( int port, int pin ){
      *gpioreg( port, 0x8000 ) |= ( 0x01 << pin );
   }
   
   static void direction_set_input( int port, int pin ){
      *gpioreg( port, 0x8000 ) &= ~ ( 0x01 << pin );
   }   
       
   template< int port, int pin >  
   static void set( bool x ){
      *gpioreg( port, 0x04 << pin ) = x ? -1 : 0;  
   }    

   template< int port, int pin >  
   static bool get(){
      return *gpioreg( port, 0x3FFC ) & ( 0x01 << pin );  
   }
   
   //========================================================================
   //
   // an open collector pin (no pullup or A/D )
   //
   //========================================================================
   
   template< int port, int pin >
   struct pin_oc : public bmptk::hardware::pin_oc {
      
      static void init(){
         target::configure_as_gpio< port, pin >();
         target::direction_set_output( port, pin );
      }
   
      static void set( bool x ){
	     if( x ){
            target::direction_set_input( port, pin );
		 } else {
            target::direction_set_output( port, pin );
            target::set< port, pin >( 0 );
         }			
      }
	  
      static bool get(){
         return target::get< port, pin >();
      }   	  
   };
            
   //========================================================================
   //
   // an input-output pin with configurable pullup
   //
   //========================================================================
   
   template< int port, int pin >
   struct pin_in_out_pullup : 
      public bmptk::hardware::pin_in_out,
      public bmptk::hardware::pin_configurable_pullup 
   {
         
      //! combine the features
      enum { features = 
         bmptk::hardware::pin_in_out::features 
         | bmptk::hardware::pin_configurable_pullup::features };
      
      static void init(){
         target::configure_as_gpio< port, pin >();
      } 
    
      static void direction_set_input(){
         target::direction_set_input( port, pin );
      }
    
      static void direction_set_output(){
         target::direction_set_output( port, pin );
      }
      
      static void set( bool x ){
         target::set< port, pin >( x );
      }     
      
      static bool get(){
         return target::get< port, pin >();
      }       
      
      //! enable the pullup
      static void pullup_enable(){
         target::pullup_configure< port, pin, 1 >();
      }
   
      //! disable the pullup
      static void pullup_disable(){
         target::pullup_configure< port, pin, 0 >();
       }
   };

   //========================================================================
   //
   // an input-output pin with configurable pullup and A/D
   //
   //========================================================================
   
   template< int port, int pin, int channel >
   struct pin_in_out_ad_pullup : 
      public pin_in_out_pullup< port, pin >, 
      public bmptk::hardware::pin_ad
   {
   
      //! combine the features
      enum { features = 
         pin_in_out_pullup< port, pin >::features 
         | bmptk::hardware::pin_ad::features };
         
      //! @copydoc bmptk::hardware::pin_ad::ad_init
      static void ad_init(){
         pin_ad_configure< port, pin, channel >();
      }
   
      //! @copydoc bmptk::hardware::pin_ad::ad_get
      static unsigned int ad_get(){
         return pin_ad_read< channel >();
      }
   
      //! @copydoc bmptk::hardware::pin_ad::ad_bits
      static unsigned int ad_bits(){
         return 10;
      }
   
      //! @copydoc bmptk::hardware::pin_ad::ad_max
      static unsigned int ad_max(){
         return 0x3FF;
      }
   };
   
public: 

   //! static uart class
   //
   //! The optional parameter is the initial baudrate.
   //! The default is the download baudrate.
   template< unsigned int baudrate = BMPTK_BAUDRATE>
   class uart {
   
      // UART line status register (LSR) bit definitions 
      static const unsigned int LSR_RDR   = 0x01;
      static const unsigned int LSR_OE    = 0x02;
      static const unsigned int LSR_PE    = 0x04;
      static const unsigned int LSR_FE    = 0x08;
      static const unsigned int LSR_BI    = 0x10;
      static const unsigned int LSR_THRE  = 0x20;
      static const unsigned int LSR_TEMT  = 0x40;
      static const unsigned int LSR_RXFE  = 0x80;

      class channel_class : public bmptk::output_channel_polled< char >{
      public:
         bool put_will_not_block(){ return uart::put_will_not_block(); }
         void put( char c ){ uart::put( c ); }
      };
      
   public:
   
      //! set the baudrate to baud
      static void baudrate_set( unsigned int baud ){
         uint32_t Fdiv;
         LPC_SYSCON->UARTCLKDIV     = 0x1;     // divided by 1 
         Fdiv = ((BMPTK_XTAL/LPC_SYSCON->SYSAHBCLKDIV)/16)/baud;
         LPC_UART->DLM = Fdiv / 256;
         LPC_UART->DLL = Fdiv % 256;    
      }
   
      //! initialize the uart
      static void init(){
       
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
         baudrate_set( baudrate );
         LPC_UART->LCR = 0x03;   // DLAB = 0 
         LPC_UART->FCR = 0x07;   // Enable and reset TX and RX FIFO. 
       
         // Read to clear the line status. 
         (void)LPC_UART->LSR;
       
         // Ensure a clean start, no data in either TX or RX FIFO. 
         while ( (LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
         while ( LPC_UART->LSR & LSR_RDR )
         {
           (void)LPC_UART->RBR; // Dump data from RX FIFO 
         } 
      }
           
      //! report if the uart is ready to accpet a char
      static bool put_will_not_block(){
         return ( LPC_UART->LSR & ( 1 << 6 )) != 0;
      }
      
	  //! put a char
	  //
	  //! When the uart is no ready to accept a char this call will
	  //! block untill it is.
      static void put( char c ){
         while(( LPC_UART->LSR & ( 1 << 6 )) == 0 );
         LPC_UART->THR = c;
      }
      
	  //! report whether the uart is ready to accept a char
      static bool get_will_not_block(){
         return 0;
      }
      
	  //! return a received char
	  //
	  //! When the uart has no char available this call will block
	  //! until it has.
      static char get(){
         return ' ';
      }   
      
	  //! return the channel that represents the chip's uart
      static channel_class & channel(){
         static channel_class ch;
         static bool init_done = false;
         if( ! init_done ){
            init();
            init_done = true;
         }   
         return ch;
      }
   };
   
   //========================================================================
   //
   // the actual pins of the chip
   //
   //========================================================================
        
   //! GPIO 0.0 : input/output with configurable pullup
   //
   //! After reset this pin is configured as reset input.
   //! When it is configured as I/O pin the chip can only be 
   //! forced to reset by a power cycle.
   //!
   //! At reset the pullup is enabled, hence a an external
   //! reset switch does not need a pull-up resistor.
   typedef pin_in_out_pullup< 0,  0 > gpio_0_0;
   
   //! GPIO 0.1 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   //!
   //! When this pin is pulled low at reset (or power-up) 
   //! the chip will enter bootload mode.
   //! The pin has a pullup that is enabled by default, so 
   //! when this pin is left unconnected the chip will
   //! not enter bootload mode (but start running the downloaded
   //! application, if any).
   typedef pin_in_out_pullup< 0,  1 > gpio_0_1;

   //! GPIO 0.2 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 0,  2 > gpio_0_2;

   //! GPIO 0.3 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 0,  3 > gpio_0_3;

   //! GPIO 0.4 : open-collector input/output
   //
   //! After reset this pin is configured as input.
   //! Note that this pin has no internal pullup.
   typedef pin_oc<     0,  4 > gpio_0_4;

   //! GPIO 0.5 : open-collector input/output
   //
   //! After reset this pin is configured as input.
   //! Note that this pin has no internal pullup.
   typedef pin_oc<     0,  5 > gpio_0_5;

   //! GPIO 0.6 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 0,  6 > gpio_0_6;

   //! GPIO 0.7 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 0,  7 > gpio_0_7;

   //! GPIO 0.8 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 0,  8 > gpio_0_8;

   //! GPIO 0.9 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 0,  9 > gpio_0_9;

   //! GPIO 0.10 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 0, 10 > gpio_0_10;

   //! GPIO 0.11 : input/output with configurable pullup and A/D
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_ad_pullup< 0, 11, 0 > gpio_0_11;
   
   //! GPIO 1.0 : input/output with configurable pullup and A/D
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_ad_pullup< 1,  0, 1 > gpio_1_0;

   //! GPIO 1.1 : input/output with configurable pullup and A/D
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_ad_pullup< 1,  1, 2 > gpio_1_1;

   //! GPIO 1.2 : input/output with configurable pullup and A/D
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_ad_pullup< 1,  2, 3 > gpio_1_2;

   //! GPIO 1.3 : input/output with configurable pullup and A/D
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_ad_pullup< 1,  3, 4 > gpio_1_3;

   //! GPIO 1.4 : input/output with configurable pullup and A/D
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_ad_pullup< 1,  4, 5 > gpio_1_4;

   //! GPIO 1.5 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.   
   typedef pin_in_out_pullup< 1,  5 > gpio_1_5;

   //! GPIO 1.6 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   //!
   //! In bootload mode (and when the hadrware UART is used)
   //! this pin serves as RxD (receive data ).
   typedef pin_in_out_pullup< 1,  6 > gpio_1_6;

   //! GPIO 1.7 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   //!
   //! In bootload mode (and when the hadrware UART is used)
  //! this pin serves as TxD (transmit data ).
   typedef pin_in_out_pullup< 1,  7 > gpio_1_7;

   //! GPIO 1.8 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 1,  8 > gpio_1_8;

   //! GPIO 1.9 : input/output with configurable pullup
   //
   //! After reset this pin is configured as input,
   //! with the pull-up enabled.
   typedef pin_in_out_pullup< 1,  9 > gpio_1_9;
   
};
    
/*	
   void uart_init( unsigned int u32BaudRate );   
   void uart_send( char c );   
   bool uart_send_empty();
   
   class input_output_uart : public bmptk::input_output_channel_polled< char > {
   public: 
      input_output_uart( unsigned int baudrate );
      bool put_ready();
      void put( char c );
      bool get_ready();
      char get();      
   };
*/

};




#endif // #ifdef _BMPTK_LPC1114FN28_H_ 
