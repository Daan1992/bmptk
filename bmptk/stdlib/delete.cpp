#include "bmptk.h"

void operator delete( void * ){
   bmptk::fatal_error_detected( "delete() called" );
}