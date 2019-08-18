#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"

#include "calcl/calcl.hpp"
#include "calcl/plugin.hpp"
#include "calcl/read.hpp"
#include "calcl/repl.hpp"

using namespace std;
using namespace cidk;
using namespace calcl;

enum struct Mode {nil, load, repl};

int main(int argc, char *argv[]) {   
  Cx cx;
  cidk_pluginit(cx);
  Env env(cx, cx.env);
  Mode m(Mode::nil);

  while (--argc && ++argv) {
    string a(*argv);
    
    if (a == "-debug") {
      cx.debug = true;
    } else if (a == "-repl") {
      m = Mode::repl;
    } else {
      Ops ops;  
      Opts opts;
      cx.load(Pos::_, a, cidk::read_ops, env, ops, opts);
      cx.eval(ops, env, cx.regp);
      if (m == Mode::nil) { m = Mode::load; }
    }
  }

  if (m == Mode::nil || m == Mode::repl) { repl(cx, cin, cout); }
  if (cx.debug) { cx.deinit(); }
  return 0;
}
