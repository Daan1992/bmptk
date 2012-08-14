#ifndef _BMPTK_WIN_H_
#define _BMPTK_WIN_H_

#include "bmptk.h"

namespace bmptk {

class target_screen : public frame {
public:
   target_screen( void );
   void checked_write( const vector v, const color c );
};

}

#endif // #ifdef _BMPTK_WIN_H_ 
