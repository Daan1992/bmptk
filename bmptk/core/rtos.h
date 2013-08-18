//***************************************************************************
//
// file : bmptk/core/rtos.h
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
//***************************************************************************

#include <iostream>
#include <limits>

namespace bmptk {

namespace rtos {

#define global_logging 0

// #define BMPTK_RTOS_STATISTICS_ENABLED 1

//! the default task stack size
const unsigned int BMPTK_RTOS_DEFAULT_STACK_SIZE = 2048;

//! the maximum user-defined taks priority
const unsigned int BMPTK_RTOS_MAX_PRIORITY = 10000;

// the macro BMPTK_RTOS_STATISTICS is used to prefix a single line
// that will be commented out when statistics is disabled
#if BMPTK_RTOS_STATISTICS_ENABLED
   #define BMPTK_RTOS_STATISTICS( x ) x
#else
   #define BMPTK_RTOS_STATISTICS( x )
#endif

class task;
class event;
class waitable;
class flag;
class timer;
class clock;
class mutex;
template <class T> class mailbox;
template <class T, const int SIZE> class channel;
template <class T> class pool;
class mailbox_base;
class channel_base;
class pool_base;
class waitable_set;
class callback;


//************************************************************************
//
// some supporting stuff
//
//************************************************************************

//! run the rtos scheduler
//
//! When the tasks of an application have been created, the
//! next (and for the main(), final) step is to call this function.
void run();

//! returns (a pointer to) the currently executing task
//
//! Initially (before run is called) this function will return 0;
task * current_task();

//! prints statistics about the tasks to the stream.
void print( std::ostream & stream );

//! clears the statistics.
//
//! The task statistics are cumulative.
//! Call this function to make a fresh start.
//! The actual clearing will be done later, inside \ref run(),
//! when the current task has given up the processor.
//! The effect is that the current timeslice is not 
//! reflected in the new statistics.
void statistics_clear();

   //! print the statistics
void display_statistics();


//************************************************************************
//
// class event
//
//! set of things that can happen, or a thing that has happened
//
//! An event
//! - is the result of adding waitables
//! - is accepted as argument to wait()
//! - is returned by wait()
//! - can be compared to a waitable
//!
//! The task::wait() calls return an event. 
//! Such an event can be compared to a waitable. 
//! The result is true if and only if the waitable caused the event.
//!
//! Events are the only rtos objects that can be destroyed 
//! (without causing an error).
//
//************************************************************************

class event {
public:
   
   //! prints an event, for debugging only
   void print( std::ostream & s, bool header ) const;
        
   //! report wether two events are the same   
   bool operator==( const event & rhs ) const;
   
   //! report whether an event corresponds to a waitable
   bool operator==( const waitable & rhs ) const;
   
   //! report wether two events are not the same    
   bool operator!=( const event & rhs ) const {
      return ! ( *this == rhs );
   }
   
   //! report whether an event does not correspond to a waitable      
   bool operator!=( const waitable & rhs ) const {
      return ! ( *this == rhs );
   }
   
   //! add two waitables, result can be used in a wait() call
   //
   //! Waitables can be added (operator+) to construct a 
   //! 'set of waitables' as argument to a task::wait() call.
   event operator+( const event & rhs ) const;

private:
   
   //! the task that owns this event
   task * t;
   
   //! the mask of this event, one bit is set
   //
   //! The bit that is set is unique among the events
   //! owned by a task.
   unsigned int mask;
   
   //! constructor, used by concrete events
   event( task * t, unsigned int mask ): t( t ), mask( mask ){}

   friend class waitable_set;
   friend class waitable;
   friend class channel_base;
   friend void beat();
   friend class flag;
   friend class callback;
   friend class clock;
   friend class timer;
};


//************************************************************************
//
// class waitable
//
//! abstract thing that a task can wait for
//
//! Waitable is an abstract class (there are no objects that are 
//! just a waitable). 
//! \ref flag, \ref timer, \ref clock and \ref channel are concrete 
//! classes that inherit from waitable. 
//! A waitable is always created for a particular task. 
//! A maximum of 31 waitables can be created for each task. 
//! (Actually the maximum is 32, but one waitable is created 
//! internally to implement the rtos-version of the wait() call.) 
//! A waitable can be two states: set or cleared. 
//! A waitable is initially cleared.
//!
//! A task can wait for one, a subset, or all waitables created for it. 
//! The default is to wait for all waitables created for the task, 
//! the other variants are specified by supplying to the task:wait() 
//! call either a single waitable, or the sum (operator+) of the 
//! waitables you want to wait for. When one of the waitables that 
//! is waited for is or becomes set the wait() call clears that 
//! waitable and returns an event that compares equal to the waitable. 
//! (Note that some waitables, for instance the channel, 
//! can immediately set itself again.) The calling task can compare 
//! that event to the waitables to see which event happened. 
//! When more than one of the waited-for waitables is set the wait() 
//! call makes an arbitrary choice from these waitables.
//!
//! The operation clear() is provided (virtual, the default only clears 
//! the waitable) set() is provided but private (not all waitables can 
//! be set by the user).
//
//************************************************************************

class waitable : public event {
public:

   //! clear the waitable
   //
   //! This is automatically done when the waitable 
   //! causes a task::wait() call to return it.
   virtual void clear();

protected:
   
   //! constructor, specify owner and name
   //
   //! The name is used for debugging only.
   waitable( task * task , const char *name );
   
   //! set the waitable
   void set();

   BMPTK_RTOS_STATISTICS( const char *waitable_name; )

   friend class waitable_set;
};


//************************************************************************
//
// class flag
//
//! basic synchronisation mechanism.
//
//! The basic synchronization mechanism is the (event) flag. 
//!
//! Like all waitables, a flag is created for a particular task. 
//! A flag is set by a flag::set() call (or the task::set( flag) 
//! call, which has the same effect). 
//! Like all waitables, when a task is waiting for a flag 
//! (using a task::wait call) and that flag becomes set, 
//! the wait call will clear the flag, and return an event 
//! that compares equal to the flag. 
//! Note that a flag does not count: setting a flag that is 
//! already set has no effect on the flag.
//!
//! A flag must be created for a specific task.
//! The normal place to do this is in the task's constructor.
//! An flag is initially cleared.
//!
//! The example below shows a led_task that responds to two event flags. 
//! The shift flag will cause it to shift the pattern on the LEDs one 
//! position to the left, while the invert flag will cause it 
//! to invert the pattern. 
//! Two addional tasks do nothing but set these flags at fixed intervals. 
//! The result is a sort of one-direction Kitt display, 
//! which will occasionally flip polarity. 
//! Note that in this example the wait call excplicitly 
//! mentions the flags it waits for.
//!
//! \code
//! TBW flags example
//! \endcode
//
//************************************************************************

class flag : public waitable {
public:
   
   //! constructor, specify onwer and name
   //
   //! This call creates a timer for task t. 
   //! The name is used for debugging and statistics.
   flag( task * t, const char *name = "" );
   
   //! sets the flag
   //
   //! Setting a flag causes the task that waits for this
   //! flag to be awakened.      
   void set ();
   
   //! prints flag infomation (for debugging)
   void print( std::ostream & s, bool header = true ) const;

private:
   
     // this information is needed for statistics only
   BMPTK_RTOS_STATISTICS( flag *next_flag; )
   BMPTK_RTOS_STATISTICS( unsigned int n_sets; )
   BMPTK_RTOS_STATISTICS( unsigned int n_gets; )
   
   friend class event;
   friend class waitable_set;
   friend void add( flag * f );
   friend void print( std::ostream & stream );
};


//************************************************************************
//
// class waitable_set
//
// This is an private implementation construct, it is not documented
//
//************************************************************************

#ifndef DOXYDOC

class waitable_set {
private:
   event wait( unsigned int mask );

public:
   task * client;
   unsigned int current_waitables;
   unsigned int requested_waitables;
   unsigned int used;

   waitable_set( task * client ):
      client( client ),
      current_waitables( 0 ),
      requested_waitables( 0 ),
      used( 0 ) {}

   unsigned int waitable_allocate();
   void set( const waitable &w );
   void clear( const waitable &w );

   event wait(){ return wait( ~0 ); }
   event wait( const waitable & w ){ 
      return wait( w.mask ); 
   }
   event wait( const event & set ){ 
      return wait( set.mask );
   }
};

#endif


//************************************************************************
//
// class task
//
//! an independent thread of execution
//
//! A task is an independent thread of execution, using its own stack. 
//! Tasks share the single CPU, so only one task can be running at any time. 
//! The rtos determines which task is running. 
//! A task has two states: runnable and blocked. 
//! When a task is created it is runnable.
//!
//! All tasks (and the rtos code itself) run in the same memory space, 
//! without protection from each other. 
//! Hence a 'wild pointer' in one task can destroy data in 
//! another task, or even in the rtos. 
//!    
//! Each task is created with a fixed priority, which can be any 
//! unsigend integer value below BMPTK_RTOS_MAX_PRIORITY (by default 10_000). 
//! After creation the priority can not be changed.
//! The value 0 indicates the highest task priority, a higher number 
//! indicates a lower priority. 
//! Each task must have a unqiue priority, it is an error to create 
//! a task with same priority as an existing task. 
//! You can omit the priority, in which case the rtos will 
//! select an unused priority starting at BMPTK_RTOS_MAX_PRIORITY 
//! (in other words, it will choose a low priority for your task).
//! 
//! Each task has its own stack. 
//! You can specify the size of the stack at task creation. 
//! If you omit the stack size, BMPTK_RTOS_DEFAULT_STACK_SIZE will be used 
//! (default: 2 Kb). 
//! This will be enough for most tasks, if you take care not to 
//! allocate big things on the stack, and avoid very deep nesting 
//! (watch out for recursion!).
//! 
//! A task is created by instantiating a class that derives from 
//! rtos::task and supplies a main(). 
//! This main() should never return. 
//! The fragment below shows how you can do this. 
//! The task name is used for statistics and debugging. 
//! As shown for the name, it might be wise to get the task parameters 
//! as arguments to the constructor of your task.
//!
//! code
//!
//! The example below is a complete program that shows the 
//! standard part (initialization, and a main that calls 
//! rtos::run()), a function for writing to an individual LED, 
//! a task class that blinks a LED, and two instatiations of 
//! this class. 
//!
//! Subseqent examples will not show the standard 
//! initialization (the part up to the comment line).
//!
//! code
//
//************************************************************************

class task {

private:

      // The rtos uses block() and unblock() to implement the
      // synhronisation mechanisms (waitables).
   bool task_is_blocked;

      // a lower number means a higher priority, 0 is highest
   unsigned int task_priority;

      // timer for sleep() call
   timer * sleep_timer;
   
   friend void task_trampoline();   
   friend void sleep_until( time t );
   
protected:

      // task name, only for statistics
   BMPTK_RTOS_STATISTICS( const char *task_name; )

   //! task body, must be provided by a derived concrete class
   //
   //! A task is created by inheriting
   //! from task and providing a main() function. 
   //! Initialisation of the task, including creating its waitables, 
   //! should be done in the constructor.
   //! Don't forget to call the constructor of the task class!
   //!
   //! The main() is the body of the task. It should never terminate.
   //!
   //! Each task has a unique priority (an unsigned integer).
   //! A lower value indicates a higher priority.
   //! The rtos scheduler will always run the task with the 
   //! higest-priority runnable (neither blocked nor suspended) task.
   //! A task runs until it changes this 'situation' by using an rtos
   //! call that changes its own state to not runnable, or the state of a
   //! higher priority task to runnable.
   //!
   //! Timers are served only when the rtos is activated
   //! by calling any of its state-changing interfaces.
   //! Hence the longest run time between such calls determines the 
   //! granularity (timewise responsiveness) of the application.
   //! Within a timeconsuming computation a task can call release() 
   //! to have the rtos serve the timers.      
   virtual void main() = 0;
   
public:

   //! constructor, specify priority, name and stacksize
   //
   //! Priorities are reasonably-valued (below BMPTK_RTOS_DEFAULT_RIORITY)
   //! unsigned integers. 0 is te highest priority. 
   //! Priorities must be unqiue. 
   //! The default causes the constructor to choose a free priority 
   //! starting at BMPTK_RTOS_DEFAULT_PRIORITY (default: 10000). 
   //!
   //! The name is used for debugging and statistics. 
   //!
   //! A stack of stack_size bytes is allocated for the task. 
   //! The default is 2 kB.
   task(
      unsigned int priority  = BMPTK_RTOS_MAX_PRIORITY,
      const char * tname     = "",
      unsigned int stacksize = BMPTK_RTOS_DEFAULT_STACK_SIZE
   );

   //! throws an error, beacuse tasks should never be destroyed
   virtual ~task () {
      fatal_error_detected( "task destructor called" ); 
   }

   //! release the CPU to the scheduler
   //
   //! Sevices timers and releases the CPU to a higher 
   //! priority task if one became ready.
   void release();

   //! report the task priority
   unsigned int priority() const {
      return task_priority; 
   }
   
   //! report the task name
   const char * name() const {
      #if BMPTK_RTOS_STATISTICS_ENABLED
         return task_name; 
      #else
         return "";
      #endif
   }   
      
   //! report whether the task is currently ready for execution
   bool is_ready() const {
      return ! task_is_blocked;
   }
   //! report whether the task is currently blocked
   bool is_blocked() const {
      return task_is_blocked;
   }

   //! print task statistics
   void print( std::ostream & stream, bool header = true ) const;
               
   //! wait for all waitables created for this task
   //
   //! Wait (prevent execution) until at least one of the waitables 
   //! is set. Return and clear that waitable. 
   //! Three variants for the parameter:
   //!
   //! - The default (no parameter) waits for all waitables 
   //!   defined for the task.
   //! - One waitable as argument waits for that specific waitable.
   //! - The addition (operator+) of waitables: wait for any one of
   //!   those waitables.
   //!
   //! It is an error to wait for waitables that have not been created 
   //! for this task.
   event wait(){ return waitables.wait(); }
   
   //! wait for a single waitable
   //
   //! @copydetails wait
   event wait( const waitable & w ){ 
      return waitables.wait( w ); 
   }
   
   //! wait for a set of waitables
   //
   //! @copydetails wait
   event wait( const event & set ){ 
      return waitables.wait( set ); 
   }

   //! set a flag
   void set( flag &w ){ 
      w.set(); 
   }

   //! ignore this activation for the statistics
   //
   //! Calling this function makes the rtos statistics ignore
   //! the current task activation as far as statistics is concerned.
   //! You can use this to avoid pullution of your task statistics
   //! with the timing effects of debug logging. But make sure you
   //! don't use it in the 'normal' execution paths, becaue that would 
   //! make the statitics lie to you.
   void ignore_activation_time(){
      ignore_this_activation = true;
   }

private:

   //! clear statistics of this task
   void statistics_clear(){
      runtime_max = 0 * ms;
      activations = 0;
   }

   //! waitables on which the task can wait
   waitable_set waitables;

   //! for blocking a task by means of a synchronization mechanism
   void block();

   //! for unblocking a task by means of a synchronization mechanism
   void unblock();

   //! statistics
   int activated;
   time latency_max;
   time runtime_max;
   int activations;
   bool ignore_this_activation;

   //! for the rtos to link all tasks
   task * nextTask;

   //! for the mutexes to link waiting tasks
   task * next_mutex_waiter;

   //! the coroutine that holds the task body and stack
   bmptk::context *context;

   friend class periodic_task;
   friend class waitable_set;
   friend class flag;
   friend class waitable;
   friend class mutex;
   friend void print( std::ostream & stream );
   friend void do_statistics_clear();
   friend void add( task * new_task );
   friend void beat();
   // friend void channel_base::print( std::ostream & stream, bool header ) const;
};


//************************************************************************
//
// class callback
//
// rtos private implementation class
//
//************************************************************************

#ifndef DOXYDOC

class callback : public waitable {
public:

   callback( task *t, const char *name );

   // a callback should never be destroyed
   virtual ~callback() {
      fatal_error_detected( "callback destructor called" );
   }

   // the time_up function must be provided by a derived class
   virtual void time_up( void ) = 0;
   
   //! print the callback (for debugging)
   void print( std::ostream & s, bool header = true ) const;   

protected:

   // the callback must be called at t
   void await( time t ){ 
      deadline = t;
      running = true;
   }

   // the callback must fire <time> from now
   void start( time t ){ 
      await( bmptk::current_time() + t );
   }
   
   // the callback must fire in the future, at a multiple of 
   // <time> from the last firing
   void restart( time t ){
      time now = bmptk::current_time();
      while( deadline <= now ){ 
         deadline += t; 
      }
      running = true;
   }

   // abort a started timer
   virtual void cancel(){ 
      running = false;
   }

   // true iff the timer is running
   bool running;
   
   // the moment the callback should be called
   time deadline;

   // for the rtos to link timers in a chain
   callback * next_callback;

   friend class task;
   friend void beat();
   friend void add( callback * t );
   friend void handle_callbacks( time t );
};

#endif


//************************************************************************
//
// class timer
//
//! one-short timer
//
//! A (one-shot) timer is a special type of flag, which can be 
//! instructed to set itself after a fixed amount of time. 
//! The amount of time is supplied with the timer::set() call. 
//! This call starts the timer. 
//! A timer that is running (waiting for its timeout to expire) 
//! can be canceled by the timer::cancel() call. 
//! When a timer that is already running is set again the previous 
//! timeout is overwritten by the new one. 
//! The suspend/resume state of its owner taks has no effect on 
//! a timer: even when the task is suspended the timer will run 
//! to its timeout and set isetlf. 
//! But of course the task, being suspended, will not be able to react.
//!
//! The example below again (like the flags example) produces 
//! a left-shifting polarity-flipping Kitt display, but the 
//! actions are now caused by two timers inside the one task. 
//! Note that these timers must be started initially, 
//! and must be re-started after each timeout.
//!
//! \code
//! TBW
//! \endcode
//
//************************************************************************

class timer : public callback {
public:
   
   //! create a timer for task t, specify its name
   timer( task * t, const char *name = "" );
   
   //! set the timer to timeout after the specified time
   //
   //! Start the timer: it will set itself after the indicated timeout, 
   //! starting from now. 
   //! When the timer was already running the previous timout 
   //! is overwritten.
   void set( time t );
   
   //! set the timer to timeout at the specified time
   //
   //! Start the timer: it will set itself at the indicated time. 
   //! When the timer was already running the previous timout 
   //! is overwritten. 
   void await( time t );
    
   //! stop and clear the timer
   //
   //! Stop the timer (when it was running), 
   //! and clears its (when it was set).
   void cancel();

   //! print the timer (for debugging)
   void print( std::ostream & s, bool header = true ) const;

private:

   void time_up();
   BMPTK_RTOS_STATISTICS( timer *next_timer; )
   BMPTK_RTOS_STATISTICS( unsigned int n_sets; )
   BMPTK_RTOS_STATISTICS( unsigned int n_cancels; )
   
   friend void add( timer * t );
   friend void print( std::ostream & stream );
   friend void beat();
};

//************************************************************************
//
// class clock
//
//! free-running clock, ticks at a fixed frequency
//
//! A clock is a waitable which is automatically sets itself 
//! at fixed intervals. 
//! The interval between these moments is specified when the 
//! clock is created. 
//! 
//! The example below again shows the left-shifting and polarity 
//! flipping Kitt display, but the actions are now caused by two 
//! clocks. 
//! Note that unlike the timers examples the clocks are never 
//! explictly started.
//!
//! \code
//! TBW
//! \endcode
//
//************************************************************************

class clock :  public callback {
public:
   
   //! create a clock for task t, specify interval and name
   //
   //! The name is used for debugging and statistics.
   clock( 
      task * t, 
      time interval, 
      const char *name = "" 
   );
   
   //! clear the waitable within the clock
   //
   //! Note that this does not stop the clock.
   void clear(){ waitable::clear(); }
      
   //! the interval of the timer    
   time interval(){ 
      return _interval; 
   }

   //! print the clock (for debugging)
   void print( std::ostream & s, bool header = true ) const;

private:
   void time_up();
   time _interval;
   BMPTK_RTOS_STATISTICS( clock *next_clock; )
   BMPTK_RTOS_STATISTICS( unsigned int ticks; )
   
   friend void add( clock * c );
   friend void beat();
   friend void print( std::ostream & stream );
};


//************************************************************************
//
// class pool_base
//
//! rtos private implementation class
//
//************************************************************************

#ifndef DOXYDOC

class pool_base {
public:

   pool_base( const char * name );
   ~pool_base() {
      fatal_error_detected( "pool destructor called" );
   }
   void print( std::ostream & s, bool header = true ) const;

#if BMPTK_RTOS_STATISTICS_ENABLED
   unsigned int reads;
   unsigned int writes;
   pool_base * next_pool;
   const char * pool_name;
#endif

};

#endif


//************************************************************************
//
// class pool
//
//! place to store and retrieve data, no built-in synchronisation
//
//! A (communication) pool is a template class that stores a single value.
//! It supports the read and write operations, which are guaranteed to be 
//! atomic. (On a non-preemptive rtos every assignment is atomic, 
//! but the pool template is still usefull to make it explicit that
//! data is transferred between tasks.) 
//!
//! The example below demonstrates the use of a pool to 
//! maintain a seconds-since-startup counter. 
//! Note that the call RTOS::runtime() returns the time elapsed since 
//! startup, so there is no need to maintain a 
//! seconds-since-startup this way yourself.
//!
//! << code >> make it an example
//! \code
//! 
//! pool< unsigned int > seconds;
//! 
//! void show_time(){ 
//!    unsigned int n = seconds.read();
//!    std::cout << ( seconds / 60 ) % 60 << ":" << seconds % 60;
//! }
//! 
//! class seconds_counter_class : public periodic_task {
//!    seconds_counter(){
//!       periodic_task::periodic_task( "sec-counter", 10, 1000 MS );
//!       seconds.write( 0 );
//!    }
//!    void main(){
//!       for(;;){
//!          (void)wait(); // only one thing to wait for
//!          seconds.write( seconds.read() + 1 );
//!       }
//!    }
//! }
//!    
//! seconds_counter_class seconds_counter;
//!    
//! \endcode
//
//************************************************************************

template <class T> class pool : public pool_base {
public:
   
   //! construct a pool, specify its name (for debgging only)
   //
   //! Use it to make (global) variables use for communication 
   //! between tasks explicit.    
   //!
   //! The template argument T must be a class that has a 
   //! non-arguments constructor and supports assignment.
   pool( const char *name = "" ): pool_base( name ){}

   //! atomic write operation on a pool
   //
   //! A read operation returns the most recently written data.
   //!
   //! In the context of co-operative multitasking a read of write
   //! operation on anything is always atomic, unless the implementation
   //! of that operating somehow invokes the rtos.
   //! But for clearness it is a good idea to implement such task-global
   //! data as pools.        
   void write (T item) {
      BMPTK_RTOS_STATISTICS( reads++; )
      data = item;
   }

   //! atomic read operation on a pool
   //
   //! @copydetails  write
   T read (void) {
      BMPTK_RTOS_STATISTICS( writes++; )
      return data;
   }

private:
   T data;
};


//************************************************************************
//
// class mutex
//
//! mutual execlusion semaphore
//
//! A mutex (mutual exclusion semaphore) is a synchronization mechanism 
//! that is used to give a task exclusive access to some resource: 
//! the task can execute a sequence of statements, being sure that 
//! no other task is accessing the same resource. 
//!
//! A typical use is to protect a resource (for instance global data)
//! that should be used by only one task at a time, so it can update
//! it and leave it in a consistent state.   
//! 
//! A mutex is not created for a particular task,and it is not a waitable.
//! 
//! Initially a mutex is free. 
//! The mutex::wait() operation blocks the task until the mutex is free, 
//! and then claims the mutex for the executing task. 
//! The mutex::signal() operation frees the mutex again. 
//! It is an error to call mutex::signal on a mutex that is not 
//! currently owned by the executing task.
//!
//! The example below shows two tasks that write messages to the 
//! LCD display. Each task writes to its own line. 
//! For the demonstration, after each LCD operation a sleep() 
//! is put to allow the other tasks to run. 
//! A mutex prevents the other task from accessing the LCD 
//! while the first one is still using it. 
//!
//! \code
//! TBW
//! \endcode
//
//************************************************************************

class mutex {
public:
   
   //! constructor, specify the name
   //
   //! The name is used for debugging only.
   mutex( const char *name = "" );
   
   //! generates an error
   //
   //! A mutex should never be destroyed
   ~mutex();  
   
   //! prints a mutex, for debugging only.
   void print( std::ostream & stream, bool header = true ) const;

   //! claim the mutex
   //!      
   //! If the mutex was set it it is now cleared, 
   //! and the calling task owns the mutex. 
   //!
   //! Otherwise the current task waits (is halted) until the 
   //! owning task calls signal() on the same mutex. 
   //! The signal() calls will release the tasks in 
   //! the order of their wait() calls.
   void wait   (void);
   
   //! release the mutex
   //
   //! If one or more tasks are waiting for the mutex the firs 
   //! one is released, and it now owns the mutex. 
   //! Otherwise, if the mutex is cleared it is now set. 
   //! 
   //! It is an error for a task to call signal() on a 
   //! mutex that it does not own (that it did not call wait() on). 
   //! After the signal the task no longer owns the mutex.
   void signal (void);

private:

   task * owner;             // current owner of the mutex
   task * waiters;           // head of the waiting tasks queue

#if BMPTK_RTOS_STATISTICS_ENABLED
   const char * mutex_name;  // for logging
   mutex *next_mutex;        // queue of all mutexes, for logging
   int wait_count;           // counts # wait calls;
#endif

   friend void add( mutex * m );
   friend void print( std::ostream & stream );
};


//************************************************************************
//
// class mailbox_base
//
// rtos private implementation class
//
//************************************************************************

#ifndef DOXYDOC

class mailbox_base {
public:
   mailbox_base( const char *name );

   ~mailbox_base() {
      fatal_error_detected( "mailbox destructor called" );
   }

   void print( std::ostream & s, bool header = true ) const;

   task * client;
#if BMPTK_RTOS_STATISTICS_ENABLED
   const char * mailbox_name;
   unsigned int writes;
   mailbox_base *next_mailbox;
#endif

};

#endif

//************************************************************************
//
// class mailbox
//
//! synchronous handling over of a data item
//
//! A mailbox is a template class synchronization mechanism. 
//! A single value can be written to the mailbox. 
//! Another task can read the value from the mailbox. 
//! The read and write calls each wait on each other before they 
//! are allowed to proceed.
//! A mailbox is not created for a particular task, 
//! and it is not a waitable.
//!
//! Initially a mailbox is empty. 
//! The \ref write() operation bloks the calling task until 
//! the mailbox is empty, then writes to the mailbox, and blocks. 
//! The \ref read() operation blocks the calling task until 
//! there is a value in the mailbox. 
//! Then it reads the value, unblocks the task that wrote to 
//! the mailbox, and returns. 
//!
//! The example below uses a mailbox to buffer writing cout 
//! data to the UART. The function that writes each cout character 
//! writes it to a mailbox, which is read by the  UART task. 
//! This task waits 2 MS after each write to the UART, 
//! to avoid busy waiting.
//!
//! \code
//! TBW
//! \endcode
//
//************************************************************************

template <class T> class mailbox : mailbox_base {
public:
   
   //! constructor, specify mailbox name
   //
   //! Create a mutex. The mutex is initially set. 
   //! The name is used for debugging and statistics.
   //!
   //! The template argument T must be a class that has a 
   //! non-arguments constructor and supports assignment.
   mailbox( const char *name = ""  ): mailbox_base( name ){}

   //! write an item into the mailbox
   //
   //! If the mutex was set it it is now cleared, 
   //! and the calling task owns the mutex. 
   //! 
   //! Otherwise the current task waits (is halted) until the 
   //! owning task calls signal() on the same mutex. 
   //! The signal() calls will release the tasks in 
   //! the order of their wait() calls.
   void write( const T item ) {
      BMPTK_RTOS_STATISTICS( writes++; )
      data = item;
      if ( client != 0 ) {
            // someone is waiting to read, unblock it
            client->unblock();
         } else {
             // block until the reader gets the data
             client = bmptk::rtos::current_task();
             client->block();
         }
   }
   
   //! read a value from the mailbox
   //
   //! If one or more tasks are waiting for the mutex the first
   //! one is released, and it now owns the mutex. 
   //! Otherwise, if the mutex is cleared it is now set. 
   //!
   //! It is an error for a task to call signal() on a mutex 
   //! that it does not own (that it did not call wait() on). 
   //! After the signal the task no longer owns the mutex.
   T read (void) {
      if ( client == 0 ) {
         // no writer yet, so wait for a writer
         client = bmptk::rtos::current_task();
         client->block();
      }
      else {
         // unblock the writer
         // but first make sure the client is 0 !
         task *temp = client;
         client = 0;
         temp->unblock();
      }
      return data;
   }

private:
   T data;
};

//************************************************************************
//
// class channel_base
//
// rtos private implementation class
//
//************************************************************************

#ifndef DOXYDOC

class channel_base : public waitable{
public:
   virtual ~channel_base() {
      fatal_error_detected( "channel destructor called" );
   }

   void print( std::ostream & s, bool header = true ) const;

protected:
public:

   channel_base( task * t, const char *name );

#if BMPTK_RTOS_STATISTICS_ENABLED
   const char *channel_name;
   channel_base *next_channel;
   int writes;
   int ignores;
#endif

   int qSize;
   int head;
   int tail;
};

#endif

//************************************************************************
//
// class channel
//
//! waitable data queue
//
//! The (communication) channel is a template class that stores a queue 
//! of values. 
//! Values can be written at the tail of the queue, up to the number of 
//! entries for which the channel was created. 
//! It is an error to write to a channel that is full. 
//! Writes are not blocking. 
//! Any task can write to a channel. 
//!
//! A channel is created for a particular task. 
//! Only this owner task can read from the channel. 
//! A read will block until an entry is available. 
//! Reads are from the head of the queue.
//!
//! A channel is a waitable, so the task that owns the channel 
//! can wait for the channel to be non-empty, after which a read 
//! from a channel will be non-blocking (because the channel is 
//! not empty). 
//! After a wait() that returns the channel's event, the channel 
//! will set itself again (because the wait did not cause it to 
//! become empty). 
//! Only a read that results in an empty queue will clear the channel. 
//! 
//! The example below shows how writing to cout can be buffered 
//! by first writing to a 2kB channel, and reading from that 
//! channel at a maximum of one character per 2 MS. 
//! The UART hardware in the LPC2148 chip buffers one character, 
//! which at default baudrate (38k4) takes ~ 1 MS to write. 
//! So by writing at a maximum rate of one character per 2 MS 
//! no blocking will occur.
//!
//! \code
//! 
//! class output_class : public task { 
//! public:
//!    channel< char, 2048 > buffer( flags, "buffer" );
//!    timer hartbeat( flags, "hartbeat" );
//!    void main(){      
//!       for(;;){
//!          wait( buffer ); 
//!          cout << buffer.get();
//!          timer.set( 2 MS );
//!          wait( timer );
//!      }
//!    }
//! }
//!    
//! output_class output;
//! 
//! void print( char *s ){
//!    while( *s != '\0' ){ output.buffer.write( *s++ ); }
//! }
//!    
//! \endcode
//
//************************************************************************

template <class T, const int SIZE> class channel : public channel_base {
public:

   //! constructor, specify stored type, number of entries, and name
   //
   //! The template argument T must be a class that has a 
   //! non-arguments constructor and supports assignment.
   channel( task * t, const char *name = "" ): channel_base( t, name ){}

   //! write an item to the queue
   void write( T item ) {
      if( qSize < SIZE ) {
         BMPTK_RTOS_STATISTICS( writes++; )
         queue[head] = item;
         if( ++head == SIZE ) {
            head = 0;
         }
         qSize += 1;
         waitable::set();
      } else {
         BMPTK_RTOS_STATISTICS( ignores++; )
      }
   }

   //! read an item from the queue
   T read() {
      if( qSize == 0 ) {
         t->wait( *this );
      }

      if( qSize == 0 ) {
         fatal_error_detected( "channel should not be empty now");
      }
      T buf = queue[ tail ];
      if( ++tail == SIZE ) {
         tail = 0;
      }
      qSize -= 1;
      if( qSize > 0 ) {
         waitable::set();
      } else {
         waitable::clear();
      }
      return buf;
   }

   //! empty the queue
   void clear (void) {
      qSize = 0;
      head = tail = 0;
      waitable::clear();
   }
   
private:
   T queue[ SIZE] ;
};


//! print information about a task
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::task & t );

//! print information about a flag
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::flag & f );

//! print information about an event
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::event & e );

//! print information about a callback
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::callback & c );

//! print information about a timer
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::timer & t );

//! print information about a clock
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::clock & c );

//! print information about a channel
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::channel_base & c );

//! print information about a mutex
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::mutex & m );

//! print information about a mailbox
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::mailbox_base & m );

//! print information about a pool
std::ostream & operator<< ( std::ostream & s, const bmptk::rtos::pool_base & p );



}; // namespace rtos;

}; // namespace bmptk;