/*!

\page rtos RTOS

The interface between the target-independent part of the bmptk library 
and the target-specific parts is small. This chapter explains this interface.

The target code must implement the bmptk::curent_time() function, 
which must return the current time in a unit that is between 1ns and 1us.
The makefile.inc must make the value of 1us available via the macro
BMPTK_TICKS_PER_US, which is used by the bmptk::time::us(),
which is in turn used by bmptk::us, bmptk::ms and bmptk::s.
The first call to bmptk::curent_time() must take care of any 
initialization that is required. 

*/

//**************************************************************************
//!
//! \page RTOS
//!
//! coroutine-based tasking service with some synchronisation mechanisms
//!
//! \authors        
//!    - Marten Wensink (marten.wensink@hu.nl)
//!    - Wouter van Ooijen (wouter@voti.nl)
//!
//! \par Introduction
//!
//! This RTOS provides a simple coroutine-based tasking service with
//! a number of synchronisation mechanisms.
//!
//! whenever this documentation states that something is or produces an 
//! error this means that bmptk::fatal_error_detected will be called.
//! Which effect this has depends on the function the applicatiuon has
//! installed as user error handler.
//!
//! RTOS objects like tasks, waitables, clocks, timers, 
//! semaphores, etc can be allocated but not deallocated 
//! (not even on the stack). 
//! You will get a compile-time error if you try to do so,
//! because the destructors are private.
//!
//***************************************************************************
//!
//! \par Non-preemptive task switching
//!
//! The RTOS uses non-preemptive task switching. 
//! This means that the CPU can be switched to another task only 
//! when the currenly executing task (directly or indirectly) 
//! calls an RTOS function. 
//! Three groups of RTOS function calls can cause such a task switch:
//!
//! -# functions that cause the current task to become non-runnable, 
//!    like task::wait(), and task::suspend()
//! -# functions that make a higher-priority task runnable, 
//!    like flag::set(), and task::resume()
//! -# the function task::release(), which only purpose is 
//!    to give up the CPU to a higher priority task.
//!
//! A task can be made runnable either by an excplicit action 
//! from another task, like an event_flag::set() or task::resume() 
//! call, or implicitly by the expiration of a timer. 
//! But even for the latter case (timer expiration) the switching 
//! to another task can occur only when an RTOS function is called.
//!
//! The diagram below shows the state-event diagram for a task. 
//! The transitions from ready to running and back are governed 
//! by the RTOS always selecting the highest-priority runnable task. 
//! The events that cause the transitions between runnable and blocked 
//! and vice versa, and between blocked-and-suspended and suspended 
//! are the same, they are shown in the enlarged box. 
//! A task can only get blocked by doing something (wait, 
//! read a mailbox, etc), hence there is no transition 
//! from suspended to blocked-and-suspended. 
//!
//! TBW diagram
//!
//***************************************************************************
//!
//! \par latency Latency
//!
//! When a task is activated by a timeout (either by a timer or clock, 
//! or because it is a periodic task) at a certain moment in time it will 
//! in general not be run at that exactly time, but at some later time. 
//! This delay is called the latency. Two things contribute to the latency:
//!
//! -# Higher priority taks will be run first, until no higher 
//!    prority taks are runnable. 
//! -# When a lower priority task is running when the timer times out 
//!    the RTOS will notice this only when one of the RTOS functions 
//!    is called that does rescheduling: task::wait(), flag::set(), 
//!    task::suspend(), task::resume(), task::release(). 
//!
//! The first contribution is a consequence of the design of the application.
//! When you feel that it is inappropriate that a particular higher-priority 
//! task is run first and hence contributes to the latency of the task that
//! is activated by the timer you have set the task priorities wrong.
//!
//! The second contribution is specific for a non-preemptive RTOS. 
//! (A preemptive RTOS would immediately stop (preempt) the running 
//! lower-priority task and switch to the higher-priority task.) 
//! When you have lower priority tasks in your system that use a 
//! lot of CPU time between the RTOS calls that do rescheduling 
//! you can insert task::release() calls. 
//! This call checks whether any timers that have timed out made a 
//! higher-rpriority task runnable, and if so, switches to that task.
//!
//! When a task is made runnable by an explicit action of another task, 
//! for instance a task:resume() call or a flag::set() call, 
//! only the first source of delay (higher priority tasks that are 
//! runnable) is applicable, because inside such calls the RTOS will 
//! immediatley switch to the highest priority runnable task.
//!
//***************************************************************************
//!
//! \par Debug support
//!
//! All objects (RTOS, task, event, all waitables, mutex, pool, mailbox, 
//! channel) can be printed to an ostream using the << operator. 
//! Printing the RTOS will print all RTOS objects.
//!
//! \par Compile-time configuration
//!
//! The RTOS can be configured by changing some \#define's in bmptk_rtos.h
//! For most users the defaults should be OK.
//!
//! \par global_logging
//!
//! All RTOS debug logging can be switched off by defining 
//! \b global_logging as 0
//! It is advised to make all application debug logging likewise
//! dependent on this macro. Check bmptk_rtos.cpp for examples
//!
//! \par BMPTK_RTOS_STATISTICS_ENABLED
//!
//! By default \b BMPTK_RTOS_STATISTICS_ENABLED is defined as 1,
//! which enables printing statistics for all objects.
//! It can be defined as 0 to reduce code and data size.
//! NOTE: doing so will disable deadlock detection!
//!
//! \par BMPTK_RTOS_DEFAULT_STACK_SIZE
//! 
//! The default stack size is 2K. 
//! You can choose another value by changing the initialization of
//! \b BMPTK_RTOS_DEFAULT_STACK_SIZE.
//!
//! \par BMPTK_RTOS_MAX_PRIORITY
//! 
//! The maximum priority value (= the lowest priority) you can 
//! assign to a task is defined by \b BMPTK_RTOS_MAX_PRIORITY, which is by
//! default set to 10000. 
//
//***************************************************************************
