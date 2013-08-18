//***************************************************************************
//
// file : bmptk/core/rtos.cpp
//
// LICENSE (MIT expat license, copy of bmptk/license.txt)
//
// Copyright (c) 2012 
//   Wouter van Ooijen (wouter@voti.nl)
//   Marten Wensink
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

#include "bmptk.h"

namespace bmptk {
   extern void (*wait_until_handler)( time t );
};

using namespace std;
namespace bmptk {
namespace rtos {


//***************************************************************************
//
// debugging & logging
//
//***************************************************************************

/*
const bool task_logging            = (global_logging && 0);
const bool debug_task_logging      = (global_logging && 0);

char task_state( const task * t ){
   if( t->is_blocked() ){
      return 'B';
   } else {
      return 'R';
   }      
}

const char * task_name( const task * t ){
   if( t ){
      return t->name();
   } else {
      return "-";
   }      
}

#define task_trace \
   if (task_logging) \
      trace \
         <<  name() \
         << "     " << task_state( this ) \
         << " "

#define task_debug_trace \
   if (debug_task_logging) \
      trace \
         <<  name() \
         << "     " << task_state( this ) \
         << " "
*/
         
int nr_from_mask( unsigned int mask ){
   for( int i = 0; i < 32; i++ ){
      if( mask & ( 1 << i )){
         return i;
     }
   }
   return -1;
}
         

//***************************************************************************
//
// string cloning
//
//***************************************************************************

unsigned int xstrlen( const char *s ){
   unsigned int n = 0;
   while( *s++ != '\0' ){
      n++;
   }
   return n;
}

void xstrcpy( char *d, const char *s ){
   for(;;){
      *d = *s;
      if( *d == '\0' ){
         return;
      }
      s++;
      d++;
   }
}

const char * string_clone( const char *name ){
   #if BMPTK_RTOS_STATISTICS_ENABLED
      char * result = new char[ 1 + xstrlen( name ) ];
      xstrcpy( result, name );
      return result;
   #else
      return "";   
   #endif 
}


//***************************************************************************
//
// linked lists
//
// These lists are in no particular order
//
//***************************************************************************

#if BMPTK_RTOS_STATISTICS_ENABLED

flag *flags = 0;
timer *timers = 0;
clock * clocks = 0;
mutex * mutexes = 0;
channel_base * channels = 0;
mailbox_base * mailboxes = 0;
pool_base * pools = 0;

void add( flag * f ){
   f->next_flag = flags;
   flags = f;
}

void add( timer * t ){
   t->next_timer = timers;
   timers = t;
}

void add( clock * c ){
   c->next_clock = clocks;
   clocks = c;
}

void add( mutex * m ){
   m->next_mutex = mutexes;
   mutexes = m;
}

void add( channel_base * c ){
   c->next_channel = channels;
   channels = c;
}

void add( mailbox_base * m ){
   m->next_mailbox = mailboxes;
   mailboxes = m;
}

void add( pool_base *p ){
   p->next_pool = pools;
   pools = p;
}

#endif

callback * callbacks = 0;

void add( callback * t ){
   t->next_callback = callbacks;
   callbacks = t;
}
   
// forward, tasks are in priority order
void add( task * task );


//***************************************************************************
//
// class event
//
//***************************************************************************

bool event :: operator==( const event & rhs ) const {
   if( t != rhs.t ){
      bmptk::fatal_error_detected( "comparing incompatible waitables" );
   }
   return mask == rhs.mask;
}

bool event :: operator==( const waitable & rhs ) const {
   if( t != rhs.t ){
      bmptk::fatal_error_detected( "comparing incompatible waitables" );
   }
   return *this == (event)rhs;
}

event event :: operator+( const event & rhs ) const {
   if( t != rhs.t ){
      bmptk::fatal_error_detected( "adding incompatible waitables" );
   }
   return event( t, mask | rhs.mask );
}

void event::print( ostream & s, bool header ) const {
   s << "event t=" << t->name() << " m=0b" << bin << mask;
}


//***************************************************************************
//
// class waitable
//
//***************************************************************************

waitable :: waitable( task *t, const char *arg_name ) :
   event( t, mask = t->waitables.waitable_allocate() )
{
   BMPTK_RTOS_STATISTICS( waitable_name = string_clone( arg_name ); )
}

void waitable::clear(){ 
   t->waitables.clear( *this ); 
}   
   
void waitable::set(){ 
   t->waitables.set( *this ); 
}


//***************************************************************************
//
// class flag
//
//***************************************************************************

flag::flag( task *t, const char *name ):
   waitable( t, name )
   #if BMPTK_RTOS_STATISTICS_ENABLED
      ,n_sets( 0 ),
      n_gets( 0 )
   #endif
{
   BMPTK_RTOS_STATISTICS( add( this ); )
}

void flag::set(){
   BMPTK_RTOS_STATISTICS( n_sets++; )
   waitable::set();
}

void flag::print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream << setw( 18 ) << left  << "flag name";
         stream << setw( 18 ) << left  << "client";
         stream << setw(  3 ) << right << "fn";
         stream << setw(  8 ) << right << "sets";
         stream << setw(  8 ) << right << "gets";
         stream << "\n";
      }
      stream << setw ( 18 ) << left  << waitable_name;
      stream << setw ( 18 ) << left  << task_name( t );
      stream << setw(   3 ) << right << dec << nr_from_mask( mask );
      stream << setw(   8 ) << right << dec << n_sets;
      stream << setw(   8 ) << right << dec << n_gets;
      stream << "\n";
   #endif
}


//***************************************************************************
//
// class callback
//
//***************************************************************************

callback::callback( task *t, const char *name ) :
    waitable( t, name ),
    running( false )
{
   add( this );
}

void callback::print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream << setw( 18 ) << left  << "callback name";
         stream << setw( 18 ) << left  << "client";
         stream << setw(  3 ) << right << "fn";
         stream << setw(  8 ) << right << "running";
         stream << setw( 12 ) << right << "deadline";
         stream << "\n";
      }
      stream << setw ( 18 ) << left  << waitable_name;
      stream << setw ( 18 ) << left  << task_name( t );
      stream << setw(   3 ) << right << dec << nr_from_mask( mask );
      stream << setw(   8 ) << right << running;
      stream << setw(  12 ) << right << dec << deadline / bmptk::us;
      stream << "\n";
   #endif
}


//***************************************************************************
//
// class timer
//
//***************************************************************************

timer::timer( task * t, const char *name ):
   callback( t, name )
   #if BMPTK_RTOS_STATISTICS_ENABLED
      ,n_sets( 0 )
      ,n_cancels( 0 )
   #endif
{
   BMPTK_RTOS_STATISTICS( add( this ); )
}

void timer::set( time t ){
   BMPTK_RTOS_STATISTICS( n_sets++; )
   callback::start( t );
}

void timer::await( time t ){
   BMPTK_RTOS_STATISTICS( n_sets++; )
   callback::await( t );
}   

void timer::cancel(){
   BMPTK_RTOS_STATISTICS( n_cancels++; )
   callback::cancel();
   waitable::clear();
}

void timer::time_up(){ 
   waitable::set(); 
}

void timer::print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream << setw( 18 ) << left  << "timer name";
         stream << setw( 18 ) << left  << "client";
         stream << setw(  3 ) << right << "fn";
         stream << setw(  8 ) << right << "sets";
         stream << setw(  8 ) << right << "cancels";
         stream << "\n";
      }
      stream << setw ( 18 ) << left  << waitable_name;
      stream << setw ( 18 ) << left  << task_name( t );
      stream << setw(   3 ) << right << dec << nr_from_mask( mask );
      stream << setw(   8 ) << right << dec << n_sets;
      stream << setw(   8 ) << right << dec << n_cancels;
      stream << "\n";
   #endif
}


//***************************************************************************
//
// class clock
//
//***************************************************************************

clock::clock(
   task * t,
   time interval,
   const char *name
):
   callback( t, name ),
   _interval( interval )
   #if BMPTK_RTOS_STATISTICS_ENABLED
      ,ticks( 0 )
   #endif
   
{
   callback::start( _interval );
   BMPTK_RTOS_STATISTICS( add( this ); )
}

void clock::time_up(){
   BMPTK_RTOS_STATISTICS( ticks++; )
   callback::restart( _interval );
   waitable::set();
}

void clock::print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream << setw( 18 ) << left  << "clock name";
         stream << setw( 18 ) << left  << "client";
         stream << setw(  3 ) << right << "fn";
         stream << setw(  9 ) << right << "interval";
         stream << setw( 10 ) << right << "ticks";
         stream << "\n";
      }
      stream << setw ( 18 ) << left  << waitable_name;
      stream << setw ( 18 ) << left  << task_name( t );
      stream << setw(   3 ) << right << dec << nr_from_mask( mask );
      stream << setw(   9 ) << right << dec << ( _interval / us );
      stream << setw(  10 ) << right << dec << ticks;
      stream << "\n";
   #endif
}


//***************************************************************************
//
// class waitable_set
//
//***************************************************************************

unsigned int waitable_set :: waitable_allocate(){
   if( used >= 8 * sizeof( current_waitables )){
      bmptk::fatal_error_detected( "max 32 waitables per task" );
   }
   return 1 << used++;
}

void waitable_set::set( const waitable &w ) {

   // set the waitable bit
   current_waitables |= w.mask;

   // if the client was waiting for this waitable,
   // the client must be unblocked
   if( requested_waitables & w.mask ){
      w.t->unblock();
   }
}

void waitable_set::clear( const waitable &w ){
   current_waitables &= ~ w.mask;
}

event waitable_set::wait( unsigned int mask ) {

   for(;;){
      // try to find a waitable for which we are waiting
      for( unsigned int i = 0 ; i < used; i++ ){
         if( current_waitables & mask & ( 1U << i )){

            // clear the waitable
            current_waitables &= ~( 1U << i );
            #if BMPTK_RTOS_STATISTICS_ENABLED
				   // update statistics
   			   for( flag * f = flags; f != 0; f = f->next_flag ){
      			   if( f->t == client && f->mask == ( 1U << i ) ){
						   f->n_gets++;
      				   break;
					   }
   			   }
            #endif
            
            // return an event for the waitable
            return event( client, 1U << i );
         }
      }

      // no waitable found? wait for better times..
      requested_waitables = mask;
      
      client->block();
      
      // why??
      requested_waitables = 0;
   }
}


//***************************************************************************
//
// class mutex
//
//***************************************************************************

mutex::mutex( const char *name ):
   owner( 0 ),
   waiters( 0 )
   #if BMPTK_RTOS_STATISTICS_ENABLED
      ,wait_count( 0 )
   #endif
{
   BMPTK_RTOS_STATISTICS( mutex_name = string_clone( name ); )
}

void mutex::wait() {
   BMPTK_RTOS_STATISTICS( wait_count++; )
   
   if( owner == 0 ){
   
      // not yet claimed, just claim the mutex
      owner = current_task();
      
   } else {
      task * t = current_task();
      task **p = &waiters;

      // get p to point to the last waiting task pointer
      while( *p != 0 ){
         p = & (*p)-> next_mutex_waiter;
      }

      // insert t after the last waiting task
      *p = t;
      t->next_mutex_waiter = 0;

      // we wait for better times...
      t->block();
   }
}

void mutex::signal() {

   // only the current owner is allowed to signal the mutex
   if( owner != current_task()) {
   
      bmptk::fatal_error_detected( "mutex signaled by another task" );
      
   } else {
   
      // try to the first waiting task
      task *t = waiters;
      if ( t != 0 ) {
      
         // remove task t from the queue
         waiters = waiters->next_mutex_waiter;

         // t is now the owner of the mutex and can run again
         owner = t;
         t->unblock();
         
      } else {
      
         // the waiting queue was empty: 
         // no task owns the semaphore any more
			owner = 0;
      }   
   }
}

void mutex::print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream << setw( 18 ) << left  << "mutex name";
         stream << setw( 18 ) << left  << "owner";
         stream << setw(  8 ) << right << "waits";
         stream << "   waiters";
         stream << "\n";
      }
      stream << setw ( 18 ) << left  << mutex_name;
      if( owner == 0 ){
         stream << "-";
      } else {
         stream << owner->task_name;
      }
      stream << setw(   8 ) << right << dec << "waits";
      stream << "[ ";
      for( task *t = waiters; t!= 0; t = t->next_mutex_waiter ){
         stream << t->task_name << " ";
      }
      stream << "]";
   #endif
}

mutex::~mutex() {
   bmptk::fatal_error_detected( "mutex destructor called" );
}


//***************************************************************************
//
// class channel
//
//***************************************************************************

channel_base::channel_base( task *t, const char *name ):
   waitable( t, name ),
   #if BMPTK_RTOS_STATISTICS_ENABLED
      writes( 0 ),
      ignores( 0 ),
   #endif
   qSize( 0 ),
   head( 0 ),
   tail ( 0 )
{
   #if BMPTK_RTOS_STATISTICS_ENABLED
      channel_name = string_clone( name );
      add( this );
   #endif
}

void channel_base::print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream << setw( 18 ) << left  << "channel name";
         stream << setw( 18 ) << left  << "owner";
         stream << setw(  3 ) << right << "fn";
         stream << setw(  8 ) << right << "writes";
         stream << setw(  8 ) << right << "ignores";
         stream << setw( 10 ) << right << "cur.size";
         stream << "\n";
      }
      stream << setw ( 18 ) << left  << channel_name;
      stream << setw ( 18 ) << left  << t->name();
      stream << setw(   3 ) << right << nr_from_mask( mask );
      stream << setw(   8 ) << right << dec << writes;
      stream << setw(   8 ) << right << dec << ignores;
      stream << setw(  10 ) << right << dec << qSize;
      stream << "\n";
   #endif
}


//***************************************************************************
//
// class pool
//
//***************************************************************************

pool_base::pool_base( const char *name )
   #if BMPTK_RTOS_STATISTICS_ENABLED
      : reads( 0 ),
      writes( 0 )
   #endif
{
   BMPTK_RTOS_STATISTICS( pool_name = string_clone( name ); )
   BMPTK_RTOS_STATISTICS( add( this ); )
}

void pool_base::print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream << setw( 18 ) << left  << "pool name";
         stream << setw(  8 ) << right << "writes";
         stream << setw(  8 ) << right << "reads";
         stream << "\n";
      }
      stream << setw ( 18 ) << left  << pool_name;
      stream << setw(   8 ) << right << dec << writes;
      stream << setw(   8 ) << right << dec << reads;
      stream << "\n";
   #endif
}


//***************************************************************************
//
// class mailbox
//
//***************************************************************************

mailbox_base::mailbox_base( const char *name )
   #if BMPTK_RTOS_STATISTICS_ENABLED
      : client( 0 ),
      writes( 0 )
   #endif
{
   BMPTK_RTOS_STATISTICS( mailbox_name = string_clone( name ); )
   BMPTK_RTOS_STATISTICS( add( this ); )
}

void mailbox_base::print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream << setw( 18 ) << left  << "mailbox name";
         stream << setw( 18 ) << left  << "client";
         stream << setw(  8 ) << right << "writes";
         stream << "\n";
      }
      stream << setw ( 18 ) << left  << mailbox_name;
      stream << setw ( 18 ) << left  << task_name( client );
      stream << setw(   8 ) << right << dec << writes;
      stream << "\n";
   #endif
}


//***************************************************************************
//
// class task
//
//***************************************************************************

// used to call the main of a task
// must be a void f(), because context requires such a function as main
void task_trampoline(){

   // call the main of the current task
   current_task()->main();
   
   // the main of a task should never return
   bmptk::fatal_error_detected( "a task has returned from main()" );
}

task::task(
   unsigned int priority,
   const char * tname,
   unsigned int stacksize
) :
   task_is_blocked( false ),
   task_priority( priority ),
   waitables( this )
   #ifdef BMPTK_RTOS_STATISTICS_ENABLED
      ,logging( task_logging )
      ,task_name( string_clone( tname ))
   #endif
{

   // no sleep timer allocated yet
   sleep_timer = 0;
   
   // construct the task context
   context = new bmptk::context( task_trampoline, stacksize );
   
   #ifdef BMPTK_RTOS_STATISTICS_ENABLED
   if( debug_task_logging ){
      trace << *context;
   }
   #endif
   
   ignore_this_activation = false;
   rtos::add( this );
   #ifdef BMPTK_RTOS_STATISTICS_ENABLED
      task_trace << "CREATED";
   #endif
}

void task::unblock(){
   #ifdef BMPTK_RTOS_STATISTICS_ENABLED
      task_trace << "unblock";
   #endif
   task_is_blocked = false;
   release();  
}

void task::block() {
   #ifdef BMPTK_RTOS_STATISTICS_ENABLED
      task_trace << "block";
   #endif      

   // Only a running task can block itself
   if (current_task() != this){
      bmptk::fatal_error_detected( "task not blocked by itself" );
   }

   if( ! is_ready() ){
      bmptk::fatal_error_detected( "panic: running task is not READY??" );
   }

   task_is_blocked = true;
   release();
}

bool scheduler_running = false;

void task::release(){
   #ifdef BMPTK_RTOS_STATISTICS_ENABLED
      task_trace << "release";
   #endif      
   main_context.resume(); 
}

void task :: print( ostream & stream, bool header ) const {
   #if BMPTK_RTOS_STATISTICS_ENABLED
      if( header ){
         stream
            << endl << endl << dec
            << setw( 18 ) << left  << "task name"
            << setw(  6 ) << right << "prio"
            << setw(  6 ) << right << "state"
            << setw(  8 ) << right << "rt_max"
            << setw(  8 ) << right << "act."
            << setw( 12 ) << right << "st u/m"
            << endl;
      }
      stream
         << dec
         << setw( 18 ) << left  << task_name
         << setw(  6 ) << right << dec << task_priority
         << setw(  4 ) << right << task_state( this )
         << setw(  8 ) << right << ( runtime_max / bmptk::us )
         << setw(  8 ) << right << activations
         << setw(  6 ) << right << context->stack_used()
         << "/"
         << setw(  5 ) << right << context->stack_size
         << hex
         << endl;
   #endif
}


//***************************************************************************
//
// class task
//
//***************************************************************************

//! used by statistics clearance
bool must_clear;
   
//! used by statistics clearance
void do_statistics_clear();

//! called repeatedly by run
void beat();

// Reference to the task currently executed:
task * rtos_current_task = 0;

task * current_task(){ 
   return rtos_current_task; 
}   
   
void statistics_clear(){ 
   must_clear = true; 
}

// the list all tasks, highest priority first
task * taskList = 0;

// register a task
void add( task * new_task ){
   if(0) if (global_logging) {
      trace << "register task " << new_task->name();
   }

   if( new_task->task_priority > BMPTK_RTOS_MAX_PRIORITY ){
      bmptk::fatal_error_detected( "illegal task priority" );
   }

   // walk the task queue untill the next task either
   // - does not exist, or
   // - has a lower priority (higher priority number) than the new task
   task ** t = &taskList;

   if(0){
      trace
         << "task " << new_task->name()
         << " priority=" << new_task->task_priority;
   }
   while( ( *t != 0 ) && ( (*t)->task_priority <= new_task->task_priority ) ) {
      if( (*t)->task_priority == new_task->task_priority ){
         if( new_task->task_priority >= BMPTK_RTOS_MAX_PRIORITY ){
            new_task->task_priority++;
         } else {
            bmptk::fatal_error_detected( "duplicate task priority" );
         }
      }
      t = &( (*t)->nextTask );
   }
   // now insert the new task after the current task
   new_task->nextTask = *t;
   *t = new_task;
}

void print( ostream & stream ){

   #if BMPTK_RTOS_STATISTICS_ENABLED

      // global info
      stream << "\n\n";
     stream << "BMPTK version : " << bmptk::version << "\n";
      stream << "RAM free      : " << dec << heap_free() << "\n";

      bool header;
      
      if( rtos_current_task != 0 ){
         rtos_current_task->ignore_this_activation = true;
      }
      
      // tasks
      header = true;
      for( task * t = taskList; t != 0; t = t->nextTask ){
         t->print( stream, header );
         header = false;
      }
      if( header ){
         stream << "no tasks\n";
      }
      stream << "\n";
      
      // flags
      header = true;
      for( flag * f = flags; f !=0; f = f->next_flag ){
         f->print( stream, header );
         header = false;
      }
      if( header ){
         stream << "no flags\n";
      }
      stream << "\n";
      
      // timers
      header = true;
      for( timer * t = timers; t != 0; t = t->next_timer ){
         t->print( stream, header );
         header = false;
      }
      if( header ){
         stream << "no timers\n";
      }
      stream << "\n";
      
      // clocks
      header = true;
      for( clock * c = clocks; c != 0; c = c->next_clock ){
         c->print( stream, header );
         header = false;
      }
      if( header ){
         stream << "no clocks\n";
      }
      stream << "\n";
      
      // channels
      header = true;
      for( channel_base * c = channels; c != 0; c = c->next_channel ){
         c->print( stream, header );
         header = false;
      }
      if( header ){
         stream << "no channels\n";
      }
      stream << "\n";
      
      // mutexes
      header = true;
      for( mutex * m = mutexes; m != 0; m = m->next_mutex ){
         m->print( stream, header );
         header = false;
      }
      if( header ){
         stream << "no mutexes\n";
      }
      stream << "\n";
      
      // mailboxes
      header = true;
      for( mailbox_base * m = mailboxes; m != 0; m = m->next_mailbox ){
         m->print( stream, header );
         header = false;
      }
      if( header ){
         stream << "no mailboxes\n";
      }
      stream << "\n";
      
      // pools
      header = true;
      for( pool_base * p = pools; p != 0; p = p->next_pool ){
         p->print( stream, header );
         header = false;
      }
      if( header ){
         stream << "no pools\n";
      }

   #endif
}


//***************************************************************************
//
// the 'rtos' itself
//
//***************************************************************************

// installed as bmptk::wait replacement by run()
void sleep_until( time t ){
   if( t < 100 * us ){
  
      // don't bother to enter the rtos for a very small delay
      bmptk::wait_busy( t );
      
   } else {

      // create the sleep timer when not done yet
      if( current_task()->sleep_timer == 0 ){
     
         current_task()->sleep_timer = 
            new timer( current_task(), "sleep timer" );
      }
     
      // sleep
      current_task()->sleep_timer->await( t );     
      current_task()->wait( *(current_task()->sleep_timer) );     
   }
}

// service the callback list
void handle_callbacks( time now ){

   for(
      callback * t = callbacks;
      t != 0;
      t = t->next_callback
   ){
      if(0){ 
         trace << "now = " << now / bmptk::us << "\n" << *t;
      }
      if( ( t->running ) && ( t->deadline < now ) ){
         t->running = false;
         t->time_up();
      }     
   }
}   

void beat(){

   // find the highest-priority ready task and run it, then return
   for (
      rtos_current_task = taskList;
      rtos_current_task != 0;
      rtos_current_task = rtos_current_task->nextTask
   ){
      if( rtos_current_task->is_ready() ){
         #if BMPTK_RTOS_STATISTICS_ENABLED
         time start = current_time();
         #endif

         rtos_current_task->context->resume();

         #if BMPTK_RTOS_STATISTICS_ENABLED
         time end = current_time();
         time runtime = end - start;
         if( ! rtos_current_task->ignore_this_activation ){
            if( runtime > rtos_current_task->runtime_max ){
               rtos_current_task->runtime_max = runtime;
            }
         }

         rtos_current_task->ignore_this_activation = false;
         rtos_current_task->activations++;
         if( must_clear ){
            do_statistics_clear();
            must_clear = false;
         }
         #endif
         // this beat is over, for the next beat we must again
         // start at the first task
         return;
      }
   }

   #if BMPTK_RTOS_STATISTICS_ENABLED

      // no runnable task has been found, nothing to do right now
      // we might as well do deadlock detection
      for( clock *c = clocks; c != 0; c = c->next_clock ){
         if( c->t->waitables.requested_waitables & c->mask ){
            return;
         }
      }
      for( timer *t = timers; t != 0; t = t->next_timer ){
         if(
            ( t->running )
            && ( t->t->waitables.requested_waitables & t->mask )
         ){
            return;
         }
      }
      
      // no task is waiting for a running timer or clock
      bmptk::fatal_error_detected( "deadlock detected" );

   #endif
}

void run() {

   scheduler_running = true;
   rtos_current_task = 0;
   #if BMPTK_RTOS_STATISTICS_ENABLED
      do_statistics_clear();
   #endif
   // print( cout );
   
   // install our sleep_until as replacement for bmptk::wait_until
   bmptk::wait_until_handler = sleep_until;

   if (global_logging) {
      trace << "scheduler starts";
   }

   for(;;){
      time now = current_time();
      handle_callbacks( now );
      beat();
   }
}

void do_statistics_clear() {
   for (
      task * task = taskList;
      task != 0;
      task = task->nextTask
   ){  
      task->statistics_clear();
   }
}

void display_statistics(){
   #if BMPTK_RTOS_STATISTICS_ENABLED
      cout << endl;
      print (cout);
      statistics_clear();
   #endif      
}


//***************************************************************************
//
// << operators
//
//***************************************************************************

ostream & operator<< ( ostream & s, const task & t ){
   t.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const flag & f ){
   f.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const event & e ){
   e.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const callback & c ){
   c.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const timer & t ){
   t.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const clock & c ){
   c.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const channel_base & c ){
   c.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const mutex & m ){
   m.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const mailbox_base & m ){
   m.print( s, false );
   return s;
}

ostream & operator<< ( ostream & s, const pool_base & p ){
   p.print( s, false );
   return s;
}


}; // namespace rtos;
}; // namespace bmptk;
