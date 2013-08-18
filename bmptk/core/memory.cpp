#include "bmptk.h"

#ifdef BMPTK_EMBEDDED_HEAP

   extern unsigned char __heap_start;
   extern unsigned char __heap_end;

#endif

namespace bmptk {

#ifdef BMPTK_EMBEDDED_HEAP

   // the start of free memory (up till &__heap_end)
   unsigned char *heap_free_marker = &__heap_start;

   unsigned int heap_free(){
      return &__heap_end - heap_free_marker;
   }

   extern "C" void *malloc( unsigned int size ){
   
      // round size up to a multiple of 8
      size = ( size + 7 ) &~ 7;
       
      // save the current start of the free space
      unsigned char *start = heap_free_marker;
      
      // reserve the memory by advancing the heap_free_marker
      heap_free_marker += size;
      
      // out of memory?
      if( heap_free_marker > &__heap_end ){
         fatal_error_detected( "out of memory" );
      }

      // return the now reserved block of memory
      return start;
   } 

#else

   unsigned int heap_free(){
      return 10 * 1000 * 1000;
   }
   
#endif

}; // namespace bmptk