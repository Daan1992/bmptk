#include "bmptk.h"
#include <iostream>

// create the uart class
// the default baudrate is the download baudrate defined by the makefile
typedef bmptk::target::uart<> uart;

bmptk::context *context_one, *context_two;

void one(){
   std::cout << "2. context one\n";
   context_two->resume();
   std::cout << "4. context one\n";
}

void two(){
   std::cout << "3. context two\n";
   context_one->resume();
   std::cout << "6. context two\n";
}

void show_use( bmptk::context *s, const char *name ){
   std::cout << "context " << name << ": " << *s << "\n";
}

void show_contexts(){
   show_use( &bmptk::main_context, "main" );
   show_use( context_one, "one" );
   show_use( context_two, "two" );
}

void show_heap(){
   std::cout << "heap free: " << bmptk::heap_free() << "\n";
}

int main( void ){	      
   std::cout.use( uart::channel() );
   std::cout << "context example\n";
   show_heap();
   
   context_one = new bmptk::context( one, 100 );
   context_two = new bmptk::context( two, 100 );
   show_contexts();
   std::cout << "1. main\n";
   context_one->resume();
   std::cout << "5. main\n";
   context_two->resume();
   std::cout << "7. main\n";
   context_one->resume();
   context_two->resume();
   context_two->resume();
   std::cout << "8. main\n";
   
   show_contexts();
   show_heap();
      
   // do not terminate
   for(;;);
}