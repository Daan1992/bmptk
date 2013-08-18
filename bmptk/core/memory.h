//***************************************************************************
//
// file : bmptk/core/memory.h
//
// LICENSE (MIT expat license, copy of bmptk/license.txt)
//
// Copyright (c) 2013 Wouter van Ooijen (wouter@voti.nl)
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

extern "C" {

//! allocate memory
//
//! Allocate a block of n bytes from the heap, and return a pointer
//! to the first byte. 
//!
//! On hosted targets the malloc from the host system is used.
//!
//! On embedded targets, the block will be 8-byte aligned.
//!
//! On an embedded system this call will never return 0: when
//! the requested amount of memory is not available 
//! fatal_error_detected( "heap too small" ) will be called.
void *malloc( unsigned int size );

};

namespace bmptk {

//! report the number of free bytes on the heap
//
//! This function will probably not give a meaningfull result
//! on a hosted environment (for instance windows).
unsigned int heap_free();

}; // namespace bmptk; 
