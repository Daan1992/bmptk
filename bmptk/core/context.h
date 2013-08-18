//***************************************************************************
//
// file : bmptk/core/context.h
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

namespace std { class ostream; };

namespace bmptk {

//! an execution context (used by the RTOS)
class context {

   //! the first (lowest) address of the stack space
   unsigned char *bottom;
   
   //! the initial SP (Stack Pointer)
   //
   //! This is the initial value to be loaded into the SP register.
   //! Whether the stack grows up or down, and whether *sp is
   //! part of the stack or not is target dependent.
   //!
   //! Cortex targets use an 8-byte aligned full decending stack: 
   //! the stack grows down (from higher addresses to lower addresses), 
   //! the stack pointer always points to an occupied entry, and
   //! the stack must be 8-byte aligned.
   unsigned char *sp;
   
   friend std::ostream & bmptk::operator<< ( std::ostream &s, const context &st );
   
public:

   //! used only to construct the main context
   constexpr context( unsigned char *bottom, unsigned char *size );
   
   //! the size of the stack
   const unsigned int stack_size;
   
   //! construct a context: specify its body and stack size
   //
   //! This constructor creates an exceution context, from
   //! the body function (the code to be executed by the context)
   //! and the stack size. To start executing within the context

   context( void body(), unsigned int stack_size );
   
   //! check how much of the stack is still free
   //
   //! This function checks how much of a stack has
   //! never been used.
   //! When an stack is alloacted its is filled with the pattern
   //! 0xDEAD. This function checks how much of this pattern
   //! is left, to determine how much of the stack is still
   //! untouched. 
   //!
   //! Note that this method can underestimate the stack use when
   //! the task only reads from (not writes tp) memory. This is rare,
   //! but when in doubt make your stack somewhat larger than seems
   //! to be required.   
   unsigned int stack_unused() const;  
   
   //! check how much of the stack has ever been used
   //
   //! This function checks how much of a stack has
   //! ever been used.
   //! When an stack is alloacted its is filled with the pattern
   //! 0xDEAD. This function checks how much of this pattern
   //! is left, to determine how much of the stack is still
   //! untouched.      
   //!
   //! Note that this method can underestimate the stack use when
   //! the task only reads from (not writes tp) memory. This is rare,
   //! but when in doubt make your stack somewhat larger than seems
   //! to be required.
   unsigned int stack_used() const {
      return stack_size - stack_unused();
   }   
   
   //! resume execution of this context
   //
   //! This method resumes (or starts) execution of this context.
   //! The execution starts at the begiining of the body function
   //! provided to the constructor. When it is suspended (by
   //! calling the resume() of another context) and then resumed,
   //! execution resumes after the resume() call.
   //! When the body function terminates the the main is resumed.
   //! (And when terminated context is resumed main is resumed again).
   void resume();
};

//! the context of the main()
//
//! This context can be used to resume the main,
//! after it has yielded execution to another context.
extern context main_context;

//! the current exceuting context
//
//! This function returns thye currently executing context
//! (initially this is of course &main_context).
context * current_context();
   
//! prints a context
//
//! \relates bmptk::context
//! This operator prints a line of information about a context.
//! This is meant for debugging purposes only.
std::ostream & operator<< ( std::ostream &s, const context &st );

}; // namespace bmptk; 
