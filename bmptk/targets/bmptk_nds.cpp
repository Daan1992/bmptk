#include "bmptk.h"

using namespace bmptk;

target_screen :: target_screen( void ):
   frame( vector( 255, 191 ))
{
    videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
    lcdMainOnBottom();
}

void target_screen :: checked_write( 
   const vector v,
   const color c 
){
   BG_GFX[ v.x_get() + ( v.y_get() * 256 ) ] = 0x8000 | c.rgb15();
}

target_top_screen :: target_top_screen( void ):
   frame( vector( 255, 191 ))
{    
    videoSetModeSub(MODE_5_2D);
    vramSetBankC(VRAM_C_SUB_BG);
    bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
    lcdMainOnBottom();
}

void target_top_screen :: checked_write( 
   const vector v,
   const color c 
){
   BG_GFX_SUB[ v.x_get() + ( v.y_get() * 256 ) ] = 0x8000 | c.rgb15();
}