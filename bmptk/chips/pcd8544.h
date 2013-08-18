// - http://wiki.noccylabs.info/wiki/Philips_PCD8544

namespace bmptk{
namespace chips {

//! create a frame controlled by a pcd8544 LCD controller 
template<
   typename sce,
   typename res,
   typename dc,
   typename sdin,
   typename sclk,
   int x_size,
   int y_size
> class pcd8544 : public bmptk::graphics::frame {
private:
	
   static unsigned char buf[ (( x_size * y_size ) + 7 ) / 8 ];
	
   static void send_byte( unsigned char d ){
      for( int i = 0; i < 8; i++ ){
         sdin::set( d & 0x80 );         
   	   bmptk::wait( 1 * bmptk::us );
         sclk::set( 1 );
         d = d << 1;
   	   bmptk::wait( 1 * bmptk::us );
         sclk::set( 0 );
      }
   }    
   
   static void data( unsigned char d ){
      dc::set( 1 );
      sce::set( 0 );
      send_byte( d );
      sce::set( 1 );
   } 	
   
   static void pixels( unsigned char x, unsigned char y, unsigned char d ){
      command( 0x80 | x );   
      command( 0x40 | y );  
      data( d );
   }
   
protected:   
   
   //! send  command to the controller chip
   static void command( unsigned char d ){
      dc::set( 0 );
      sce::set( 0 );
      send_byte( d );
      sce::set( 1 );
   } 	

public:	

   //! construct the controller
   //
   //! The argument is the size of the frame
   //! The initialization of the controller is 
   //! LCD-specific and must be done by the derived type.
   pcd8544(): 
      bmptk::graphics::frame( bmptk::graphics::vector( x_size, y_size ))
   { 
      sce::init();
   	  res::init();
	  dc::init();
	  sdin::init();
	  sclk::init();

  	  sclk::set( 0 );
   	  bmptk::wait( 1 * bmptk::us );
   	  sce::set( 1 );
   	  bmptk::wait( 1 * bmptk::us );
	  res::set( 0 );
   	  bmptk::wait( 1 * bmptk::us );
	  res::set( 1 ); 
 	  bmptk::wait( 1 * bmptk::us );
	   
      // initialization is not done here
   }	
   
   //! \copydoc bmptk::graphics::frame::checked_write
   void checked_write( 
      const bmptk::graphics::vector p, 
      const bmptk::graphics::color c 
   ){
      unsigned int a = p.x_get() + ( p.y_get() / 8 ) * 84;
      unsigned int m = 1 << ( p.y_get() % 8 );
      if( c == bmptk::graphics::color::black() ){
         buf[ a ] |= m;
      } else {     
         buf[ a ] &= ~m;
      }
      pixels( p.x_get(), p.y_get() / 8, buf[ a ] );         
   }
   
   //! \copydoc bmptk::graphics::frame::clear
   void clear( 
      const bmptk::graphics::color c 
         = bmptk::graphics::color::white() 
   ){
      unsigned char fill = c == bmptk::graphics::color::white() ? 0 : 0xFF;
      command( 0x80 | 0 );   
      command( 0x40 | 0 );  
      for( int i = 0; i < 504; i++ ){
         buf[ i ] = fill;
         data( fill );
      }         
   }

};

// buf instance
template <
   typename sce,
   typename res,
   typename dc,
   typename sdin,
   typename sclk,
   int x_size,
   int y_size
> unsigned char pcd8544 < 
   sce, res, dc, sdin, sclk, x_size, y_size 
>::buf[ (( x_size * y_size ) + 7 ) / 8 ];

//! create a frame for an generic 5510 LCD
template<
   typename sce,
   typename res,
   typename dc,
   typename sdin,
   typename sclk
> class lcd5510 : public pcd8544 < sce, res, dc, sdin, sclk, 84, 48 > {

   void command( unsigned char d ){ 
      pcd8544<sce,res,dc,sdin,sclk,84,48>::command( d ); 
   }

public:

   lcd5510(){
      command( 0x21 );  // select exteded instructions
      command( 0xC8 );  // Vop = 110000b
      command( 0x06 );  // TCx = 00b
      command( 0x13 );  // BSx = 100b
      command( 0x20 );  // select basic instructions, horizontal addressing
      command( 0x0C );  // normal mode  
   }	  
};

//! create a frame for an Olimex LCD3310 LCD 
template<
   typename sce,
   typename res,
   typename dc,
   typename sdin,
   typename sclk
> class lcd3310 : public pcd8544< sce, res, dc, sdin, sclk, 84, 48 > {

   void command( unsigned char d ){ 
      pcd8544<sce,res,dc,sdin,sclk,84,48>::command( d ); 
   }

public:
   lcd3310(){
	   // initialization according to the Olimex example
	   
      command( 0x21 );  // select exteded instructions
      command( 0xC8 );  // Vop = 110000b
      command( 0x04 );  // TCx = 00b
      command( 0x12 );  // BSx = 100b, LCD bias mode 1:48.
     
      command( 0x04 | 1 );  // mystery command, set temp S6 for start line?
      command( 0x40 | (64 - 3 ) );  // mystery command, set temp S[5:0] for start line?
      
      command( 0x20 );  // select basic instructions, horizontal addressing
      command( 0x0C );	// normal mode    
   }	  
   
};

}; // namespace chips
}; // namespace bmptk
