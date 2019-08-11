#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/repl.hpp"

#include "calcl/calcl.hpp"

using namespace std;
using namespace cidk;
using namespace calcl;

enum struct Mode {load, repl};

int main(int argc, char *argv[]) {
  const Pos &p(Pos::_);
  Mode m(Mode::repl);
  
  Cx cx;
  Env env(cx, cx.env);
  
  while (--argc && ++argv) {
    string a(*argv);
    
    if (a == "-debug") { cx.debug = true; }
    else {
      Ops ops;  
      Opts opts;
      cx.load(p, a, env, ops, opts);
      cx.eval(ops, env, cx.regp);
      m = Mode::load;
    }
  }

  if (m == Mode::repl) {
    repl(cx, str("calcl v", calcl::VERSION[0], '.', calcl::VERSION[1]), cin, cout);
  }
  
  if (cx.debug) { cx.deinit(); }
  return 0;
}
