// ==========================================================================
//                                                                           
// file: bmptk/stdlib/iostream     
//
// This file (and iostream) attempts to provide facilities of the 
// standard iostream in a ROM and RAM effective way.
//                                                                           
// ==========================================================================

#include "bmptk.h"
#include <iostream>

/*
namespace bmptk {
   output_channel_polled< char > *cout_output_channel = 0;
   void cout_attach( output_channel_polled< char > &output ){
      cout_output_channel = &output;
   }
}
*/
   
namespace std {
   
   bmptk::output_channel_polled< char > *ch = 0;
   bmptk_ostream_putc cout( &ch );
   
   void ostream :: digit( char c ){
      if( c > 9 ){
          c += ( hex_base - 10 );
      } else {
         c += '0';
      } 
      putc( c );
   }
   
   void ostream :: filler( int n ){
      while( n-- > 0 ){
         *this << fill_char;
      }
   }
   
   ostream & operator<< ( ostream & stream, const setw & x ){
      stream.width( x.x );
      return stream;
   }
   
   ostream & operator<< ( ostream & stream, const setbase & x ){
      stream.base( x.x );
      return stream;
   }
   
   
   ostream & operator<< ( ostream & stream, const _showpos & x ){
      stream.showpos( x.x );
      return stream;
   }
   
   ostream & operator<< ( ostream & stream, const _showbase & x ){
      stream.showbase( x.x );
      return stream;
   }
   
   ostream & operator<< ( ostream & stream, const _boolalpha & x ){
      stream.boolalpha( x.x );
      return stream;
   }
     
   ostream & operator<< ( ostream & stream, const setfill x ){
      stream.fill( x.x );
      return stream;
   }
   
   ostream & operator<< ( ostream & stream, const _right x ){
      stream.right();
      return stream;
   }
   
   ostream & operator<< ( ostream & stream, const _left x ){
      stream.left();
      return stream;
   }
   
   ostream & operator<< ( ostream & stream, const _flush x ){
      stream.flush();
      return stream;
   }
   
   int strlen( const char *s ){
      int n = 0;
      while( *s != '\0' ){
         n++;
         s++;
      }
      return n; 
   }
   
   ostream & operator<< ( ostream & stream, const char *s ){
     if( stream.must_align_right()){
        stream.filler( stream.width() - strlen( s )); 
     }    
      for( const char *p = s; *p != '\0'; p++ ){
         stream << *p;
      }
     if( ! stream.must_align_right()){
        stream.filler( stream.width() - strlen( s )); 
     }  
      stream.width( 0 );
      return stream;
   }
   
   template< int size > class mkt_ostream_reverse_string : public ostream {
      public: 
        mkt_ostream_reverse_string( void ){ p = size; s[ size ] = '\0'; }
         void putc( char c ){
            if( p > 0 ){
               s[ --p ] = c;
            }
         }
         const char *result( void ){ return & s[ p ]; }
      private:
         char s[ size + 1 ];   
         int p;
   };
   
   ostream & operator<< ( ostream & stream, bool x ){
      if( stream.boolalpha()){
         if( x ){
            stream << "true";
         } else {
            stream << "false";
         }
      } else {
         if( x ){
            stream << "1";
         } else {
            stream << "0";
         }
      }
      return stream;   
   }
   
   const char * ostream :: prefix( void ){
      if( ! show_base ){
         return "";
      } else {
         switch( numerical_radix ){
            case 2  : return "b0";
            case 8  : return "o0";
            case 10 : return "";
            case 16 : return "x0";
         }
      }
      return "?0";
   }
   
   ostream & operator<< ( ostream & stream, int x ){
      mkt_ostream_reverse_string<70> s;
      bool minus = ( x < 0 );
      if( x < 0 ){ x = -x; }
      if( x == 0 ){
         s.digit( 0 );
      }
      while( x > 0 ){
         s.digit( x % stream.base() );
         x = x / stream.base();
      }
      s << stream.prefix();
      if( minus ){
         s.putc( '-' );
      } else if( stream.showpos() ){
         s.putc( '+' );
      }   
      stream << s.result();
      return stream;   
   }
   
    ostream & operator<< ( ostream & stream, long int x ){
      return stream << (int) x;
   }
   
   ostream & operator<< ( ostream & stream, long long int x ){
      mkt_ostream_reverse_string<70> s;

      bool minus = ( x < 0 );

      
      if( x < 0 ){ x = -x; }

      if( x == 0 ){
         s.digit( 0 );
      }

      //stream.base( 10 );
   	while( x != 0 ){
         s.digit( x % stream.base() );
         x = x / stream.base();
      }
      s << stream.prefix();

      if( minus ){
         s.putc( '-' );
      } else if( stream.showpos() ){
         s.putc( '+' );
      }   

      stream << s.result();

      return stream;   
   }
   
   ostream & operator<< ( ostream & stream, unsigned int x ){
      return stream << (long long int) x;      
   }
   
   ostream & operator<< ( ostream & stream, unsigned long int x ){
      return stream << (long long int) x;
   }
   
   ostream & operator<< ( ostream & stream, unsigned long long int x ){
      return stream << (long long int) x;
   }
   
   ostream & operator<< ( ostream & stream, signed char c ){
      stream.putc( c );
	  return stream;
   }
   
   ostream & operator<< ( ostream & stream, unsigned char c ){
      stream.putc( c );
	  return stream;
   }

}; // namespace std

