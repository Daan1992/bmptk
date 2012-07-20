#ifndef _BMPTK_NDS_H_
#define _BMPTK_NDS_H_

#include <nds.h>
#include <nds/touch.h>

#include "bmptk.h"
#include "graphics.h"

using namespace bmptk;
using namespace bmptk::graphics;

class nintendo_screen : public frame {
public:
   nintendo_screen( void );
   void checked_write( const vector v, const color c );
};

#endif // #ifdef _BMPTK_NDS_H_    