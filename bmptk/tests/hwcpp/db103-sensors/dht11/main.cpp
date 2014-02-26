#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

namespace hwcpp {

   template< class _pin, class timing, bool debug = false >
   struct dht11 {
   
      HARDWARE_REQUIRE_ARCHETYPE( timing, timing );   
   
      typedef pin_oc_from< _pin > pin;
      static constexpr auto timeout = timing::duration::ms( 1 );
      
      static void init(){
         pin::init();
         pin::set( 1 );
         timing::init();
      }
           
      static bool data_get( int data[ 5 ] ){
      
         // activation
         pin::set( 0 );
         // 400 ms seems to work, 500 ms seems to be too long
         timing:: template ms< 25, 5 >::wait();
         pin::set( 1 );
         timing:: template us< 60, 5 >::wait();
         
         // detect presence
         if( pin::get() ){
            if( debug ){ TRACE << "no presence pulse\n"; }
            return false;
         }
         
         // await end of presence pulse
         auto start = timing::now();
         while( ! pin::get() ){
            if( timing::now() - start > timeout ){
               if( debug ){ TRACE << "presence pulse does not end\n"; }
               return false;
            }              
         }
         
         // read all 5 data bytes
         for( int i = 0; i < 5; i++ ){
         
            for( int j = 0; j < 8; j++ ){
            
               data[ i ] = ( data[ i ] << 1 ) & 0xFF;   
               
               // wait for low
               start = timing::now();
               while( pin::get() ){
                  if( timing::now() - start > timeout ){
                     if( debug ){ TRACE << "high took too long\n"; }
                     return false;
                  }   
               }
         
               // wait for high
               start = timing::now();
               while( ! pin::get() ){
                  if( timing::now() - start > timeout ){
                     if( debug ){ TRACE << "low took too long\n"; }
                     return false;
                  }   
               }

               // measure high
               start = timing::now();
               while( pin::get() ){
                  if( timing::now() - start > timeout ){
                     TRACE;
                     return false;
                  }   
               }
               auto width = timing::now() - start;
                      
               if( width > timing::duration::us( 50 ) ){
                  data[ i ] |= 0x01;
               }
            }
         }   
         
         // check the checksum
         int checksum = 0xFF & ( 
            data[ 0 ] + data[ 1 ] + data[ 2 ] + data[ 3 ] );
         if( checksum != data[ 4 ] ){
            if( debug ){
               TRACE << std::hex 
                  << data[ 0 ] << ":" << data[ 1 ] 
                  << ":" << data[ 2 ] << ":" << data[ 3 ]
                  << ":" << data[ 4 ] << " expected " << checksum;
               return false;
            }   
         } 
         
         return true;
      }
   
      static bool temperature_get( int & temperature ){
         int data[ 5 ];
         if( ! data_get( data ) ){ return false; }
         temperature = 1000 * data[ 2 ] + data[ 3 ];
         return true;
      }
   
      static bool humidity_get( int & humidity ){
         int data[ 5 ];
         if( ! data_get( data ) ){ return false; }
         humidity = 1000 * data[ 0 ] + data[ 1 ];
         return true;
      }
   
      static bool temperature_humidity_get( int & temperature, int & humidity ){
         int data[ 5 ];
         if( ! data_get( data ) ){ return false; }
         temperature = 1000 * data[ 2 ] + data[ 3 ];
         humidity = 1000 * data[ 0 ] + data[ 1 ];
         return true;
      }
      
   }; // struct dht11      
   
};

typedef hwcpp::dht11< target::gpio_1_5, timing, false > dht11;

int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "Read a DHT11 temperature & humidity sensor\n";
   timing::init();
   for(;;){
      timing::s< 2, 1 >::wait();
      int t,h;
      if( dht11::temperature_humidity_get( t, h )){
         std::cout << std::dec << std::setw( 2 )
            << "t=" << t / 1000 << "C"
            << "  "
            << "h=" << h / 1000 << "%"
            << "\n";
      } else {
         TRACE << "reading failed";
      }
   }
}

