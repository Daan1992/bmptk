#include "bmptk.h"

#ifdef BMPTK_EMBEDDED_HEAP

   extern unsigned char __heap_start;
   extern unsigned char __heap_end;
   extern unsigned char __stack_start;
   extern unsigned char __stack_end;
   extern unsigned char __stack_size;

#endif

namespace bmptk {

// there is also one in the startup!
const unsigned int unused_memory_marker = 0xDEADBEEF;

// helper to call a function and then keep calling another
extern "C" void _trampoline();

// helper for the trampoline: when a context returns, resume main
void resume_main(){
   bmptk::main_context.resume();
}

// used only for the main context
constexpr context::context( 
   unsigned char *bottom,  
   unsigned char *size 
): bottom( bottom ),sp( 0 ), stack_size( (unsigned int) size ){}
   
// construct the main context from the facts
context main_context( &__stack_start, &__stack_size );   

// create a new context
context::context( void f(), unsigned int size ) : 

   // round size up to a multiple of 8
   stack_size( ( size + 7 ) &~ 7 )
{

   // allocate the rounded-up amount of memory
   bottom = (unsigned char *) malloc( stack_size );
 
   // write 0xDEAD all over the allocated memory
   unsigned int *p = (unsigned int *) bottom;
   for( unsigned int i = 0; i < size; i+= sizeof( unsigned int ) ){
      *p++ = unused_memory_marker;
   }
   
   // full-descending stack: the initial sp is one beyond the actual stack
   sp = bottom + stack_size;
   
   // argument 1 for the trampoline: to call when f returns
   sp -= 4;   
   *(unsigned int *) sp = 0x01 | (unsigned int) resume_main;
   
   // argument 2 for the trampoline: the function to call
   sp -= 4;
   *(unsigned int *) sp = 0x01 | (unsigned int) f;
   
   // make sure the trampoline is called
   sp -= 4;
   *(unsigned int *) sp = 0x01 | (unsigned int) _trampoline;
    
   // room for the 10 stacked registers that will be 'restored' 
   sp -= ( 10 * sizeof( unsigned char * ));
}

unsigned int context::stack_unused() const {

   // specific for a full-descending stack
   
   // start at the original bottom of the stack
   unsigned int *p = (unsigned int *) bottom;
   
   // step through the stack space from low to high,
   // counting the number of 0xDEADBEEF words
   unsigned int unused = 0;
   while( ( unused < stack_size ) && ( *p++ == unused_memory_marker )){
      unused += 4;
   }
   
   //! return the number of 0xDEADBEEF words found (*4)
   return unused;
}

std::ostream & operator<< ( std::ostream &s, const bmptk::context &st ){
   std::cout
      << "obj=0x" << std::hex << (unsigned int) &st
      << " bot=0x" << std::hex << (unsigned int) st.bottom
      << " sp=0x"  << std::hex << (unsigned int) st.sp
      << " used " << std::dec << st.stack_used()
      << " of " << st.stack_size;
   return s;      
}

// interface to the assembly code that does the real context switching
extern "C" void _resume( unsigned char ** old_sp, unsigned char ** new_sp );

// initially the current context is of course the main context
context *current_context_ptr = &main_context;

// resume the context
void context::resume(){

   context *old_context = current_context_ptr;
   current_context_ptr = this;
   _resume( &old_context->sp, &sp );
      
   // Setting the current_context_ptr can NOT be
   // done after the _resume, because we are on a different stack now.
   // The very first time a context is resumed we are 'returning'
   // from the fake stack frame created by the context constructor,
   // which doed not include the 'this' pointer.
}

// return the current context
context * current_context(){
   return current_context_ptr;
}

};