   .thumb
   .syntax unified
   .global _resume, _trampoline

.section .text

   // extern "C" _resume( void ** old_sp, void ** new_sp );
   // R0 points to the location where the old stack pointer must be stored
   // R1 points to the location where the new stack pointer can be retrieved
   // (They can be the same location!)
   .align 4
_resume:
   mov   r2, lr        // save lr
   push  { r2 }

   push  { r3-r7 }     // save r3-r7
   mov   r3, r8
   mov   r4, r9
   mov   r5, r10
   mov   r6, r11
   mov   r7, r12
   
   push  { r3-r7 }     // save r8-r12
   
   mov   r3, sp        // save the old stack pointer
   str   r3, [ r0 ]    
   
   ldr   r0, [ r1 ]    // install the new stack pointer
   mov   sp, r0
   
   pop   { r3-r7 }     // restore r8-r12
   mov   r8, r3
   mov   r9, r4
   mov   r10, r5
   mov   r11, r6
   mov   r12, r7
   
   pop   { r3-r7 }     // restore r3-r7
   
   pop   { r1 }        // return to the caller (of the new stack)
   ldr   r0, =1
   orrs  r1, r1, r0
   bx    r1    

   // called with two function pointers on the stack:
   // the body of the context, and resume_main()
.align 4
_trampoline:
   pop   { r0 }        // call the body of the context
   blx   r0   
forever:   
   pop   { r0 }        // when the body returns: keep
   push  { r0 }        // calling resume_main()
   blx   r0
   b     forever   
