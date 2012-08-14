#pragma once
#ifndef BMPTK_NDS_H
#define BMPTK_NDS_H

#include <nds.h>
#include <nds/touch.h>

#include "bmptk.h"

namespace bmptk { 

class target_screen : public frame {
public:
   target_screen( void );
   void checked_write( const vector v, const color c );
};

}

#endif // #ifdef BMPTK_NDS_H