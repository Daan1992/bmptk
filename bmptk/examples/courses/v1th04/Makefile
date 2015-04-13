#############################################################################
# 
# Makefile that just passes the work to its subdirectories.
#
# (c) Wouter van Ooijen (www.voti.nl) 2015
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)
# 
# Bare Metal Programming Tool Kit V1TH04 examples directory
#
# This part of the tree contains example (complete) and skeleton (incomplete)
# code that is used in the microcontroller course I give at the 
# Hogeschool Utrecht, the Netherlands.
# All code uses the DB013 board, which contains an LPC1114 chip.
#
#############################################################################

# subdirectories - week 1 :
#    blink    : blink a LED
#    switch   : copy switch to LED
#    beep     : beep a speaker
SUBDIRS += blink switch beep
   
# subdirectories - week 2 :
#    440hz     : use timer to beep at an accurate 440 Hz 
#    timer     : idem, but timer code in separate .c/.h files
#    servo     : 'move' a servo back and forth
#    sw-uart   : use timer to send asynchronous data to the PC
#    ds1990    : skeleton (incomplete) code to show a DS1990 ID
SUBDIRS += 440hz timer servo sw-uart ds1990
   
# subdirectories - week 3 :
#    no example code for this week

# subdirectories - week 4 :
#    i2c       : i2c library (library, not a complete application)

# subdirectories - week 5 :
#    no example code for this week

# subdirectories - week 6 :
#    lcd5510   : show random lines on an LCD5510 display
SUBDIRS += lcd5510

# The Makefile.subdirs does the work
include ../../../Makefile.subdirs

