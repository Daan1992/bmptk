#include <math.h>

// constexpr 
bool f( int x ){
   return sin( x ) > 0.5;
}

int main(){	         
   volatile bool x = f( 12 );
   (void) x;
}
