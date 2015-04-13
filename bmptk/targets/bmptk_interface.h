#ifndef _BMPTK_INTERFACE_H_
#define _BMPTK_INTERFACE_H_

#include <stddef.h>

#ifdef BMPTK_HEAP
   void * malloc( size_t n );
   void free( void * );
#endif

#endif // #ifdef _BMPTK_INTERFACE_H_ 
