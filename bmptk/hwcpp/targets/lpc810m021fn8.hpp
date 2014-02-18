#include "hardware.hpp"

// ==========================================================================
//
// LPC810M021FN8 target
//
// ==========================================================================

namespace hardware {

class target : public target_archetype< 1 * Kb, 4 * Kb, 12 * MHz > {
private:

   //! return the pointer to an IO port related register
   static constexpr volatile int *gpioreg( int port, int offset ){
      return (volatile int *)( 0xA0001000 + port * 0x10000 + offset );
   }
     
   //========================================================================
   //
   // configure a pin as GPIO
   //
   //========================================================================
   
   static void pin_gp_conf( volatile uint32_t *p, int value ){
      *p = value | ( *p & ~0x07 );
   }   
   
   template< int port, int pin >
   static void configure_as_gpio(){
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);      // enable GPIO
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);      // enable switchmatrix
      switch( ( port << 8 ) | pin ){
         case 0x0000: pin_gp_conf( &LPC_IOCON->RESET_PIO0_0,  0x01 ); break;
         case 0x0001: pin_gp_conf( &LPC_IOCON->PIO0_1,        0x00 ); break;
         case 0x0002: pin_gp_conf( &LPC_IOCON->PIO0_2,        0x00 ); break;
         case 0x0003: pin_gp_conf( &LPC_IOCON->PIO0_3,        0x00 ); break;
         case 0x0004: break;
         case 0x0005: pin_gp_conf( &LPC_IOCON->PIO0_5,        0x00 ); break;
         default: break;
      
      
         case 0x0000: LPC_SWM->PINENABLE0 |= ( 0x01 << 0 ); break;
         case 0x0001: LPC_SWM->PINENABLE0 |= ( 0x01 << 1 ) | ( 0x01 << 7 ); break;
         case 0x0002: LPC_SWM->PINENABLE0 |= ( 0x01 << 3 ); break;
         case 0x0003: LPC_SWM->PINENABLE0 |= ( 0x01 << 2 ); break;
         case 0x0004: break;
         case 0x0005: LPC_SWM->PINENABLE0 |= ( 0x01 << 6 ); break;
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
   
   typedef pin_out< 0,  0 > gpio_0_0_out;
   typedef pin_out< 0,  1 > gpio_0_1_out;
   typedef pin_out< 0,  2 > gpio_0_2_out;
   typedef pin_out< 0,  3 > gpio_0_3_out;
   typedef pin_out< 0,  4 > gpio_0_4_out;
   typedef pin_out< 0,  5 > gpio_0_5_out;
   
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
