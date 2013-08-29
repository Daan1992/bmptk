//===========================================================================
// Cortex startup
// Wouter van Ooijen - wouter@voti.nl
// license: zlib
//===========================================================================

int main( void );
void fatal_error_detected( const char * );
void __startup( void ); 
void _hard_fault( void ); 
extern int __stack_end;			

// nr 7 (offset 0x1C) is the vectors checksum
void (* const __vectors[ 16 ])(void) 
   __attribute__ ((section(".vectors"))) = {
      (void (*)(void)) & __stack_end,	
		                     __startup,
                                   0,
                          _hard_fault
};

void _hard_fault( void ) {
   fatal_error_detected( "hard fault" );
}

void __startup( void ){
   extern int __data_init_start;
   extern int __stack_start;
   extern int __stack_end;
   extern int __data_start;
   extern int __data_end;
   extern int __bss_start;
   extern int __bss_end;
      
	 unsigned int *s, *d, *e;

	 // clear .bss section 
	 d = & __bss_start;
	 e = & __bss_end;
	 while( d != e ){
	    *d++ = 0;
	 }
	 
	 // copy .data section from flash to ram
	 s = & __data_init_start;
	 d = & __data_start;
	 e = & __data_end;
	 while( d != e ){ 
	    *d++ = *s++;
	 } 
    
	 // fill the stack
	 d = & __stack_start;
	 e = & __stack_end;
	 while( d != e ){
	    *d++ = 0xDEADBEEF;
	 }    
  
   // call main   
   (void) main(); 
   
   // when main returns: loop till hell freezes down
   while(1){}	
}

