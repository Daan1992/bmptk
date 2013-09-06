// run the HopeRF demo for the RFM73 module

#include "bmptk.h"

typedef bmptk::hardware::rfm73<
   bmptk::target::gpio_0_10_out,  // sclk
   bmptk::target::gpio_0_9_out,   // mosi
   bmptk::target::gpio_0_8_in,    // miso
   bmptk::target::gpio_0_2_out,   // csn
   bmptk::target::gpio_0_3_out    // ce
> radio;

int main( void ){	  
   bmptk::hardware::hoperf_rfm73_demo<
      radio,
      bmptk::target::gpio_1_8_out,
      bmptk::target::gpio_1_5_out
   >();
}
