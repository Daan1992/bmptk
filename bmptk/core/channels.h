//***************************************************************************
//
// file : bmptk/core/channels.h
//
// LICENSE (MIT expat license, copy of bmptk/license.txt)
//
// Copyright (c) 2012 Wouter van Ooijen (wouter@voti.nl)
// 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE..
//
//***************************************************************************

// document everything in this file
/*! \file */

namespace bmptk {

//! interface for a polled output channel
template< class t >
class output_channel_polled {
public:

   //! check whether a put() call will not block
   virtual bool put_will_not_block() = 0;
   
   //! put one data item
   //
   //! When the channel is not ready to receive a data item
   //! this call will block until the channel is ready.
   //! put_will_not_block() can be called first to avoid
   //! blocking.
   virtual void put( t ) = 0;
};

//! interface for a polled input channel
template< class t >
class input_channel_polled {
public:

   //! check whether a get() call will not block   
   virtual bool get_will_not_block() = 0;
   
   //! get one data item
   //
   //! When the channel is not ready to deliver a data item
   //! this call will block until the channel is ready.
   //! get_will_not_block() can be called first to avoid
   //! blocking.
   virtual t get() = 0;
};

//! interface for a polled input/output channel
template< class t >
class input_output_channel_polled :
   public output_channel_polled< t >, input_channel_polled< t >{};     
   
//! a buffered output channel
//
//! This class provides buffering for an output channel.
//! Up to buffer_size data items will be buffered before the 
//! buffered channel will block.    
template< class t, int buffer_size>
class output_channel_polled_buffer : public output_channel_polled< t > {
private:
   output_channel_polled< t > &output;
   t buffer[ buffer_size ];
   int read_index;
   int write_index;
   int overflows;
   int n_buffered;
public:

   //! construct a buffer on top of an output channel
   output_channel_polled_buffer(
      output_channel_polled< t > &output
   ): 
      output( output ),
      read_index( 0 ), 
      write_index( 0 ), 
      overflows( 0 ), 
      n_buffered( 0 )
   {}

   //! put a data item
   void put( t c ){
      if( n_buffered < buffer_size ){
         buffer[ write_index ] = c;
         if( ++write_index == buffer_size ){
            write_index = 0;
         }   
         n_buffered++;
      } else {
         overflows++;
      }
   }
   
   //! report whether the channel is ready to accept a data item
   bool put_will_not_block(){
      return true;
   }   

   //! poll the channel
   //
   //! This method will do the writing from the buffer to the underlying
   //! channel. It must be called 'regularly' to keep this activity going.
   void poll(){
      if( ( n_buffered > 0 ) && output.put_will_not_block() ){
         output.put( buffer[ read_index ] );
         if( ++read_index == buffer_size ){
            read_index = 0;
         }
         n_buffered--;
      }
   }   
};    

	  	  
}; // namespace bmptk; 