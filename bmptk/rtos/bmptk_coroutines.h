// ==========================================================================
//
// file: bmptk/graphics/bmptk_coroutines.h
//
// ==========================================================================

#ifndef BMPTK_COROUTINES_H
#define BMPTK_COROUTINES_H


   ##########################################################################
   #
   # Stack allocation (no deallocation)
   #
   ##########################################################################

   def Stack_Allocation( self ):
      self.Documentation( """
         #article stacks Stacks
            
         A stack of a specified size can be allocated (but not deallocated).
      
         #interface mkt_stack_allocate
            allocate a stack, return 0 on failure
            #parameter size unsigned int 
               the size of the stack to be allocated (in bytes)
            #result S mkt_stack *
               the address representing the stack
   
        #interface mkt_stack_must_allocate
            allocate a block of memory, return 0 on failure
            #parameter size unsigned int 
                the size of the stack to be allocated (in bytes)
            #result S mkt_stack *
                the address representing the stack

         The routines mkt_stack_allocate and mkt_stack_must_allocate both
         try to allocate a stack of the size (in bytes) indicated 
         by the parameter. They return an address tha represents the stack.
         allocated block. 
         When a stack of the requested size can not be allocated
         mkt_stack_allocate will return 0, mkt_stack_must_allocate will
         call 
         #link mkt_fatal.

         #interface mkt_stack_size
            get the size of a stack
            #parameter N mkt_stack * 
               pointer to a stack that was previously allocated
            #result N unsigned int
               the size of the stack

        #interface mkt_stack_used
            get the number of bytes in a stack that were used
            #parameter N mkt_stack * 
               pointer to a stack that was previously allocated
            #result N unsigned int
               the number f bytes that were used

        #interface mkt_stack_unused
            get the number of bytes in a stack that were never used
            #parameter N mkt_stack * 
               pointer to a stack that was previously allocated
            #result N unsigned int
               the number f bytes that were never used
       
         In a stack the original size of the stack is stored, and the stack
         is filled with data. This makes it possible to get both
         the original size of a stack, and the number of bytes in the stack
         that were used or never used. This makes is possible to
         evaluate whether the size of a stack is reasonable.
      """ )

      self.C_Header( """
         typedef struct { } mkt_stack;
         mkt_stack * mkt_stack_allocate( int n );
         mkt_stack * mkt_stack_must_allocate( int n );
         int mkt_stack_size( mkt_stack * stack );
         int mkt_stack_unused( mkt_stack * stack );
         int mkt_stack_used( mkt_stack * stack );
         int mkt_stack_pointer( void );
         int mkt_stack_root( mkt_stack * stack );
         int mkt_stack_top( mkt_stack * stack );
      """ )
      
      self.C( """         
         mkt_stack * mkt_stack_allocate( int n ){
            int i;
            
            // round up to nearest multiple of 4
            n = ( n + 3 ) & ~ 0x03;
            
            // one word extra to store the size
            unsigned int *p = (unsigned int *) mkt_memory_allocate( n + 4 );
            
            if( p == 0 ){
               return 0;
            }
            
            // store size
            p[ n / 4 ] = n;
            
            // fill with 'magic' numbers
            for( i = 0; i < n / 4; i++ ){
               p[ i ] = i;
            }
            
            // a stack 'is' the 'size' entry
            unsigned int *x = p + ( n / 4 );
            return (mkt_stack *) x;
         }         
         
         mkt_stack * mkt_stack_must_allocate( int n ){
            mkt_stack * temp = (mkt_stack *) mkt_stack_allocate( n );
            if( temp == 0 ){
               mkt_fatal( mkt_fatal__memory_allocation_failure );               
            }
            return temp;         
         }
         
         int mkt_stack_size( mkt_stack * stack ){
            int *p = (int *) stack;
            return *( p );
         }

         int mkt_stack_unused( mkt_stack * stack ){
            int *p = (int *) stack;
            int intsize = *( p ) / 4;
            int n;
            p -= intsize;
            for( n = 0; n < intsize; n++ ){
               if( p[ n ] != n ){
                  return 4 * n;
               }
            }
            return 4 * intsize;
         }
         
         int mkt_stack_used( mkt_stack * stack ){
            return mkt_stack_size( stack ) - mkt_stack_unused( stack );
         }
         
         int mkt_stack_root( mkt_stack * stack ){
            return ((int) stack ) - (4 + * (int*) stack );
         }
         
         int mkt_stack_top( mkt_stack * stack ){
            return (int) stack;
         }
         
      """ )            
       
      self.Assembler( """
         mkt_subroutine mkt_stack_pointer            
            mov r0, sp
            # mov r0, #15
            mov pc, lr
      """ )


   ##########################################################################
   #
   # Coroutines
   #
   ##########################################################################

   def Coroutines( self ):
      self.Documentation( """
         #article coroutines Coroutines
            
         #interface mkt_coroutine_create
            create a coroutine
            #parameter f (void *)(void)
               a function (no parameters, no return) 
               that is the body of the coroutine
            #parameter n unsigned int
               the size of the stack to be allocated for the
               execution fo the coroutine
            #result co mkt_coroutine *
               the size of the stack to be allocated for the
               execution fo the coroutine

         A coroutine can be created. 
         This requires a routine or function that contains the 
         code of the coroutine (the body), and a size for the stack. 
         The result is a pointer to an (allocated) 
         coroutine structure.
         When the coroutine structure or the stack can not be allocated
         this routine will return 0.

         The similar routine mkt_coroutine_must_create will call
         #link mkt_fatal
         when the coroutine structure or the stack 
         can not be allocated.

         #interface mkt_coroutine_resume
            resumes execution of a coroutine
            #parameter co mkt_coroutine *
               pointer to the coroutine to be resumed

         mkt_coroutine_resumes resumes execution of the passed coroutine.
         Execution of the current coroutine is suspended untill it
         is resumed by an mkt_coroutine_resume call.

         The main can be resumed by calling 
         #code
         mkt_coroutine_resume( mkt_coroutine_main );
         #end code 
         
         When the subroutine or function that is the body of a coroutine
         returns 
         #link mkt_fatal
         is called (just like when the main() returns).

         For C++ a simple coroutine class is available.
         #code
            class mkt_coroutine_class {
               public:
                  mkt_coroutine_class( 
                     void(*m)(void),
                     int n
                  ){
                     data = mkt_coroutine_must_create( m, n );
                  }
                  
                  void resume( void ){
                     mkt_coroutine_resume( data );    
                  }
                  
                  mkt_stack * stack( void ){
                     return data->stack;
                  }      
            
               private:
                  mkt_coroutine * data;
            };
         #end code
      """ )

      self.C_Header( """

         typedef struct {
            mkt_stack     * stack;
            unsigned int  * _sp;
         } mkt_coroutine;
         
         extern mkt_coroutine mkt_coroutine_main_struct;
         #define mkt_coroutine_main ( & mkt_coroutine_main_struct )
         extern mkt_coroutine *mkt_coroutine_running;
         
         mkt_coroutine * mkt_coroutine_create( 
            void(*m)(void),
            int n
        );
         
         mkt_coroutine * mkt_coroutine_must_create( 
            void(*m)(void),
            int n
        );
         
         void mkt_coroutine_resume( 
            mkt_coroutine *p 
         );
         
      """ )
      
      self.C( """         
         mkt_coroutine mkt_coroutine_main_struct = { 0, 0 };
         mkt_coroutine *mkt_coroutine_running = mkt_coroutine_main;
         extern mkt_c_linking void mkt_return_from_coroutine( void );

         extern mkt_c_linking void mkt_coroutine_switch_context_to( 
            unsigned int ** store_sp, 
            unsigned * new_sp 
         );
                  
         mkt_coroutine * mkt_coroutine_create( 
            void(*m)(void),
            int n
         ){
            mkt_coroutine *p = (mkt_coroutine *) 
               mkt_memory_allocate( sizeof( mkt_coroutine ));
            if( p == 0 ){ return 0; }

            p->stack = mkt_stack_allocate( n );
            if( p->stack == 0 ){ return 0; }

            ((unsigned int * )p->stack)[ -1 ] = ( unsigned int ) m;
            
            // if I told you why 12 I'd have to kill you ;)
            p->_sp = ((unsigned int * )p->stack) - 12;

            return p;
         }
         
         mkt_coroutine * mkt_coroutine_must_create( 
            void(*m)(void),
            int n
         ){
            mkt_coroutine *p = (mkt_coroutine *) 
               mkt_memory_must_allocate( sizeof( mkt_coroutine ));

            p->stack = mkt_stack_must_allocate( n );

            ((unsigned int * )p->stack)[ -1 ] = ( unsigned int ) m;
            
            // if I told you why 12 I'd have to kill you ;)
            p->_sp = ((unsigned int * )p->stack) - 12;

            return p;
         }
         
         void mkt_coroutine_resume( 
            mkt_coroutine *p 
         ){
            mkt_coroutine *old = mkt_coroutine_running;
            mkt_coroutine_running = p;
            mkt_coroutine_switch_context_to( &old->_sp, p->_sp );
         }          
         
      """ )            

      self.Assembler( """         
      
         // void mkt_coroutine_switch_context_to( 
         //    void ** store_sp, 
         //    void * new_sp 
         // );
         
            mkt_subroutine mkt_coroutine_switch_context_to
            
            // save old C context
            stmfd   sp!, { r2-r12, lr }
               
            // remember current (old) sp
            str sp, [ r0 ]
            
            // set new sp
            mov sp, r1
            
            // this is for the coroutine's 'main' only
            ldr lr, =mkt_return_from_coroutine
                  
            // install new C context
            ldmfd   sp!, { r2-r12, pc }
                  
      """ )            
      
      self.CPP_Header( """
         class mkt_coroutine_class {
            public:
               mkt_coroutine_class( 
                  void(*m)(void),
                  int n
               ){
                  data = mkt_coroutine_must_create( m, n );
               }
               
               void resume( void ){
                  mkt_coroutine_resume( data );    
               }
               
               mkt_stack * stack( void ){
                  return data->stack;
               }      
         
            private:
               mkt_coroutine * data;
         };

      """ )
	  
#endif BMPTK_COOROUTINES_H