// =======================================================================
//
// featherweight multitasking demo
//
// =======================================================================
   
#ifndef BMPTK_TARGET_db103
   #error "this is a DB103 project"
#endif   

#include "targets/lpc1114fn28.hpp"

    class MyException
    {
    public:
        // Do me a virtual table, please 
        // virtual ~MyException() {}
    };

    int main()
    {
        throw MyException{};
    } 
