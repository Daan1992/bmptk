#include "bmptk.h"

void * operator new( unsigned int n ){
   return malloc( n );
} 

void * operator new[]( unsigned int n ){
   return malloc( n );
}