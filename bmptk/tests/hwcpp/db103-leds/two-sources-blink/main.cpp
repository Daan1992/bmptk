#include "project.h"

int main(){	        
   std::cout.connect< target::uart<> >();
   TRACE << "two-sources blink test";
   timing::init();
   blink();
}

