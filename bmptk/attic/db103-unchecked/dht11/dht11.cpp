// basic blink-a-led example
//
// The bmptk::target:: provides the pin_out, 
// the bmptk::hardware:: provides a blink.
// Just instantiate the blink with the pin_out and let it run.

#include "bmptk.h" 

template< typename pin >
struct pin_oc {
   static void init(){ pin::init(); }
   static void set( bool x ){
      if(x){
	     pin::direction_set_input();
	  } else {
	     pin::direction_set_output();
	     pin::set(0);
	  }
   }
   static bool get(){ return pin::get(); }
};

template< typename pinx > 
struct dht11 { 
   typedef pin_oc< pinx > pin;
   
   static void init(){
      pin::init();
      pin::set(1);
   }
     
   static bool is_present(){  
      // wait for a quiet line
      pin::set( 1 );
      bmptk::wait( 50 * bmptk::ms );
      
      // check for idle
      if( ! pin::get() ){ return 0; }
      
      // give start pulse
      pin::set( 0 );
      bmptk::wait( 30 * bmptk::ms );
      pin::set( 1 );
      
      // wait for the line to go high
      bmptk::wait( 10 * bmptk::us );
      if( ! pin::get() ){ return 0; }
      
      // wait for the line to go get low
      bmptk::wait( 40 * bmptk::us );
      if( pin::get() ){ return 0; }
      
      return 1;
   }

};

typedef dht11< bmptk::target::gpio_1_9_in_out > sensor;
	
int main( void ){	
   for(;;){
      bmptk::wait( 500 * bmptk::ms );
      (void)sensor::is_present();
   }
}
