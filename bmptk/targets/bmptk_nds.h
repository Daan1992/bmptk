#pragma once
#ifndef BMPTK_NDS_H
#define BMPTK_NDS_H

#include "nds.h"
#include <nds/touch.h>

#include "bmptk.h"

namespace bmptk { 
namespace graphics {

#ifdef BMPTK_GRAPHICS

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

#endif

} // namespace graphics
} // namespace bmptk

#endif // #ifdef BMPTK_NDS_H