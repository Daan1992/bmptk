#ifndef _BMPTK_WIN_H_
#define _BMPTK_WIN_H_

#include "bmptk.h"

namespace bmptk {
namespace graphics {

#ifdef BMPTK_GRAPHICS

class target_screen : public frame {
public:
   target_screen( void );
   void checked_write( const vector v, const color c );
};

#endif

} // namespace graphics
} // namespace bmptk

#endif // #ifdef _BMPTK_WIN_H_ 
