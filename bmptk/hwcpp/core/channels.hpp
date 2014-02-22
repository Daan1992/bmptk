// ==========================================================================
//
// File      : channels.hpp
// Part of   : hwcpp library (www.voti.nl/hwcpp)
// Copyright : wouter@voti.nl 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

namespace hwcpp {

   struct channel_in_archetype {
      typedef void has_channel_in;
      static void init();
      static bool get_will_block();
      static char get();
   };   

   struct channel_out_archetype {
      typedef void has_channel_out;
      static void init();
      static bool put_will_block();
      static void put( char c );
   };   

   struct channel_in_out_archetype :
      public channel_in_archetype,
      public channel_out_archetype
   {};
   
   struct channel_sink :
      public channel_out_archetype
   {
      static void init(){}
      static bool put_will_block(){ return false; }
      static void put( char c ){}
   };
   
   template< 
      class channel, 
      unsigned int buffer_size,
      class interval
   >
   class channel_out_buffer :
      public channel_out_archetype 
   {
   private:
   
      static char buffer[ buffer_size ];
      static int read_index;
      static int write_index;
      static int n_buffered;   
      
   public:
   
      static int overflow_count;
      
      static void init(){
         read_index = 0; 
         write_index = 0;
         overflow_count = 0;
         n_buffered = 0;      
      }
      
      static void put( char c ){
         if( n_buffered < buffer_size ){
            buffer[ write_index ] = c;
            if( ++write_index == buffer_size ){
               write_index = 0;
            }   
            n_buffered++;
         } else {
            overflow_count++;
         }
         poll();
      }
   
      static bool put_will_block(){
         return false;
      }        
      
      static void poll(){
         if( ( n_buffered > 0 ) && ! channel::put_will_not_block() ){
            channel::put( buffer[ read_index ] );
            if( ++read_index == buffer_size ){
               read_index = 0;
           }
           n_buffered--;
         }
      }        
   
   };   
   
}; // namespace hwcpp