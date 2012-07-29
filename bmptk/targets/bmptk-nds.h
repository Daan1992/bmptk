#ifndef _BMPTK_NDS_H_
#define _BMPTK_NDS_H_

#include <nds.h>
#include <nds/touch.h>

#include "bmptk.h"
#include "graphics.h"

namespace bmptk { namespace graphics {

class target_screen : public frame {
public:
   target_screen( void );
   void checked_write( const vector v, const color c );
};

}}

#endif // #ifdef _BMPTK_NDS_H_    