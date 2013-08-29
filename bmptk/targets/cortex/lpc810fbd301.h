#ifndef _BMPTK_LPC1114FN28_H_
#define _BMPTK_LPC1114FN28_H_

#include "bmptk.h"
#include "LPC8xx.h"
// #include <iostream>

namespace bmptk {

class target {
private:

   static constexpr volatile int *gpioreg( int port, int offset ){
      return (volatile int *)( 0xA0001000 + port * 0x10000 + offset );
   }
     
   template< int port, int pin >
   static void configure_as_gpio(){
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);      // enable GPIO
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);      // enable switchmatrix
      switch( ( port << 8 ) | pin ){
         case 0x0000: LPC_SWM->PINENABLE0 |= ( 0x01 << 0 ); break;
         case 0x0001: LPC_SWM->PINENABLE0 |= ( 0x01 << 1 ) | ( 0x01 << 7 ); break;
         case 0x0002: LPC_SWM->PINENABLE0 |= ( 0x01 << 3 ); break;
         case 0x0003: LPC_SWM->PINENABLE0 |= ( 0x01 << 2 ); break;
         case 0x0004: break;
         case 0x0005: LPC_SWM->PINENABLE0 |= ( 0x01 << 6 ); break;
         default: break;
      }         
   }   
   
   static void direction_set_output( int port, int pin ){
      *gpioreg( port, 0x1000 ) |= ( 0x01 << pin );
   }
   
   static void direction_set_input( int port, int pin ){
      *gpioreg( port, 0x1000 ) &= ~ ( 0x01 << pin );
   }   
       
   template< int port, int pin >  
   static void set( bool x ){
      *gpioreg( port, 0x04 * pin ) = x ? -1 : 0;  
   }    

   template< int port, int pin >  
   static bool get(){
      return LPC_GPIO_PORT->PIN0 & ( 0x01 << pin );  
   }

   template< int port, int pin >
   class pin_out : public bmptk::hardware::pin_out {
   public:
   
      // check whether the port/pin combination is valid for this chip
      static_assert( (
            (( port == 0 ) && ( pin >= 0 ) && ( pin <= 0x0B ))
         || (( port == 1 ) && ( pin >= 0 ) && ( pin <= 0x09 ))
      ), "illegal port/pin combination for an LPC1114" ); 
      
      static void init(){
         target::configure_as_gpio< port, pin >();
         target::direction_set_output( port, pin );
      }
   
      static void set( bool x ){
         target::set< port, pin >( x );
      }
   };
      
   template< int port, int pin >
   class pin_in : public bmptk::hardware::pin_in {
   public:
   
      // check whether the port/pin combination is valid for this chip
      static_assert( (
            (( port == 0 ) && ( pin >= 0 ) && ( pin <= 0x0B ))
         || (( port == 1 ) && ( pin >= 0 ) && ( pin <= 0x09 ))
      ), "illegal port/pin combination for an LPC1114" ); 
      
      static void init(){
         target::configure_as_gpio< port, pin >();
         target::direction_set_input( port, pin );
      }
   
      static bool get(){
         return target::get< port, pin >();
      }
   };   
      
   template< int port, int pin >
   class pin_in_out : public bmptk::hardware::pin_in_out {
   public:
   
      // check whether the port/pin combination is valid for this chip
      static_assert( (
            (( port == 0 ) && ( pin >= 0 ) && ( pin <= 0x0B ))
         || (( port == 1 ) && ( pin >= 0 ) && ( pin <= 0x09 ))
      ), "illegal port/pin combination for an LPC1114" ); 
      
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
   };

   public: 

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
         bool put_ready(){ return send_empty(); }
         void put( char c ){ send( c ); }
      };
      
   public:
   
      static void baudrate_set( unsigned int baud ){
         uint32_t Fdiv;
         LPC_SYSCON->UARTCLKDIV     = 0x1;     // divided by 1 
         Fdiv = ((BMPTK_XTAL/LPC_SYSCON->SYSAHBCLKDIV)/16)/baud;
//         LPC_UART->DLM = Fdiv / 256;
//        LPC_UART->DLL = Fdiv % 256;    
      }
   
      static void init(){
       
         // Not using interrupts
//         NVIC_DisableIRQ(UART_IRQn);
         
         // enable IO config
         LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     //enable IOCON  
         LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);     //enable UART 
       
         // UART I/O config
//         LPC_IOCON->PIO1_6 &= ~0x07;
//         LPC_IOCON->PIO1_6 |= 0x01;     // UART RXD
//         LPC_IOCON->PIO1_7 &= ~0x07;
//         LPC_IOCON->PIO1_7 |= 0x01;     // UART TXD 
       
         // Enable UART clock 
         LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
       
/*
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
*/
      }

      static void send( char c ){
//         while(( LPC_UART0->LSR & ( 1 << 6 )) == 0 );
//         LPC_UART0->THR = c;
      }
      
      static bool send_empty(){
//         return ( LPC_UART0->LSR & ( 1 << 6 )) != 0;
      }
            
      static bool put_ready(){
//        return ( LPC_UART0->LSR & ( 1 << 6 )) != 0;
      }
      
      static void put( char c ){
//         while(( LPC_UART->LSR & ( 1 << 6 )) == 0 );
//         LPC_UART->THR = c;
      }
      
      static bool get_ready(){
         return 0;
      }
      
      static char get(){
         return ' ';
      }   
      
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
   
   typedef pin_out< 0,  0 > gpio_0_0_out;
   typedef pin_out< 0,  1 > gpio_0_1_out;
   typedef pin_out< 0,  2 > gpio_0_2_out;
   typedef pin_out< 0,  3 > gpio_0_3_out;
   typedef pin_out< 0,  4 > gpio_0_4_out;
   typedef pin_out< 0,  5 > gpio_0_5_out;
  
   typedef pin_in< 0,  0 > gpio_0_0_in;
   typedef pin_in< 0,  1 > gpio_0_1_in;
   typedef pin_in< 0,  2 > gpio_0_2_in;
   typedef pin_in< 0,  3 > gpio_0_3_in;
   typedef pin_in< 0,  4 > gpio_0_4_in;
   typedef pin_in< 0,  5 > gpio_0_5_in;

   typedef pin_in_out< 0,  0 > gpio_0_0_in_out;
   typedef pin_in_out< 0,  1 > gpio_0_1_in_out;
   typedef pin_in_out< 0,  2 > gpio_0_2_in_out;
   typedef pin_in_out< 0,  3 > gpio_0_3_in_out;
   typedef pin_in_out< 0,  4 > gpio_0_4_in_out;
   typedef pin_in_out< 0,  5 > gpio_0_5_in_out;
   
};
       
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


};




#endif // #ifdef _BMPTK_LPC1114FN28_H_ 
