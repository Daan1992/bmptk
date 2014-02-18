
	.cpu cortex-m0
   .global switch_to
   .text
   .align 2
   
   // void switch_to( task * t );
switch_to:

   // save current context on the stack
   push { r4 - r7, lr }
   mov r1, r8
   mov r2, r9
   mov r3, r10
   mov r4, r11
   mov r5, r12
   push { r1 - r5 }
   
   // save sp in current_thread->sp
   ldr r1, = current_thread
   ldr r2, [ r1 ]
   mov r3, sp
   str r3, [ r2 ]
   
   // current_thread = t
   str r0, [ r1 ]
   
   // sp = t->sp
   ldr r3, [ r0 ]
   mov sp, r3
   
   // restore the new context from the stack
   pop { r1 - r5 }
   mov r12, r5
   mov r11, r4
   mov r10, r3
   mov r9, r2
   mov r8, r1
   pop { r4 - r7, pc }

   

