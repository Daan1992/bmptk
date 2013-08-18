//***************************************************************************//
//
// COPYRIGHT NOTICE (zlib license)
//
// Loosely based on the example application provided by HopeRF
//
// (c) Wouter van Ooijen - wouter@voti.nl
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//***************************************************************************//


#ifndef _RFM73_H_
#define _RFM73_H_

//***************************************************************************//
//
//! \defgroup lowlevel low level interface
//!
//! The low-level interface provides read and write access to the 
//! commands and registers of the RFM73.
//!
//! When a register ( < 0x20 ) is specified for a read or write command
//! the appropriate read or write command bits are added automatically.
//!
//! To use any of these methods, (except init() itself)
//! the interface and the module must have been initialized by
//! an init() call.
//!
//! Besides the registers shown here (bank 0) the rfm73 also has a
//! set of registers in bank 1. These bank 1 registers are initialized by
//! the init() call. If you want to do this yourself: the datasheet
//! shows the required values, but in a very confusing way. The HopeRF 
//! example code is a better reference. No (or very scarce) explanation 
//! is given for these values.
//!
//! For most users, especially novices, it is recommended to use the
//! high level interaface instead of this low level interface.
//
//***************************************************************************//

//***************************************************************************//
//
//! \defgroup highlevel high level interface
//!
//! The high-level interface provides methods for using the rfm73 module.
//! These methods are implemneted by calling the appropriate low level
//! methods. 
//! When possible, it is recommended to use only these high level methods.
//! But when a functionality is needed that is missing it can be implemented
//! using the low level interface.
//!
//! To use any of these methods, (except init() itself)
//! the interface and the module must have been initialized by
//! an init() call.
//
//***************************************************************************//

namespace bmptk {
namespace chips {

template< 
   typename sclk,      
   typename mosi,      
   typename miso,      
   typename csn,     
   typename ce  
> class rfm73 {

public:

//! maximum number of data bytes in a (received or transmitted) rfm73 packet
//
//! \ingroup highlevel
static const unsigned int RFM73_MAX_PACKET_LEN = 32;

//! type of rfm73 (transmit or receive) buffer
//
//! \ingroup highlevel
typedef unsigned char rfm73_buffer [ RFM73_MAX_PACKET_LEN ];


//***************************************************************************//
//
// RFM73 SPI commands
//
//***************************************************************************//

//! \addtogroup lowlevel
//! @{

//! SPI comamnd to read a received payload
static const unsigned int RFM73_CMD_R_RX_PAYLOAD =        0x61;

//! SPI command to write a payload to be sent
static const unsigned int RFM73_CMD_W_TX_PAYLOAD =        0xA0;

//! SPI command to empty the transmit queue
static const unsigned int RFM73_CMD_FLUSH_TX =            0xE1;

//! SPI command to empty the receive queue
static const unsigned int RFM73_CMD_FLUSH_RX =            0xE2;

//! SPI command to start continuous retransmission
static const unsigned int RFM73_CMD_REUSE_TX_PL =         0xE3;

//! SPI command to write a payload to be sent without auto-acknowledgement
static const unsigned int RFM73_CMD_W_TX_PAYLOAD_NOACK =  0xB0;

//! SPI command to write the payload to be transmitted with an ack
static const unsigned int RFM73_CMD_W_ACK_PAYLOAD =       0xA8;

//! SPI command to toggle register bank or toggle extended functions
static const unsigned int RFM73_CMD_ACTIVATE =            0x50;

//! SPI command to read the payload length for the top payload in the FIFO
static const unsigned int RFM73_CMD_R_RX_PL_WID =         0x60;

//! SPI 'no peration', can be used to read the status register
static const unsigned int RFM73_CMD_NOP =                 0xFF;


//***************************************************************************//
//
// RFM73 register addresses
//
//***************************************************************************//

//! CONFIG : rfm73 configuration register
//
//! Bits (0 = LSB):
//! - 7 : reserved, must be 0
//! - 6 : 1 masks RX_DR (see...) from IRQ pin, 0 allows
//! - 5 : 1 masks RX_DS (see...) from IRQ pin, 0 allows
//! - 4 : 1 masks MAX_RT (see...) from IRQ pin, 0 allows
//! - 3 : 1 enables CRC (forced high when EN_AA != 0)
//! - 2 : 0 = 1 byte CRC, 1 = 2 byte CRC
//! - 1 : 0 = power down, 1 = power up
//! - 0 : 0 = transmit mode, 1 = receive mode
static const unsigned int RFM73_REG_CONFIG =              0x00;

//! EN_AA : enable auto ack on pipes
//
//! Bits (0 = LSB):
//! - 7, 6 : reserved, must be 00
//! - 5 : 0 disables auto ack on pipe 5, 1 enables
//! - 4 : 0 disables auto ack on pipe 4, 1 enables
//! - 3 : 0 disables auto ack on pipe 3, 1 enables
//! - 2 : 0 disables auto ack on pipe 2, 1 enables
//! - 1 : 0 disables auto ack on pipe 1, 1 enables
//! - 0 : 0 disables auto ack on pipe 0, 1 enables
static const unsigned int RFM73_REG_EN_AA =               0x01;

//! EN_RXADDR : enable receive pipes
//
//! Bits (0 = LSB):
//! - 7, 6 : reserved, must be 00
//! - 5 : 0 disables receive pipe 5, 1 enables
//! - 4 : 0 disables receive pipe 4, 1 enables
//! - 3 : 0 disables receive pipe 3, 1 enables
//! - 2 : 0 disables receive pipe 2, 1 enables
//! - 1 : 0 disables receive pipe 1, 1 enables
//! - 0 : 0 disables receive pipe 0, 1 enables
static const unsigned int RFM73_REG_EN_RXADDR =           0x02;

//! SETUP_AW : set address length
//
//! Bits (0 = LSB):
//! - 7 .. 2 : reserved, must be 000000
//! - 1 .. 0 : 00 = illegal, 01 = 3 bytes, 10 = 4 bytes, 11 = 5 bytes
static const unsigned int RFM73_REG_SETUP_AW =            0x03;

//! SETUP_RETR : retransmission settings
//
//! Bits (0 = LSB):
//! - 7 .. 4 : delay between (re) transmissions, ( n + 1 ) * 250 us
//! - 3 .. 0 : max number of retransmissions, 0 disableles retransmissions
static const unsigned int RFM73_REG_SETUP_RETR =          0x04;

//! RF_CH : RF channel (frequency)
//
//! The RF channel frequency is 2.4 MHz + n * 1 MHz.
static const unsigned int RFM73_REG_RF_CH =               0x05;

//! RF_SETUP : RF setup: data rate, transmit power, LNA
//
//! Bits (0 = LSB):
//! - 7 .. 4 : reserved, must be 0011
//! - 3 : air data rate, 0 = 1 Mbps, 1 = 2 Mbps, 2 = 250Kbps
//! - 2 .. 1 : transmit power, 00 = -10 dBm, 01 = -5 dBm, 10 = 0 dBm, 11 = 5 dBm
//! - 0 : LNA gain, 0 = - 20 dB (low gain), 1 = standard
static const unsigned int RFM73_REG_RF_SETUP =            0x06;

//! STATUS : status register
//
//! The value of this register is also clocked out
//! while a SPI command is clocked in.
//!
//! Bits (0 = LSB):
//! - 7 : active register bank, 0 = bank 0, 1 = bank 1
//! - 6 : data available, 0 = RX FIFO not empty, 1 = RX FIFO empty
//! - 5 : data sent, 0 = no packet sent, 1 = packet has been sent
//! - 4 : 1 = maximum number of retransmissions reached
//! - 3 .. 1 : data pipe of the message at the RX queue head
//! - 0 : TX FIFO full: 0 = TX FIFO not full, 1 = TX FIFO full
//!
//! Bits 6,5,4 are cleared by writing a 1 (!) in that position.
//! When bit 4 is set this will block any communication.
//! When auto retransmission is enabled bit 5 will be set only
//! after the acknowledge has been received.
static const unsigned int RFM73_REG_STATUS =              0x07;

//! OBSERVE_TX : lost and retransmitted packets
//
//! Bits (0 = LSB):
//! - 7 .. 4 : counts number of lost packets
//! - 3 .. 0 : counts retranmits 
//! The lost packets counter will not increment beyond 15. 
//! It is reset by writing to the channel frequency register.
//!
//! The retransmits counter can not increment beyond 15 because
//! the maximum number of transmissions is 15. This counter
//! is reset when the transmission of a new packet starts.
static const unsigned int RFM73_REG_OBSERVE_TX =          0x08;

//! CD : carrier detect
//
//! Bits (0 = LSB):
//! - 7 .. 1 : reserved
//! - 1 : carrier detect
static const unsigned int RFM73_REG_CD =                  0x09;

//! RX_ADDR_PO : receive address for data pipe 0, 5 bytes
//
//! This is the (up to) 5 byte receive address for data pipe 0.
//! For auto acknowledgement to work this address must be 
//! the same as the transmit address.
static const unsigned int RFM73_REG_RX_ADDR_P0 =          0x0A;

//! RX_ADDR_P1 : receive address for data pipe 1, 5 bytes
//
//! This is the (up to) 5 byte receive address for data pipe 1.
//! The higher bytes (all but the LSB) are also used in
//! the receive addresses of data pipes 2 .. 5.
static const unsigned int RFM73_REG_RX_ADDR_P1 =          0x0B;

//! RX_ADDR_P2 : receive address for data pipe 2, 1 byte
//
//! This is the LSB of the receive address for data pipe 2.
//! The higher bytes are copied from the receive address of
//! data pipe 1.
static const unsigned int RFM73_REG_RX_ADDR_P2 =          0x0C;

//! RX_ADDR_P3 : receive address for data pipe 3, 1 byte
//
//! This is the LSB of the receive address for data pipe 3.
//! The higher bytes are copied from the receive address of
//! data pipe 1.
static const unsigned int RFM73_REG_RX_ADDR_P3 =          0x0D;

//! RX_ADDR_P4 : receive address for data pipe 4, 1 byte
//
//! This is the LSB of the receive address for data pipe 4.
//! The higher bytes are copied from the receive address of
//! data pipe 1.
static const unsigned int RFM73_REG_RX_ADDR_P4 =          0x0E;

//! RX_ADDR_P5 : receive address for data pipe 5, 1 byte
//
//! This is the LSB of the receive address for data pipe 2.
//! The higher bytes are copied from the receive address of
//! data pipe 5.
static const unsigned int RFM73_REG_RX_ADDR_P5 =          0x0F;

//! TX_ADDR : tranmsit adress, 5 bytes
//
//! This is the (up to) 5 byte adress used in transmitted packets.
//! For auto acknowledgement to work this address must be 
//! the same as the pipe 0 receive address.
static const unsigned int RFM73_REG_TX_ADDR =             0x10;

//! RX_PW_P0 : number of bytes in package received into pipe 0
//
//! This is the number of data bytes in the message at
//! the head of receive pipe 0.
static const unsigned int RFM73_REG_RX_PW_P0 =            0x11;

//! RX_PW_P1 : number of bytes in package received into pipe 1
//
//! This is the number of data bytes in the message at
//! the head of receive pipe 1.
static const unsigned int RFM73_REG_RX_PW_P1 =            0x12;

//! RX_PW_P2 : number of bytes in package received into pipe 2
//
//! This is the number of data bytes in the message at
//! the head of receive pipe 2.
static const unsigned int RFM73_REG_RX_PW_P2 =            0x13;

//! RX_PW_P3 : number of bytes in package received into pipe 3
//
//! This is the number of data bytes in the message at
//! the head of receive pipe 3.
static const unsigned int RFM73_REG_RX_PW_P3 =            0x14;

//! RX_PW_P4 : number of bytes in package received into pipe 4
//
//! This is the number of data bytes in the message at
//! the head of receive pipe 4.
static const unsigned int RFM73_REG_RX_PW_P4 =            0x15;

//! RX_PW_P5 : number of bytes in package received into pipe 5
//
//! This is the number of data bytes in the message at
//! the head of receive pipe 5.
static const unsigned int RFM73_REG_RX_PW_P5 =            0x16;

//! FIFO_STATUS : receive and transmit FIFO status (readonly)
//
//! Bits (0 = LSB):
//! - 7   : reserved, only 0 allowed
//! - 6   : high = re-use last transmitted packet
//! - 5   : high = transmit FIFO is full
//! - 4   : high = transmit FIFO is empty
//! - 3:2 : reserved, only 0 allowed
//! - 1   : high = receive FIFO is full
//! - 0   : high = receive FIFO is empty
static const unsigned int RFM73_REG_FIFO_STATUS =         0x17;

//! DYNPD: dynamic payload flags
//
//! Bits (0 = LSB):
//! - 7:6 : reserved, only 00 allowed
//! - 5   : high = dynamic payload enabled on data pipe 5
//! - 4   : high = dynamic payload enabled on data pipe 4
//! - 3   : high = dynamic payload enabled on data pipe 3
//! - 2   : high = dynamic payload enabled on data pipe 2
//! - 1   : high = dynamic payload enabled on data pipe 1
//! - 0   : high = dynamic payload enabled on data pipe 0
//! Setting dynamic payload on pipe x requires EN_DPL 
//! (in the special features flags register) and ENAA_Px.
static const unsigned int RFM73_REG_DYNPD =               0x1C;

//! FEATURE: special fature flags
//
//! Bits (0 = LSB):
//! - 7:3 : reserved, only 00000 allowed
//! - 2   : (EN_DPL) high = enable dynamic payload length
//! - 1   : (EN_ACK_PAY) high = enable payload with ack
//! - 0   : (EN_DYN_ACK) high = enables W_TX_PAYLOAD_NOACK command 
static const unsigned int RFM73_REG_FEATURE =             0x1D;

//! @}   
       
  // RFM73 SPI read and write commands
static const unsigned int RFM73_CMD_READ_REG =            0x00;
static const unsigned int RFM73_CMD_WRITE_REG =           0x20;

//interrupt status
static const unsigned int STATUS_RX_DR =                  0x40;
static const unsigned int STATUS_TX_DS =                  0x20;
static const unsigned int STATUS_MAX_RT =                 0x10;

static const unsigned int STATUS_TX_FULL =                0x01;

//FIFO_STATUS
static const unsigned int FIFO_STATUS_TX_REUSE =          0x40;
static const unsigned int FIFO_STATUS_TX_FULL =           0x20;
static const unsigned int FIFO_STATUS_TX_EMPTY =          0x10;

static const unsigned int FIFO_STATUS_RX_FULL =           0x02;
static const unsigned int FIFO_STATUS_RX_EMPTY =          0x01;

private:   

   // write-and-read ction on the SPI bus
static unsigned char spi_read_write( unsigned char value ){
   unsigned char i;
   for( i =0 ; i < 8; i++ ){
      bmptk::wait( 1 * bmptk::us );
      mosi::set( value & 0x80  );      
      value = (value << 1);    // shift next bit into MSB..
      
      bmptk::wait( 1 * bmptk::us );
      sclk::set( 1 );          
      value |= miso::get();     // capture current MISO bit
      
      bmptk::wait( 1 * bmptk::us );
      sclk::set( 0 );
      bmptk::wait( 1 * bmptk::us );
  }
  return value;
}

   // select the indicated bank
static void bank( unsigned char b ){
   unsigned char st = 0x80 & rfm73::register_read( RFM73_REG_STATUS );
   if(( st &&( b == 0 )) || (( st == 0 ) && b )){
      rfm73::register_write( RFM73_CMD_ACTIVATE, 0x53 );
   }
}

   // initialize bank1 like in the example code.
   // don't ask why, just do it
static void init_bank1( void ){
   unsigned char i, j;
   unsigned char WriteArr[ 12 ];
        
   // magic bank1 register initialization values
   static const unsigned long Bank1_Reg0_13[] = {
   0xE2014B40,
   0x00004BC0,
   0x028CFCD0,
   0x41390099,
   0x1B8296d9, 
   0xA67F0624,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00127300,
   0x36B48000 };   
   
   // more magic bank1 register initialization values
   static const unsigned char Bank1_Reg14[] = {
   0x41, 0x20, 0x08, 0x04, 0x81, 0x20, 0xCF, 0xF7, 0xFE, 0xFF, 0xFF };
   
   bank( 1 );
   
   for( i = 0; i <= 8; i++ ){ //reverse!
      for( j = 0; j < 4; j++ ){
         WriteArr[ j ]=( Bank1_Reg0_13[ i ]>>(8*(j) ) )&0xff;
      }  
      buffer_write( i,WriteArr, 4 );
   }

   for( i = 9; i <= 13; i++ ){
      for( j = 0; j < 4; j++ ){
         WriteArr[ j ]=( Bank1_Reg0_13[ i ]>>(8*(3-j) ) )&0xff;
      }
      buffer_write( i, WriteArr, 4 );
   }

   rfm73::buffer_write( 14, Bank1_Reg14, 11 );

   //toggle REG4<25,26>
   for(j=0;j<4;j++){
      WriteArr[ j ]=( Bank1_Reg0_13[ 4 ]>>(8*(j) ) )&0xff;
   } 

   WriteArr[ 0 ] = WriteArr[ 0 ] | 0x06;
   rfm73::buffer_write( 4, WriteArr, 4);

   WriteArr[ 0 ] = WriteArr[ 0 ] & 0xf9;
   rfm73::buffer_write( 4, WriteArr,4);

   rfm73::bank( 0 );
}

   
public:   
   
//! initialize the library and the rfm73 module
//
//! \ingroup lowlevel
//! This method must be called before any other rfm73 
//! method is called. It can also be called later (maybe even
//! periodically) to re-initialize the interafce and the module.
//!
//! The rfm73 is initialized to
//! - 2 byte CRC
//! - power up
//! - receive mode
//! - auto-acknowledge on all pipes enabled
//! - pipes 0 and 1 are enabled, others disabled
//! - use 5 byte addresses
//! - auto retransmission delay 4000 ms, retry 15 times
//! - use channel 10
//! - air data rate 1Mbit power 5dbm, LNA gain high
//! - use some fixed address
static void init(){

// Bank0 register initialization values
#define BANK0_ENTRIES 10
static const unsigned char Bank0_Reg[ BANK0_ENTRIES ][ 2 ]={
   {  0, 0x0F }, // receive, enabled, CRC 2, enable interupts
   {  1, 0x3F }, // auto-ack on all pipes enabled
   {  2, 0x03 }, // Enable pipes 0 and 1
   {  3, 0x03 }, // 5 bytes addresses
   {  4, 0xff }, // auto retransmission delay 4000 ms, 15 times
   {  5, 0x0A }, // channel 10
   {  6 ,0x07 }, // data rate 1Mbit, power 5dbm, LNA gain high
   {  7, 0x07 }, // why write this at all?? but seems required to work...
   {  8, 0x00 }, // clear Tx packet counters
   { 23, 0x00 }, // fifo status
};

   // default receive address data pipe 0:
   // just a bunch of bytes, nothing magical
static const unsigned char RX0_Address[]={ 0x34, 0x43, 0x10, 0x10, 0x01 };

   sclk::init();
   mosi::init();
   miso::init();
   csn::init();
   ce::init();
   
   ce::set( 0 );
   csn::set( 1 );
   sclk::set( 0 );
   mosi::set( 0 );

   // delay at least 50ms.
   // the example code says so, but why??
   bmptk::wait( 50 * bmptk::ms );

   // write array of default init settings
   rfm73::bank( 0 );
   for( int i = 0; i < BANK0_ENTRIES; i++ ){
     rfm73::register_write( Bank0_Reg[ i ][0], Bank0_Reg[ i ][1] );
   }

   rfm73::receive_address_p0( RX0_Address );
   rfm73::receive_address_p1( RX0_Address );
   rfm73::transmit_address( RX0_Address );

   // enable the extra features
   int i = rfm73::register_read(29);
   if( i == 0 ){ // only when the extra features are not yet activated!
      rfm73::register_write( RFM73_CMD_ACTIVATE, 0x73 );
   }
   
   // select dynamic payload length data pipe5\4\3\2\1\0
   rfm73::register_write( 28, 0x3F ); 
   
   // select Dynamic Payload Length, Payload with ACK, W_TX_PAYLOAD_NOACK
   rfm73::register_write( 29, 0x07 );  
   
   // dynamic payload sizing on channels 0 and 1
   rfm73::channel_payload_size( 0, 0 );
   rfm73::channel_payload_size( 1, 0 );

   rfm73::init_bank1();
   bmptk::wait( 50 * bmptk::ms );
   rfm73::mode_receive();
   
} 

//! read a single-byte command or register
//
//! \ingroup lowlevel
//! This method reads and returns the a single-byte (8 bit) 
//! RFM73 command or register reg. 
static unsigned char register_read( unsigned char reg ){
   unsigned char value;
   
   if( reg < RFM73_CMD_WRITE_REG ){
      reg |= RFM73_CMD_READ_REG;       
   }  
   
   csn::set( 0 );               // CSN low, initialize SPI communication...
   (void)spi_read_write( reg );   // Select register to read from..
   value = spi_read_write( 0 );   // ..then read register value
   csn::set( 1 );               // CSN high, terminate SPI communication
   return value;                 // return register value
}

//! read a multi-byte command or register
//
//! \ingroup lowlevel
//! This method reads length bytes (8 bit each) from the RFM73 
//! command or register reg into the buffer buf. 
static void buffer_read(
   unsigned char reg,
   unsigned char pBuf[],
   unsigned char length
){
   unsigned char i;
   if( reg < RFM73_CMD_WRITE_REG ){
      reg |= RFM73_CMD_READ_REG;       
   }  
   csn::set( 0 );                     // Set CSN 0
   (void)spi_read_write( reg );         // Select register to write
   for( i = 0; i < length; i++ ){      // read all bytes
      pBuf[ i ] = spi_read_write( 0 );  // read one byte from RFM73
   }
   csn::set( 1 );                     // Set CSN high again
}

//! write a single-byte command or register
//
//! \ingroup lowlevel
//! This method writes the single-byte (8 bit) val to
//! the  RFM73 command or register reg. 
static void register_write( unsigned char reg, unsigned char value ){
   if( reg < RFM73_CMD_WRITE_REG ){
      reg |= RFM73_CMD_WRITE_REG;      
   }  
   csn::set( 0 );                // CSN low, init SPI transaction
   (void)spi_read_write( reg );    // select register
   (void)spi_read_write( value );  // ..and write value to it..
   csn::set( 1 );                // CSN high again
}

//! write a multi-byte command or register
//
//! \ingroup lowlevel
//! This method writes length bytes (8 bit each) from 
//! the buffer buf into the RFM73 
//! command or register reg. 
static void buffer_write(
   char reg,
   const unsigned char pBuf[],
   unsigned char length
){
   unsigned char i;
   if( reg < RFM73_CMD_WRITE_REG ){
      reg |= RFM73_CMD_WRITE_REG;      
   }  
   csn::set( 0 );                      // Set CSN low, init SPI tranaction
   (void)spi_read_write( reg );          // Select register to write tio write
   for( i = 0; i < length; i++ ){       // write all bytes in buffer(*pBuf)
      (void)spi_read_write( pBuf[ i ]);  // write one byte
   }   
   csn::set( 1 );                      // Set CSN high again
}

//***************************************************************************//
//
// high-level interface
//
//***************************************************************************//

//! report whether the rfm73 module is present
//
//! \ingroup highlevel
//! This method tests whether the rfm73 module is present.
//! It does so by reading the status register, and verifying that
//! an activate 0x53 command toggles the RBANK bit
//! in the status register.
static bool is_present( void ){
   unsigned char st1, st2; 
   st1 = rfm73::register_read( RFM73_REG_STATUS );
   rfm73::register_write( RFM73_CMD_ACTIVATE, 0x53 );
   st2 = rfm73::register_read( RFM73_REG_STATUS );
   rfm73::register_write( RFM73_CMD_ACTIVATE, 0x53 );
   return ( st1 ^ st2 ) == 0x80;
}

//! switch the rfm73 to transmit mode
//
//! \ingroup highlevel
//! This method flushes the transmit queue,
//! and switches the rfm73 to transmit mode.
static void mode_transmit( void ){
   unsigned char value;
   
   // flush transmit queue
   rfm73::register_write( RFM73_CMD_FLUSH_TX, 0);
   
   // clear interrupt status
   value = rfm73::register_read( RFM73_REG_STATUS );
   rfm73::register_write( RFM73_REG_STATUS ,value );
   
   // switch to transmit mode
   ce::set( 0 );
   value = rfm73::register_read( RFM73_REG_CONFIG );
   value &= 0xFE; // clear RX bit
   value |= 0x02; // set PWR_UP bit
   rfm73::register_write( RFM73_REG_CONFIG, value );
   ce::set( 1 );
}

//! switch the rfm73 to receive mode
//
//! \ingroup highlevel
//! This method flushes the receive queue,
//! and switches the rfm73 to transmit mode.
static void mode_receive( void ){
   unsigned char value;
   
   // flush receive queue 
   rfm73::register_write( RFM73_CMD_FLUSH_RX, 0 );
   
   // clear interrupt status
   value = rfm73::register_read( RFM73_REG_STATUS );
   rfm73::register_write( RFM73_REG_STATUS ,value );
   
   // switch to receive mode
   ce::set( 0 );
   value = rfm73::register_read( RFM73_REG_CONFIG );
   value |= 0x01; // set RX bit
   value |= 0x02; // set PWR_UP bit
   rfm73::register_write( RFM73_REG_CONFIG, value );
   ce::set( 1 );
}

//! switch the rfm73 to standby mode
//
//! \ingroup highlevel
//! This method puts the rfm73 in standby I mode,
//! which reduces the power consumption 
//! (50 uA max).
//! mode_powerdown() reduces the power consumption
//! even further, but requires a longer (but unspecified?)
//! time to return to active mode.
static void mode_standby( void ){
   unsigned char value;
   ce::set( 0 );
   value = rfm73::register_read( RFM73_REG_CONFIG );
   value |= 0x02; // set PWR_UP bit
   rfm73::register_write( RFM73_REG_CONFIG, value );
}

//! switch the rfm73 to power down mode
//
//! \ingroup highlevel
//! This method puts the rfm73 in power down mode,
//! which reduces the power consumption to a minimum
//! ( 3 uA max). 
static void mode_powerdown( void ){
   unsigned char value;
   ce::set( 0 );
   value = rfm73::register_read( RFM73_REG_CONFIG );
   value &= 0xFD; // clear PWR_UP bit
   rfm73::register_write( RFM73_REG_CONFIG, value );
}

//! set the rfm73 lna gain to low
//
//! \ingroup highlevel
//! This sets the LNA gain of the receiver to the low
//! level (-20 dB compared to the 'high' level).
static void lna_low( void ){
   unsigned char val = rfm73::register_read( RFM73_REG_RF_SETUP ); 
   val &= 0xFE;
   rfm73::register_write( RFM73_REG_RF_SETUP, val ); 
}

//! set the rfm73 lna gain to high
//
//! \ingroup highlevel
//! This sets the LNA gain of the receiver to the so-called
//! 'high' level. (I would have called it the 'normal' level.)
static void lna_high( void ){
   unsigned char val = rfm73::register_read( RFM73_REG_RF_SETUP ); 
   val |= 0x01;
   rfm73::register_write( RFM73_REG_RF_SETUP, val ); 
}

//! set the rfm73 channel frequency
//
//! \ingroup highlevel
//! This method sets the frequency (channel) used by the rfm73 for 
//! receiving and transmitting to ( 2400 + ch ) MHz.
//! The highest bit of val is ignored, so the frequency range is
//! 2.4 .. 2.517 GHz. 
//! Not all of these frequencies might be free to use in your jurisdiction.
static void channel( unsigned char ch ){
  // MSB must be 0
  rfm73::register_write( RFM73_REG_RF_CH, ch & 0x7E );
}

//! set the rfm73 air data rate (baudrate)
//
//! \ingroup highlevel
//! This method sets the air data rate used by the rfm73 for 
//! receiving and transmitting. 
//! Allowed values are 0 (250 Kbps), 1 (1 MBps) and 2 (Mbps).
//! A value > 2 will have the same effect as a value of 2.
//! Note that this is the bitrate the rfm73 uses in the 
//! packages that it sends. Due to various overhead factors
//! the data rate that a user of the module can achieve is much lower,
//! probably by a factor of 4.
static void air_data_rate( unsigned char rate ){
   unsigned char value; 
   ce::set( 0 );
   value = rfm73::register_read( RFM73_REG_RF_SETUP );
   value &= 0x07;
   if( rate == 0 ){
      value |= 0x20;  
   }
   if( rate > 1 ){
      value |= 0x08;  
   }
   rfm73::register_write( RFM73_REG_RF_SETUP, value );
   ce::set( 1 );
}

//! set the rfm73 CRC length
//
//! \ingroup highlevel
//! This method sets the length of the CRC used by the rfm73 in bytes.
//! Valid values are 0 (no CRC), 1 and 2.
//! A value > 2 has the same effect as the value 2.
//! Disabling the CRC disables the auto-acknowledge function.
//! Enabling the CRC does not automatically enable the
//! auto-acknowledge function.
static void crc_length( unsigned char len ){
   unsigned char val;
   if( len > 2 ){
      len = 2;
   }   
   val = rfm73::register_read( RFM73_REG_CONFIG );
   if( len == 0 ){
      val &= 0xF3;
      rfm73::register_write( RFM73_REG_EN_AA, 0 );
      rfm73::register_write( RFM73_REG_CONFIG, val );
   } else {
      rfm73::register_write( RFM73_REG_EN_AA, 0x3F );
      val &= 0xFB;
      if( val == 2 ){
         val |= 0x04; 
      }   
      rfm73::register_write( RFM73_REG_CONFIG, val );
   }  
}

//! set the rfm73 address length
//
//! \ingroup highlevel
//! This method sets the length (in bytes) of the addresses used by 
//! the rfm73. Valid values are 3, 4 and 5.
//! A value < 3 has the same effect as the value 3.
//! A value > 5 has the same effect as the value 5.
//! This setting is common for all data pipes.
static void address_length( unsigned char len ){
   if( len > 5 ){
      len = 5;
   }
   if( len < 3 ){
      len = 3;    
   }
   rfm73::register_write( RFM73_REG_SETUP_AW, len - 2  );   
}

//! set the rfm73 transmit power
//
//! \ingroup highlevel
//! This method sets the rfm73 transmitter output power level.
//! - level == 0 => -10 dBm
//! - level == 1 =>  -5 dBm
//! - level == 2 =>   0 dBm
//! - level == 3 =>  +5 dBm
//! 
//! A level > 3 has the same effect as level == 3.
static void power( unsigned char level ){
   if( level > 3 ){ 
       level = 3; 
   }
   ce::set( 0 );
   unsigned char val = rfm73::register_read( RFM73_REG_RF_SETUP ); 
   val &= 0x09; 
   val |= 0x30;
   val |= ( level << 1 );
   rfm73::register_write( RFM73_REG_RF_SETUP, val ); 
   ce::set( 1 );
}

//! set the retransmission delay and number of attempts
//
//! \ingroup highlevel
//! This method sets the delay d between retransmission attempts,
//! and the maximum number of attempts n.
//! The range of both arguments is 0..15. 
//! A value > 15 has the same effect as the value 15.
//!
//! The retransmission delay d is specified in steps of 250 us
//! with a minimum of 250 us:
//! - d == 0 => 250 us
//! - d == 1 => 500 us
//! - ...
//! - d == 15 => 4 ms
//!
//! The number of retransmissions n can range from
//! 0 (effectively disabling auto-retransmission) to 15.
static void retransmit_delay_attempts( unsigned char d, unsigned char n ){
   rfm73::register_write( 
      RFM73_REG_SETUP_RETR, ( n & 0x0F ) | (( d & 0x0F ) << 4 ));
}

//! read rfm73 retransmit count
//
//! \ingroup highlevel
//! This method reads and reports the number of retransmissions 
//! for the last packet that was sent. The number of retransmissions
//! is reset to zero when a new packet is sent.
static unsigned char retransmit_count( void ){
   return rfm73::register_read( RFM73_REG_OBSERVE_TX ) & 0x0F;   
}


//! read rfm73 lost packets count
//
//! \ingroup highlevel
//! This method reads and reports the number of lost packets.
//! The range of this count is 0..15, at 15 it will not
//! increment when a next packet is lost.
//! The lost packets count is reset implicitly when the channel
//! is written (by calling channel() ) or 
//! explicitly by calling lost_packets_reset().
static unsigned char lost_packets_count( void ){
   return ( rfm73::register_read( RFM73_REG_OBSERVE_TX ) >> 4 ) & 0x0F;   
}

//! reset rfm73 lost packets count
//
//! \ingroup highlevel
//! This method resets the number of lost packets by reading and
//! re-writing the RF channel (RFM73_REG_RF_CH register).
static void lost_packets_reset( void ){
   unsigned char val = rfm73::register_read( RFM73_REG_RF_CH );  
   rfm73::register_write( RFM73_REG_RF_CH, val );    
}

//! enables or disables the autoack on a pipe
//
//! \ingroup highlevel
//! This method enables or disables the auto acknowledgement
//! function on the specified pipe.
//!
//! pipe must be in the range 0..5. 
//! A pipe > 5 has the same effect as using pipe 5. 
static void pipe_autoack( unsigned char pipe, bool enabled ){
   unsigned char val = rfm73::register_read( RFM73_REG_EN_AA ); 
   if( pipe > 5 ){ 
      pipe = 5; 
   }   
   if( enabled ){
      val |= 1 << pipe;
   } else {
      val &= ~ ( 1 << pipe );
   }
   rfm73::register_write( RFM73_REG_EN_AA, val ); 
}

//! enables or disables a pipe
//
//! \ingroup highlevel
//! This method enables or disables the specified pipe.
//!
//! pipe must be in the range 0..5. 
//! A pipe > 5 has the same effect as using pipe 5. 
static void pipe_enable( unsigned char pipe, bool enabled ){
   unsigned char val = rfm73::register_read( RFM73_REG_EN_RXADDR ); 
   if( pipe > 5 ){ 
      pipe = 5; 
   }   
   if( enabled ){
      val |= 1 << pipe;
   } else {
      val &= ~ ( 1 << pipe );
   }
   rfm73::register_write( RFM73_REG_EN_RXADDR, val ); 
}

//! set the rfm73 pipe 0 address
//
//! \ingroup highlevel
//! This method sets the (up to 5 byte) receive address of pipe 0.
//! When the address length is set to less than 5 the excess
//! bytes are ignored, but address must still be a 5 byte array.
//!
//! Pipes 0 and 1 have a full (up to 5) byte address.
//! The other pipes (2..5) copy all but the least significant
//! (= first) byte from the pipe 1 address.
static void receive_address_p0( const unsigned char address[ 5 ] ){
   rfm73::buffer_write( RFM73_REG_RX_ADDR_P0, address, 5 );  
}

//! set the rfm73 pipe 1 address
//
//! \ingroup highlevel
//! This method sets the (up to 5 byte) receive address of pipe 1.
//! When the address length is set to less than 5 the excess
//! bytes are ignored, but address must still be a 5 byte array.
//!
//! Pipes 0 and 1 have a full (up to 5) byte address.
//! The other pipes (2..5) copy all but the least significant
//! (= first) byte from the pipe 1 address.
static void receive_address_p1( const unsigned char address[ 5 ] ){
   rfm73::buffer_write( RFM73_REG_RX_ADDR_P1, address, 5 );  
}

//! set the rfm73 pipe n (2..5) address
//
//! \ingroup highlevel
//! This method sets the least significant byte of 
//! the receive address of the pipe n.
//! The other bytes of the address are copied from pipe 1.
static void receive_address_pn( unsigned char channel, unsigned char address ){
   rfm73::register_write( RFM73_REG_RX_ADDR_P0 + channel, address ); 
}

//! set the payload size for pipe n
//
//! \ingroup highlevel
//! This method sets the size (= number of bytes, can be 1..32) 
//! for packets to be received  on pipe n. 
//! This setting must be the same as on the tranmitter.
//! A size of 0 will enable dynamic length packets.
//! A size > 32 will have the same effect as a size of 32.
static void channel_payload_size( unsigned char channel, unsigned char size ){
   unsigned char val;
   if( size > 32 ){
      size = 32;
   }
   val = rfm73::register_read( RFM73_REG_DYNPD );  
   if( size == 0 ){
      val |= 1 << channel;
   } else {
      val &= ~ ( 1 << channel );
   }    
   rfm73::register_write( RFM73_REG_DYNPD, val );
   rfm73::register_write( RFM73_REG_RX_PW_P0 + channel, size );  
}

//! set the rfm73 transmit address
//
//! \ingroup highlevel
//! This method sets the (up to 5 byte) address used
//! for all transmissions.
static void transmit_address( const unsigned char address[] ){
   rfm73::buffer_write( RFM73_REG_TX_ADDR, address, 5 );   
} 

//! report whether the transmit fifo is full
//
//! \ingroup highlevel
//! This method reads and reports whether the transmit fifo is full.
static bool transmit_fifo_full( void ){
  unsigned char s;
  s = rfm73::register_read( RFM73_REG_FIFO_STATUS );
  return ( s & FIFO_STATUS_TX_FULL ) != 0;
}

//! report whether the receive fifo is empty
//
//! \ingroup highlevel
//! This method reads and reports whether the receive fifo is full.
static bool receive_fifo_empty( void ){
  unsigned char s;
  s = rfm73::register_read( RFM73_REG_FIFO_STATUS );
  return ( s & FIFO_STATUS_RX_EMPTY ) != 0;
}

//! transmit a message
//
//! \ingroup highlevel
//! This method transmits the specified message.
//!
//! The specified length must be less than or equal to 
//! RFM73_MAX_PACKET_LEN (32).
//! Specifying a larger length has the same effect as
//! specifying a length of RFM73_MAX_PACKET_LEN.
//!
//! The retransmission setting (set by
//! the method rfm73_retransmit_delay_attempts) determines
//! whether the message is transmitted on the air just once
//! or repeatedly until an acknowledge is received. 
//! 
//! The RFM73 must be in transmit mode.
static void transmit_message(
   const unsigned char buf[],
   unsigned char length
){
   if( length > 32 ){
      length = 32;
   }
   rfm73::buffer_write( RFM73_CMD_W_TX_PAYLOAD, buf, length );
}
    
//! transmit a message once
//
//! \ingroup highlevel
//! This method transmits the specified message once.
//!
//! The specified length must be less than or equal to 
//! RFM73_MAX_PACKET_LEN (32).
//! Specifying a larger length has the same effect as
//! specifying a length of RFM73_MAX_PACKET_LEN.
//!
//! The message is transmitted on the air once, irrespective
//! of the retransmission setting. 
//! 
//! The RFM73 must be in transmit mode.
static void transmit_message_once(
   const unsigned char buf[],
   unsigned char length
){
   if( length > 32 ){
      length = 32;
   }
   rfm73::buffer_write( RFM73_CMD_W_TX_PAYLOAD_NOACK, buf, length );
}

//! get pipe number of the next message in receive FIFO
//
//! \ingroup highlevel
//! This method returns the number of the
//! pipe (0..5) on which the head message in the receive FIFO
//! was received.
//!
//! When the receive fifo is empty 0x07 is returned.
static unsigned char receive_next_pipe( void ){
   unsigned char status = rfm73::register_read( RFM73_REG_STATUS ); 
   return ( status >> 1 ) & 0x07;
}

//! get payload length of the next message in receive FIFO
//
//! \ingroup highlevel
//! This method returns length of the head message 
//! in the receive FIFO in bytes (1..32).
//! 
//! The RFM73 datasheet does not specify the value that is 
//! returned when the receive FIFO is empty
static unsigned char receive_next_length( void ){
   return rfm73::register_read( RFM73_CMD_R_RX_PL_WID );
}

//! (try to) receive a message
//
//! \ingroup highlevel
//! This method tries to receive a message.
//!
//! This method tries to retrieve a received message
//! from the receive FIFO. When no message is available
//! this method returns false. When a message is avaible
//! it is retrieved. The data is put in the buffer buf,
//! the length is written to length, and the method 
//! returns true.
//!
//! The size of the buffer buf must be at least
//! RFM73_MAX_PACKET_LEN (32).
//! 
//! The RFM73 must be in transmit mode.
static bool receive(
   unsigned char & pipe,
   unsigned char buf[],
   unsigned char & length
){
   unsigned char p = rfm73::receive_next_pipe();
   if( p == 0x07 ){
      return 0;
   }   
    pipe = p;
    length = rfm73::receive_next_length();
   rfm73::buffer_read( RFM73_CMD_R_RX_PAYLOAD, buf,  length );
   return 1;
}


}; // class rfm70

template< 
   typename radio, 
   typename led_tx,
   typename led_rx
>     
void hoperf_rfm73_demo(){   
   int transmit_led = 0, receive_led = 0;

   const unsigned char tx_buf[17]={
   0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
   0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x78 };

   led_tx::init();
   led_rx::init();
   
   led_tx::set( true ); 
   led_rx::set( true );
 
   radio::init();   
   radio::mode_receive(); 
   
   if( ! radio::is_present() ){
      bmptk::hardware::blink< 
	     bmptk::hardware::tee< led_tx, led_rx >, 
		 100 * bmptk::ms
      >();
   }

   led_tx::set( false );
   led_rx::set( false );
      
	unsigned char len,i,chksum, n = 0;
	unsigned char rx_buf[ radio::RFM73_MAX_PACKET_LEN ];
	while(1){
        bmptk::wait( 2 * bmptk::ms );

        led_tx::set( transmit_led > 0 );
        if( transmit_led > 0 ){ transmit_led--; }

        led_rx::set( receive_led > 0 );
        if( receive_led > 0 ){ receive_led--; }  
		     
        if(1) if( ++n > 250 ){
	       	radio::mode_transmit();
	         transmit_led = 20;	
	         if(0) radio::buffer_write( 
               radio::RFM73_CMD_W_TX_PAYLOAD_NOACK, tx_buf, 17 ); 
	         radio::transmit_message_once( tx_buf, 17 ); 
            bmptk::wait( 10 * bmptk::ms );	 	
            radio::mode_receive(); 
            n = 0; 	
        }

   	    if( ! radio::receive_fifo_empty() ){

		    len=radio::register_read( radio::RFM73_CMD_R_RX_PL_WID );
			if( len <= radio::RFM73_MAX_PACKET_LEN ){
				radio::buffer_read( radio::RFM73_CMD_R_RX_PAYLOAD,rx_buf,len );

 
	   	       chksum = 0;
		       for(i=0;i<16;i++){
			      chksum +=rx_buf[i]; 
		       }

		       if(chksum==rx_buf[16]&&rx_buf[0]==0x30){
		          receive_led = 20;
		       }	
			}
			radio::register_write( radio::RFM73_CMD_FLUSH_RX,0 );
      }
	}
}

}; // namspace chips
}; // namespace bmptk


//**************************************************************************
//
//! \page rfm73
//!
//! interface for the HopeRF RFM73 2.4 GHz transceiver module
//!
//! \image html RFM73-D.jpg
//! \image rtf RFM73-D.jpg
//!
//! \page Introduction
//!
//! The RFM73 module from HopeRF (www.hoperf.com) is a small and cheap
//! transceiver (transmitter + receiver) module for the license-free
//! 2.4 MHz frequency band. This module is intended for short range
//! communication, for instance within a room. 
//! Typical applications are wires mouses and keybords, 
//! and garage door openers.
//! In open air a distance of 
//! 50 .. 100 m is possible, but this is strictly line-of-sight: even 
//! the leaves of a single tree can obstruct the signal.
//!
//! The two main files in this libary, rfm73.h and rfm73.c,
//! are target independent. 
//! However, rfm73.c included by a file that must be provided
//! by the user. 
//! This file must provide macro's for initializing and accessing the
//! I/O pins that connect to the RFM73 module, and for
//! delays of a specified number of milliseconds and microseconds.
//! A template for this file is provided.
//!
//! \par Files
//! - rfm73.h
//! - rfm73.c
//! - rfm73-config.h
//!
//!
//! \par License
//!
//! The files in this library are licensed under the zlib license, 
//! which rougly says: when you redistribute the *source* you must 
//! keep the copyright notice intact. Otherwise you can use these files
//! any way you like, including getting rich from selling applications.
//! But don't complain to me when it does not work (OTOH, contructive
//! comments are appreciated!).
//!
//! \par ToDo: bugs, notes, pitfalls, todo, known problems, etc
//!
//! - pictures for power
//! - write 5 bytes when address <5 bytes?
//! - check and improve rtf documentation
//! - check CE cycling requirements
//! - descriptions for the other files
//! - add write ack payload for pipe N
//! - add 'report package has been sent'
//! - application: thermometer
//! - application: test signal strength
//! - application: asynch serial through the air
//!
//
//***************************************************************************//


//***************************************************************************//
//
//! \page overview RFM73 overview
//!
//! \image html smd-pins.jpg
//! \image rtf smd-pins.jpg
//!
//! The RFM73 is a small module (PCB with some components, ~ 18 x 26 mm) 
//! that implements a transciever for the license-free 2.4 GHz band. 
//! The module contains a small antenna on the PCB, 
//! there is no provision for an external antenna.
//! Two versions of the module exist, one with 
//! pins and one without. The pins are on a 1.28 mm grid, which is twice the
//! density of a standard DIL package. The module requires 1.9 .. 3.6 Volt
//! for its power, 3.3 Volt seems to be the preferred operating voltage. 
//! It is NOT designed to run at 5.0 Volt. 
//! According to the datasheet that maximum 'typical' operating current 
//! is 23 mA.
//!
//! Note that 'licence free' means that certain well-described use of this 
//! frequency band is allowd without a license. 
//! The RFM73 is designed to be compatible with the requirements for such 
//! use, but it is the responsibility of anyone who sells a product that
//! to make sure that it complies with such rules.
//!
//! The main component of an RFM73 module is the RF70 chip (hidden beneath
//! the black blob). The manufacturer provides separate datasheets for both 
//! the RF70 chip and the RFM73 module. 
//! The two datasheets seem to be the same, except for the physical 
//! and pinout information which is of course different for the chip
//! and the module, so you can probably ignore the RF70 chip datasheet.
//! The RFM73 module datasheet can currently be found at 
//! http://www.hoperf.com/upload/rf/RFM73.pdf
//!
//! The RFM73 module is intended for short-range communication,
//! like wireless computer peripherals (mouse, keyboard, tablet, ect.)
//! keyfobs (car opener, garage door opener, motorized fence opener - 
//! some cryptografy will probably be required for such applications)
//! and toys. In a line of sight situation a maximum range of 50 .. 100 m
//! is possible. Indoors communication within a single room will generally
//! be OK (unless you have a very large room..) but passing even a single
//! wall can be a problem.
//!
//! An RFM73 module must be used with a microcontroller that initializes
//! and operates the module. By itself the module can not be used as a
//! 'wireless wire', although such a mode could be implemented in the
//! microcontroller. The interface between the RFM73 and the microcontroller
//! is a standard 4-PIN SPI interface (MISO, MOSI, CLCK, CSN) plus a CE
//! (Chip Enable) pin. The module also provides an IRQ pin that could be used
//! to speed up the detection of certain events within the module. 
//! The library does not used this pin. 
//! The datahseet seems to claim that the SPI input pins are 5V-tolerant, 
//! but experiments have shown that this is not the case. 
//!
//! An RFM73 module operates in the 2.4 GHz band. 
//! Within that band a frequency can be selected in 1 MHz steps. 
//! RFM73 modules can communicate only when they use the same frequency.
//!
//! The RFM73 module operates on a packet basis. 
//! Each packet has a destination address.
//! By default the address is a 5 byte value, but a 4 or 3 byte
//! address can also be configured. 
//! For succesfull communication the RFM73 modules that are involved must
//! be configures for the same address length, and the receiving
//! module(s) must have one of their so-called pipes configured
//! for the address used by the transmitter.
//!
//! An RFM73 contains 6 so-called data pipes that can receive packages.
//! Each pipe has an address, and will receive only messages
//! for that address.
//! Pipes 0 and 1 have a full (up to 5 bytes) address. 
//! For pipes 2 .. 6 only the least significant byte can be
//! configured, the other bytes are copied from the address of pipe 1.
//! The length of the address (3, 4 or 5 bytes) is the same for 
//! transmission and for all 6 receive data pipes.
//!
//! A packet contains 1 .. 32 user data bytes. 
//! The packet length can either be fixed or flexible.
//! When the packet length is fixed each receiving pipe is configured
//! with its packet length. The length of a package that is sent is 
//! determined by the number of bytes written into the transmit buffer,
//! and it it must match the length configured for the receiving pipe.
//! When the packet length is flexible the length of a package is again
//! determined by the number of bytes written into the transmit, but in
//! this case this number is transmitted in the message, and on the
//! receiving side it can be retrieved by the R_RX_PL_WID command
//! (rfm73_channel_payload_size function).
//!
//! The simple way to send a package is without acknowledgement.
//! The RFM73 just transmits the package and considers it done.
//! It can be received by zero, one, or multiple RFM73 modules, but
//! the sending modules does not care and has no way of knowing.
//! For this simple mode of communication the involved RFM73's
//! - must be configured for the same channel frequency
//! - must use the same address length
//! - the receiving RFM73 must have a receive pipe configured
//!   for the same address as used by the transmitting RFM73
//!
//! Alternatively a package can be sent with acknowledgement and
//! (if necessary) retransmission. 
//! In this case the RFM73 will send the message, and consider it
//! done only when it receives an acknowledge for it.
//! If it does not receive an acknowledge within a fixed amount of
//! time it will re-transmit the package, up to a set maximum.
//! A receiving RFM73 can automatically send an acknowledgement
//! for a successfully received package. 
//! For this to work the same requirements as for simple unacknowledged
//! communicatiobn apply, and additionally on both RFM73's
//! - CRC must be enabled (length 1 or 2) on both modules
//! - receive data pipe 0 must be configured for the same address 
//!   as the module uses for transmitting
//!
//! The automatic retransmission provided by the RFM73 uses a fixed
//! retransmission timout, which is probly fine for correcting an 
//! occasional packet loss due to interference from other 2.4 GHz sources,
//! but it is potentially inefective when the interference is caused by
//! another RFM73 that uses the same timout! In such cases the 
//! microcontroller should implemnet its own retransmission scheme,
//! with for instance a random timeout, and maybe an exponential 
//! backoff.
//! 
//
//***************************************************************************//

//***************************************************************************//
//
//! \page hardware RFM73 hardware interface
//!
//! \image html smd-pins.jpg
//! \image rtf smd-pins.jpg
//!
//! The RFM73 uses a standard 4-wire SPI interface.
//! It also provides an active low interrupt pin, which could be used to
//! avoid polling. This library does not use the interrupt pin.
//! The RFM73 also has a CE (chip enable) input, which must be de-asserted
//! to put the chip in standby or power-down mode, and must be cycled
//! to switch between receive and transmit mode. Hence the interface
//! uses 5 data pins, plus ground and power (3.3V):
//! - GND : common ground
//! - VDD : 3.3V power
//! - CE : Chip Enable, active high, microcontoller output
//! - CSN : Chip Select Negative, active low, microcontoller output
//! - SCK : Serial ClocK, microcontoller output
//! - MOSI : Master Out Slave In, microcontoller output
//! - MISO : Master In Slave Out, microcontoller input
//! - IRQ : Interrupt ReQuest, not used
//!
//! When the microcontroller operates at 3.3 Volt (or lower, 
//! the RFM73 datasheet claims operation down to 1.9 Volt) all lines, 
//! including power,  
//! can be connected directly to the microcontroller. 
//! If you are experimenting and want to protect yourself against
//! problems when you accidentally configure the pin connected to MISO
//! as output, you could insert a suitable series resistor in this line.
//! 2k2 seems to be a good choice.
//!
//! When the microcontroller operates at 5 Volt there are three possible issues:
//! - power : the RFM73 operates at 1.9 .. 3.3 Volt, so the 5 Volt must somehow
//!   be reduced to a voltage within this range
//! - data from the microcontroller to the RFM73 : although the datasheet seems to
//!   claim that the inputs are 5V tolerant, this does not seem to be the case.
//!   You will have to convert the 5V outputs of your microcontroller down
//!   to 3.3V.
//! - data from RFM73 to the microcontroller : in most cases this will not 
//!   be a problem, but you might want to check the minimum voltage required
//!   by your microcontroller to reliably detect a logic 1. For instance a 
//!   PIC requires ~ 2.0 Volt on a TTL level input, but 0.8 * VDD on a
//!   Schmitt Trigger input! And you must consider this at the maximum VDD 
//!   for the microcontroller, which can be 5.5 Volt when delivered by 
//!   an 7805 or an USB port.
//!
//! There are various ways to create a 3.3 Volt supply for the RFM73 from a
//! 5 Volt supply. I prefer to use a low-drop low-quiecent
//! current 3.3 Volt linear regulator. Read the datasheet of the regulator
//! carefully: some put very stringent requirements on the value and impedance
//! of the decoupling capacitors. My favourite is the Microchip MCP1702, 
//! which has a low quiecent current (5 uA maximum), and is stable with 
//! only 1 uF input an output capacitors of any type 
//! (ceramic, tantalum, aluminium electrolytic).
//!
//! TBW: picture
//!
//! A crude way to create the power for the RFM73 is to use a 
//! resistor divider. I would do this only in an experimental
//! setup on my desk, never in a final product.
//! The source impedance of the divider causes a drop in the 
//! voltage when the RFM73 uses more current.
//! This drop can be reduced by lowering the resistor values, but at
//! the cost of a higher current through the resistors. The RFM73
//! can operate down to 1.9 Volt, but at that level the microcontroller 
//! might not reliably recognise a logic 1 from the RFM73. Another issue
//! is the dissipation in the resistors. The circuit below is a compromise.
//! It uses three equal-valued resistors because I don't stock many
//! different resistor values.
//! The idle current through the resistors is 83 mA at 5.5 Volt, in
//! this situation the RFM73 gets 3.7 Volt. That is slightly high, but
//! probably not a big problem.
//! When the RFM73 draws its maximum current of 23 mA when the 
//! microcontroller's power is at 4.5 Volt the RFM73 still gets 2.6 Volt. 
//! You might want to double-check that the microcontroller accepts ~ 2 Volt
//! as a logic 1. 
//!
//! TBW: picture
//! 
//
//***************************************************************************//


#endif
