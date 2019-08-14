#ifndef CALCL_REPL_HPP
#define CALCL_REPL_HPP

#include <iostream>

namespace cidk {
  struct Cx;
}

namespace calcl {
  using namespace std;
  
   void repl(Cx &cx, istream &in, ostream &out);
}

#endif
