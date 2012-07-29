#include "bmptk.h"

#include <nds.h>
#include <nds/touch.h>

using namespace bmptk;
using namespace graphics;

target_screen lcd;

int position_is_in_rectangle( int x, int y, int x0, int y0, int x1, int y1 ){
   return( x > x0 && x < x1 && y > y0 && y < y1 );
}

typedef struct area_struct area;
typedef void ( *action )( int x, int y, area *a );
struct area_struct {
   area *next;
   int x0, y0, x1, y1;
   action f;
};

area *first = NULL;

void area_add( int x0, int y0, int x1, int y1, color fill, action f ){
   area *p = (area*) malloc( sizeof( area ));
   p->x0 = x0;
   p->x1 = x1;
   p->y0 = y0;
   p->y1 = y1;
   p->f = f;
   lcd.draw( vector( x0, y0 ), rectangle( vector( x1, y1 ) - vector( x0, y0 ), color::black, fill )); 
   p->next = first;
   first = p;
}

color active_color = color::red;

void execute( int x, int y ){
   area *p;
   for( p = first; p!= NULL; p = p->next ){
      if( position_is_in_rectangle( x, y, p->x0, p->y0, p->x1, p->y1 )){
         p->f( x, y, p );
      }   
   }
}

void paint( int x, int y, area *a ){
   lcd.write( vector( x, y ), active_color );
}

void select_red( int x, int y, area *a ){
   active_color = color::red;
}

void select_green( int x, int y, area *a ){
   active_color = color::green;
}

void select_blue( int x, int y, area *a ){
   active_color = color::blue;
}

int main( void ){
   struct touchPosition position;
   lcd.clear( color::yellow );
   area_add(  10, 10, 200, 180, color::white,  paint );
   area_add( 210, 10, 220,  20, color::red,    select_red );
   area_add( 210, 25, 220,  35, color::green,  select_green );
   area_add( 210, 40, 220,  50, color::blue,   select_blue );
   for(;;){
      scanKeys();
      int held = keysHeld();
      if( held & KEY_TOUCH ){
         touchRead( & position );
         execute( position.px, position.py );
      }
      swiWaitForVBlank();
   }
   return 0;
}

