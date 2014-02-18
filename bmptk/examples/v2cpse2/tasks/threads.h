#ifndef _threads_
#define _threads_

#define STACK_SIZE 100

typedef struct thread  {
   int sp;   
   int stack[ STACK_SIZE ];
   struct thread * next;
   int wakeup_time;
} thread;

void thread_init( thread *t, void f( void ));
void wait( int d );
void rtos();

#endif
