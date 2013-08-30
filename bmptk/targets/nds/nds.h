#include "libnds/include/nds.h"

namespace bmptk { 
namespace graphics {

//! the main screen of the target hardware
class target_screen : public bmptk::graphics::frame {
public:
   target_screen( void );
   void checked_write( const vector v, const color c );
};

//! specific for an NDS: the top screen
class target_top_screen : public bmptk::graphics::frame {
public:
   target_top_screen( void );
   void checked_write( const vector v, const color c );
};

} // namespace graphics
} // namespace bmptk
