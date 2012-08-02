#include "bmptk.h"
#include <stdlib.h>

using namespace bmptk;
using namespace graphics;

#define C_GLCD_REFRESH_FREQ     (50HZ)
#define C_GLCD_H_SIZE           480       
#define C_GLCD_H_PULSE          45        
#define C_GLCD_H_FRONT_PORCH    45       
#define C_GLCD_H_BACK_PORCH     45     
#define C_GLCD_V_SIZE           272     
#define C_GLCD_V_PULSE          16      
#define C_GLCD_V_FRONT_PORCH    16     
#define C_GLCD_V_BACK_PORCH     16      

#define C_GLCD_PWR_ENA_DIS_DLY  50000
#define C_GLCD_ENA_DIS_DLY      50000

#define CRSR_PIX_32     0
#define CRSR_PIX_64     1
#define CRSR_ASYNC      0
#define CRSR_FRAME_SYNC 2

typedef unsigned int Int32U;
typedef unsigned int * pInt32U;

#define MHZ           *1000000L
#define KHZ           *1000L
#define HZ            *1L

#define I_RC_OSC_FREQ   (4MHZ)
#define MAIN_OSC_FREQ   (12MHZ)
#define RTC_OSC_FREQ    (32768UL)

#define SYS_FREQ        (72MHZ)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define _2BL(a)   (Int8U)(a),(Int8U)(a>>8)
#define _2BB(a)   (Int8U)(a>>8),(Int8U)(a),
#define _3BL(a)   (Int8U)(a),(Int8U)(a>>8),(Int8U)(a>>16)
#define _3BB(a)   (Int8U)(a>>16),(Int8U)(a>>8),(Int8U)(a)
#define _4BL(a)   (Int8U)(a),(Int8U)(a>>8),(Int8U)(a>>16),(Int8U)(a>>24)
#define _4BB(a)   (Int8U)(a>>24),(Int8U)(a>>16),(Int8U)(a>>8),(Int8U)(a)

#define LongToBin(n) (((n >> 21) & 0x80) | \
                      ((n >> 18) & 0x40) | \
                      ((n >> 15) & 0x20) | \
                      ((n >> 12) & 0x10) | \
                      ((n >>  9) & 0x08) | \
                      ((n >>  6) & 0x04) | \
                      ((n >>  3) & 0x02) | \
                      ((n      ) & 0x01))

#define __BIN(n) LongToBin(0x##n##l)

#define BIN8(n)                       __BIN(n)
#define BIN(n)                        __BIN(n)
#define BIN16(b1,b2)        ((        __BIN(b1)  <<  8UL) + \
                                      __BIN(b2))
#define BIN32(b1,b2,b3,b4) ((((Int32U)__BIN(b1)) << 24UL) + \
                            (((Int32U)__BIN(b2)) << 16UL) + \
                            (((Int32U)__BIN(b3)) <<  8UL) + \
                              (Int32U)__BIN(b4))

//#pragma section = "SDRAM_VRAM"
//#pragma location = "SDRAM_VRAM"
//Int32U LCD_Buffer[2][C_GLCD_H_SIZE * C_GLCD_V_SIZE];

//int BuffIndex = 0;

#define C_GLCD_CLK_PER_LINE     (C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH)
#define C_GLCD_LINES_PER_FRAME  (C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH)
#define C_GLCD_PIX_CLK          (9.1MHZ)

#define LCD_VRAM_BASE_ADDR ((Int32U) LCD_Buffer)

void GLCD_Init ( void )
{
pInt32U pDst = (pInt32U) LCD_VRAM_BASE_ADDR;
  // Assign pin
  PINSEL0 &= BIN32(11111111,11110000,00000000,11111111);
  PINSEL0 |= BIN32(00000000,00000101,01010101,00000000);
  PINMODE0&= BIN32(11111111,11111100,00000000,11111111);
  PINMODE0|= BIN32(00000000,00000010,10101010,00000000);
  PINSEL3 &= BIN32(11110000,00000000,00000000,11111111);
  PINSEL3 |= BIN32(00000101,01010101,01010101,00000000);
  PINMODE3&= BIN32(11110000,00000000,00000000,11111111);
  PINMODE3|= BIN32(00001010,10101010,10101010,00000000);
  PINSEL4 &= BIN32(11110000,00110000,00000000,00000000);
  PINSEL4 |= BIN32(00000101,01001111,11111111,11111111);
  PINMODE4&= BIN32(11110000,00110000,00000000,00000000);
  PINMODE4|= BIN32(00001010,10001010,10101010,10101010);
  PINSEL9 &= BIN32(11110000,11111111,11111111,11111111);
  PINSEL9 |= BIN32(00001010,00000000,00000000,00000000);
  PINMODE9&= BIN32(11110000,11111111,11111111,11111111);
  PINMODE9|= BIN32(00001010,00000000,00000000,00000000);
  PINSEL11&= BIN32(11111111,11111111,11111111,11110000);
  PINSEL11|= BIN32(00000000,00000000,00000000,00001111);
  
  PINSEL4 &= 0xfffffff3;      //set LCD backlite
  FIO2DIR |= 0x00000002;
  FIO2SET |= 0x00000002;
  
  PINSEL4 &= 0xfffffffc;      //set DISP 
  FIO2DIR |= 0x00000001;
  FIO2SET |= 0x00000001; 
  
  // Init GLCD cotroller
  PCONP_bit.PCLCD = 1;      // enable LCD controller clock
  CRSR_CTRL_bit.CrsrOn = 0; // Disable cursor
  LCD_CTRL_bit.LcdEn = 0;   // disable GLCD controller
  LCD_CTRL_bit.LcdBpp= 5;   // 24 bpp
  LCD_CTRL_bit.LcdTFT= 1;   // TFT panel
  LCD_CTRL_bit.LcdDual=0;   // single panel
  LCD_CTRL_bit.BGR   = 0;   // notmal output
  LCD_CTRL_bit.BEBO  = 0;   // little endian byte order
  LCD_CTRL_bit.BEPO  = 0;   // little endian pix order
  LCD_CTRL_bit.LcdPwr= 0;   // disable power
  // init pixel clock
  LCD_CFG_bit.CLKDIV =  SYS_GetFsclk() / (Int32U)C_GLCD_PIX_CLK;
  LCD_POL_bit.BCD    = 1;   // bypass inrenal clk divider
  LCD_POL_bit.CLKSEL = 0;   // clock source for the LCD block is HCLK
  LCD_POL_bit.IVS    = 1;   // LCDFP pin is active LOW and inactive HIGH
  LCD_POL_bit.IHS    = 1;   // LCDLP pin is active LOW and inactive HIGH
  LCD_POL_bit.IPC    = 0;   // data is driven out into the LCD on the rising edge
  LCD_POL_bit.IOE    = 0;   // active high
  LCD_POL_bit.CPL    = C_GLCD_H_SIZE-1;
  // init Horizontal Timing
  LCD_TIMH_bit.HBP   =  C_GLCD_H_BACK_PORCH - 1;
  LCD_TIMH_bit.HFP   =  C_GLCD_H_FRONT_PORCH - 1;
  LCD_TIMH_bit.HSW   =  C_GLCD_H_PULSE - 1;
  LCD_TIMH_bit.PPL   = (C_GLCD_H_SIZE/16) - 1;
  // init Vertical Timing
  LCD_TIMV_bit.VBP   =  C_GLCD_V_BACK_PORCH;
  LCD_TIMV_bit.VFP   =  C_GLCD_V_FRONT_PORCH;
  LCD_TIMV_bit.VSW   =  C_GLCD_V_PULSE;
  LCD_TIMV_bit.LPP   =  C_GLCD_V_SIZE - 1;
  // Frame Base Address doubleword aligned
  LCD_UPBASE         =  LCD_VRAM_BASE_ADDR & ~7UL ;
  LCD_LPBASE         =  LCD_VRAM_BASE_ADDR & ~7UL ;
  // init colour pallet

  {
 /*   // clear display memory
    for(Int32U i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
      *pDst++ = 0;
    }
  */
//------------------------------------------------------------------
    Int32U pix = 0;
    /* Set test picture */
    for(Int32U v = 0; (C_GLCD_V_SIZE) > v; v++)
    {
      pDst = (pInt32U)LCD_VRAM_BASE_ADDR + v*C_GLCD_H_SIZE;
      Int32U i = 0;

      for(Int32U n = 0; 8 > n; n++)
      {
        pix = testtbl[n]*0xFF/*(Int32U)(275*(1-exp(EXP_COEF*v)))*/;
        for(/*empty*/ ; i < C_GLCD_H_SIZE*(n+1)/8;i++)
        { 
           *(pDst+i) = pix;
        }
      }
    }
//-------------------------------------------------------------------    
    
  
  }

  for(volatile Int32U i = C_GLCD_ENA_DIS_DLY; i; i--);
}


unsigned int random_in_range( unsigned int min, unsigned int max ){
   unsigned int x = rand();
   x = x % ( max - min + 1 );
   return min + x;
}

vector random_vector_within( const vector size ){
   return vector( 
      random_in_range( 0, size.x_get()), 
      random_in_range( 0, size.y_get())
   );
}

int main( void ){
   target_screen lcd;
   for(;;){
      lcd.clear( color::red );
      for( unsigned int n = 0; n < 500; n++ ){
         vector start = random_vector_within( lcd.size_get());
         vector end = random_vector_within( lcd.size_get());
         lcd.draw( start, line( end - start, color::black ));
         swiWaitForVBlank();
      }
   }
   return 0;
}