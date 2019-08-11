#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/repl.hpp"

#include "calcl/calcl.hpp"
#include "calcl/read.hpp"

using namespace std;
using namespace cidk;
using namespace calcl;

int main(int argc, char *argv[]) {   
  Cx cx;
  Env env(cx, cx.env);
  
  while (--argc && ++argv) {
    string a(*argv);
    
    if (a == "-debug") { cx.debug = true; }
    else {
      Ops ops;  
      Opts opts;
      cx.load(Pos::_, a, cidk::read_ops, env, ops, opts);
      cx.eval(ops, env, cx.regp);
    }
  }

  repl(cx,
       str("calcl v", calcl::VERSION[0], '.', calcl::VERSION[1]),
       calcl::read,
       cin, cout);
  
  if (cx.debug) { cx.deinit(); }
  return 0;
}
