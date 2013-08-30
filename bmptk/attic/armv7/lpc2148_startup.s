#include "bmptk_lpc2148.h"
         //==================================================================
         //
         // ARM exception vectors
         //
         // The "vectors" section is mapped to a 64-byte memory area that is 
         // handled in a special way: when running from ROM its content is 
         // first copied to RAM and next that RAM area is remapped to 
         // address 0. 
         // This makes it possible for a running program to change the 
         // exception (interrupt) vectors
         //
         //==================================================================
         
         // this section is handled in a special way by the link script
         .section vectors, "awx"
         .arm
         .align

         .global _start
         _start:
         
         // the ARM exception vectors, at physical address 0
         ldr     pc, mkt_reset_vector
         ldr     pc, mkt_undefined_instruction_vector
         ldr     pc, mkt_software_interrupt_vector
         ldr     pc, mkt_prefetch_abort_vector
         ldr     pc, mkt_data_abort_vector
         ldr     pc, mkt_reserved_vector
         .global irq_vec
         irq_vec:
         b _mkt_IRQ_interrupt_handler
         // ldr     pc, mkt_IRQ_vector         
         ldr     pc, mkt_FIQ_vector
      
         .global mkt_reset_vector
         .global mkt_undefined_instruction_vector
         .global mkt_software_interrupt_vector
         .global mkt_prefetch_abort_vector
         .global mkt_data_abort_vector
         .global mkt_reserved_vector
         .global mkt_IRQ_vector
         .global mkt_FIQ_vector
      
         //==================================================================
         //
         // The following vectors are variables that contain the addresses 
         // the various execptions will jump to. Change the content of a 
         // vector to redirect the corresponding exception.
         //
         //==================================================================
      
         mkt_reset_vector:                    .word mkt_reset_handler
         mkt_undefined_instruction_vector:    .word mkt_undefined_instruction_handler
         mkt_software_interrupt_vector:       .word mkt_software_interrupt_handler
         mkt_prefetch_abort_vector:           .word mkt_prefetch_abort_handler
         mkt_data_abort_vector:               .word mkt_data_abort_handler
         mkt_reserved_vector:                 .word mkt_reserved_handler
         // mkt_IRQ_vector:                      .word unexp_exc_handler_6
         mkt_IRQ_vector:                      .word _mkt_IRQ_interrupt_handler
         mkt_FIQ_vector:                      .word mkt_FIQ_handler
         
         //==================================================================
         //
         // default unexpected exception handlers
         //
         // These handlers just load a unique value into R0 and jumps 
         // to the mkt_fatal_error handler.
         //
         //==================================================================
      
                    // start of a section of assembler code that can be placed in ROM
            .macro mkt_code
               .text
               .align
               .arm
            .endm   
                     // start of a section for read/write, 
            // explicitly initialised data
            .macro mkt_data
               .data
               .align
            .endm
            
            .macro mkt_uninitialized
               .section .uninitialized
               .align
            .endm                  
                     // start of a section for read/write, 
            // 0-initialised data
            .macro mkt_bss
               .bss
               .align
            .endm
                     // start of a section for read-only, initialised data
            .macro mkt_rodata
               .text
               .align
            .endm
 
         
         mkt_undefined_instruction_handler: 
            .weak mkt_undefined_instruction_handler
            nop // mkt_FATAL mkt_fatal__unhandled_undefined_instruction
            
         mkt_software_interrupt_handler: 
            .weak mkt_software_interrupt_handler
            nop // mkt_FATAL mkt_fatal__unhandled_software_interrupt
            
         mkt_prefetch_abort_handler: 
            .weak mkt_prefetch_abort_handler
            nop // mkt_FATAL mkt_fatal__unhandled_prefetch_abort
            
         mkt_data_abort_handler: 
            .weak mkt_data_abort_handler
            nop // mkt_FATAL mkt_fatal__unhandled_data_abort
            
         mkt_reserved_handler: 
            .weak mkt_reserved_handler
            nop // mkt_FATAL mkt_fatal__unhandled_reserved
            
         unexp_exc_handler_6: 
         nop // mkt_FATAL mkt_fatal__unhandled_IRQ
         
         mkt_FIQ_handler: 
            .weak mkt_FIQ_handler
            nop // mkt_FATAL mkt_fatal__unhandled_FIQ 
         
         mkt_reset_handler:
         
               //==================================================================
         //
         // set stacks
         //
         // note: Early ARM ARM erroneously stated that msr cpsr_c, <imm> 
         // is not an allowed form, and this error can still be found for 
         // instance in Furber's book. 
         // ref: http://www.arm.com/support/faqdev/1472.html
         //
         //==================================================================
         
         mkt_uninitialized
            .global _mkt_first_free
            _mkt_first_free:  .space 4
            
         mkt_code   
      
         // get first free location
         ldr     r6, = _mkt_first_free
         ldr     r5, = mkt_ln_bss_beyond;
         // ldr     r5, [ r6 ]
         str     r5, [ r6 ]
      
         #define MKT_FIQ_STACK_SIZE 2048   
         #define MKT_IRQ_STACK_SIZE 2048   
         #define MKT_MAIN_STACK_SIZE 2048            
         #define ROUND_UP( N, M ) ((N + (M-1)) & ~ (M-1)) 
         
         #ifdef MKT_FIQ_STACK_SIZE

            // select FIQ mode
            msr     cpsr_c, #0x11
            nop

            // claim stack area
            add     r5, r5, #ROUND_UP( MKT_FIQ_STACK_SIZE, 4 )
            
            // set stack pointer
            mov     sp, r5

         #endif

         #ifdef MKT_IRQ_STACK_SIZE

            // select IRQ mode
            msr     cpsr_c, #0x12
            nop

            // claim stack area
            add     r5, r5, #ROUND_UP( MKT_IRQ_STACK_SIZE, 4 )
           
            // set stack pointer
            mov     sp, r5

         #endif

         // select supervisor mode
         msr     cpsr_c, #0x13
         nop

         // select supervisor mode
         //msr     cpsr_c, #0xD3
         //nop
   
         // claim stack area
         add     r5, r5, #ROUND_UP(MKT_MAIN_STACK_SIZE, 4 )

         // set stack pointer
         mov     sp, r5  
         
         // update the 'free ram' pointer
         str     r5, [ r6 ]
               //==================================================================
         // clear BSS (non-initialized variables)
         //==================================================================

         mov     r0, #0
         ldr     r1, = mkt_ln_bss_first
         ldr     r2, = mkt_ln_bss_beyond
      clear_bss_loop:
         cmp     r1, r2
         beq     clear_bss_beyond
         str     r0, [ r1 ], #4
         b       clear_bss_loop
      clear_bss_beyond:
               //==================================================================
         // set clocks and related timing
         //==================================================================

         // first disable MAM
         ldr     r1, =MAMCR
         mov     r0, #0
         str     r0, [ r1 ]

         // Set MAMTIM to appropriate rate  
         ldr     r1, = MAMTIM
         #if mkt_cclk < 20 * MHz
            mov     r0, #1
         #elif mkt_cclk < 40 * MHz
            mov     r0, #2
         #else
            mov     r0, #3
         #endif
         mov     r0, #3
         str     r0, [ r1 ]
   
         // Set MAM fully enabled, as far as possible
         ldr     r1, =MAMCR
         #if mkt_chip == mkt_lpc2148
            // http://www.standardics.nxp.com/
            //    support/documents/microcontrollers/pdf/errata.lpc2148.pdf
            // MAM.2 : Under certain conditions in MAM Mode 2 code execution 
            //            out of internal Flash can fail
            // Note that the advice conflicts with MAM.1 ...
            mov     r0, #1
         #else
            mov     r0, #2
         #endif
         mov     r0, #0
         str     r0, [ r1 ]
      
         // set VPB to the MK_VPBDIV as calculated from the configuration 
         mov     r0, #0
         ldr     r1, = VPBDIV
         str     r0, [ r1 ]

         // alias for chips with only one PLL
         #ifndef PLL0FEED
            #define PLL0FEED PLLFEED
         #endif
      
         
            // set main PLL
            
            // xtal=12000000, cclk=60000000
            // M=5, P=2
            ldr     r0, =PLL0CFG
            ldr     r1, =0x00000044
            str     r1, [ r0 ]
            
            ldr     r0, =PLL0CON
            ldr     r1, =1
            str     r1, [ r0 ]
            
         // feed the PLL
         ldr     r0, =PLL0FEED
         mov     r1, #0xAA
         str     r1, [ r0 ]
         mov     r1, #0x55
         str     r1, [ r0 ]
      
            
         pll_init_wait:
            ldr     r0, =PLL0STAT
            ldr     r1, [ r0 ]
            ands    r1, r1, #( 1 << 10)
            beq     pll_init_wait
            
            ldr     r0, =PLL0CON
            ldr     r1, =3
            str     r1, [ r0 ]           
            
         // feed the PLL
         ldr     r0, =PLL0FEED
         mov     r1, #0xAA
         str     r1, [ r0 ]
         mov     r1, #0x55
         str     r1, [ r0 ]
      
            
                  
            
            
         //==================================================================
         //
         // copy the vectors to SRAM
         //
         // when running from RAM is is not needed because the vectors
         // are already at the correct place in RAM 
         //
         //==================================================================
      
         #if 0
         #if mkt_memory == mkt_rom           
               ldr     r1, = mkt_ln_vectors_rom_first
               ldr     r2, = 0x40000000
               ldr     r3, = mkt_ln_vectors_rom_beyond
            copy_vectors_loop:
               ldr     r0, [ r1 ], #4
               str     r0, [ r2 ], #4
               cmp     r3, r1
               bne     copy_vectors_loop         
         #endif
         #endif
         
         //==================================================================
         //
         // remap the first 64 bytes of RAM (the vector block) 
         // to appear (also) at 0x00
         //
         //==================================================================

         #if 0
         #ifdef MEMMAP
            ldr     r0, = MEMMAP
            mov     r1, #10 
            str     r1, [ r0 ]
         #endif
         #endif
         
         //==================================================================
         //
         // select fast or slow GPIO access
         //
         //==================================================================
      
         #ifdef SCS
            #if mkt_fast_gpio == 1
               mov     r0, #3
            #else
               mov     r0, #0
            #endif      
            ldr     r1, =SCS
            str     r0, [ r1 ]      
         #endif
         
            
         //==================================================================
         // call global initializers
         //==================================================================
         
         // b ctors_done
         ldr     r10, =__ctors_start__
         ldr     r11, =__ctors_end__
      ctors_loop:   
         cmp     r10, r11
         beq     ctors_done
         ldr     lr, =ctors_loop
         ldr     pc, [ r10 ], #4
      ctors_done:   

         //==================================================================
         // call main (= the application)
         //==================================================================

         mov     r0, #0   // no arguments
         mov     r1, #0   // no argv either
         bl      main   
         
         .global mkt_return_from_coroutine
      mkt_return_from_coroutine:         
         
         // main should never return
         nop // mkt_fatal mkt_fatal__return_from_main
forever:
         b forever
         
               //==================================================================
         //
         // default IRQ interrupt handler
         //
         // This handler will call the C function that is stored in the
         // VICVectAddr for the interrupt that ocurred. This must be a 
         // *normal* C function, without interrupt attribute.
         //
         //==================================================================

        .text
        .code 32
        .align
 
      
      _mkt_IRQ_interrupt_handler:
         stmfd   sp!, {r0-r12,lr}       // store all regs
         
         ldr     lr, =_mkt_IRQ_interrupt_handler_resume
         ldr     r0, =VICVectAddr       // get handler
         ldr     pc, [ r0 ]             // jump to handler
         
      _mkt_IRQ_interrupt_handler_resume:      
         ldr     r1, =VICVectAddr        
         str     r0, [r1]               // must update priority hardware
      
         ldmfd   sp!, {r0-r12,lr}
         subs    pc, lr, #4             // the way IRQ handler should return  
         
     mkt_disable_interrupts:
         stmfd           sp!, { r0, lr }
         mrs             r0, CPSR
         orr             r0, r0, #0xC0   // disable IRQ/FIQ interrupts
         msr             CPSR_c, r0
         ldmfd           sp!, { r0, pc }
      
      mkt_enable_interrupts:
         stmfd           sp!, { r0, lr }
         mrs             r0, CPSR
         bic             r0, r0, #0x80   // enable IRQ interrupts
         msr             CPSR_c, r0
         ldmfd           sp!, { r0, pc }         
        
               //==================================================================
         // fatal error handler
         //==================================================================
      
         #if mkt_error_handling != 0
           
            mkt_data
            
               // overwrite this variable with the address 
               // of your error handler function, but don't assume 
               // too much about the state of the system!
               .global mkt_user_error_vector
               mkt_user_error_vector:  .word mkt_fatal_error
      
            mkt_fatal_error:
         
               // Insight automatically sets a breakpoint at 'exit'
               // so why not put that label here so a fatal error is caught
               exit: 
         
               // When then debugger stops at this breakpoint a fatal error
               // has occurred during execution. 
               // R0 contains the error code.
               // You can check the stack to see where the error ocurred.
               ldr     r1, =mkt_user_error_vector
               ldr     pc, [ r1 ]           
            
         #endif
               mkt_stack_pointer:
            mov r0, sp
            # mov r0, #15
            mov pc, lr
               
      
         // void mkt_coroutine_switch_context_to( 
         //    void ** store_sp, 
         //    void * new_sp 
         // );
         
            mkt_coroutine_switch_context_to:
            
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
                  

      
