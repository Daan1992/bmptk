// basic blink-a-led example
//
// The bmptk::target:: provides the pin_out, 
// the bmptk::hardware:: provides a blink.
// Just instantiate the blink with the pin_out and let it run.

#include "bmptk.h"

typedef bmptk::hardware::nrf24l01<
   bmptk::target::gpio_0_10_out,  // sclk
   bmptk::target::gpio_0_9_out,   // mosi
   bmptk::target::gpio_0_8_in,    // miso
   bmptk::target::gpio_0_2_out,   // csn
   bmptk::target::gpio_0_4_out    // ce
> radio;

typedef bmptk::target::gpio_1_9_out  led;
typedef bmptk::target::gpio_1_8_out  led_tx;
typedef bmptk::target::gpio_1_5_out  led_rx;

void wait_ms( unsigned int n ){ bmptk::wait( n * bmptk::ms); }
void wait_us( unsigned int n ){ bmptk::wait( n * bmptk::us ); }

unsigned char transmit_led = 0, receive_led = 0;

const unsigned char tx_buf[17]={
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x78};
    
void rfm73_test(){   
   led_tx::init();
   led_rx::init();
   led::init();
   
   led_tx::set( true ); 
   led_rx::set( true );
   led::set( false );
 
   radio::init();   
   // radio::channel( 23 );
   radio::mode_receive(); 
   
   if(0)if( ! radio::is_present() ){
      bmptk::hardware::blink< 
	     bmptk::hardware::tee< led_tx, led_rx >, 
		 100 * bmptk::ms
      >();
   }

   led_tx::set( false );
   led_rx::set( false );
      
	unsigned char len,i,chksum, n = 0;
	unsigned char rx_buf[ radio::NRF24L01_MAX_PACKET_LEN ];
	while(1){
        wait_us( 2 * 1000 );

        led_tx::set( transmit_led > 0 );
        if( transmit_led > 0 ){ transmit_led--; }

        led_rx::set( receive_led > 0 );
        if( receive_led > 0 ){ receive_led--; }  
		     
        if(1) if( ++n > 250 ){
	       	radio::mode_transmit();
	         transmit_led = 20;	
	         if(0) radio::buffer_write( 
               radio::NRF24L01_CMD_W_TX_PAYLOAD_NOACK, tx_buf, 17 ); 
	         radio::transmit_message_once( tx_buf, 17 ); 
            bmptk::wait( 10 * bmptk::ms );	 	
            radio::mode_receive(); 
            n = 0; 	
        }

   	    if( ! radio::receive_fifo_empty() ){
receive_led = 20;

		    len=radio::register_read( radio::NRF24L01_CMD_R_RX_PL_WID );
			if( len <= radio::NRF24L01_MAX_PACKET_LEN ){
				radio::buffer_read( radio::NRF24L01_CMD_R_RX_PAYLOAD,rx_buf,len );

 
	   	       chksum = 0;
		       for(i=0;i<16;i++){
			      chksum +=rx_buf[i]; 
		       }

		       if(chksum==rx_buf[16]&&rx_buf[0]==0x30){
		          receive_led = 20;
			      // Send_Packet( radio::NRF24L01_CMD_W_TX_PAYLOAD_NOACK,rx_buf,17);
		       }	
			}
			radio::register_write( radio::NRF24L01_CMD_FLUSH_RX,0 );
      }
	}
}
	
int main( void ){	  
   rfm73_test();
}
