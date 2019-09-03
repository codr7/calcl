#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/libs/abc.hpp"
#include "cidk/libs/math.hpp"

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
  Lib &abc(libs::init_abc(cx, Pos::_));
  cx.env.use(cx, Pos::_, abc.env);

  Lib &math(libs::init_math(cx, Pos::_));
  cx.env.use(cx, Pos::_, math.env);
  
  cidk_pluginit(cx, Pos::_);
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
      cx.load(Pos::_, a, cidk::read_ops, cx.env, ops, opts);
      Reg *eval_regs = cx.alloc_regs(opts.regs.size());
      cx.eval(ops, cx.env, eval_regs);

      for (Val *v(eval_regs); v < eval_regs + opts.regs.size(); v++) {
        if (v->type) { cx.env.set(cx, Pos::_, v->id, *v, true); }
      }

      cx.regp = eval_regs;
      if (m == Mode::nil) { m = Mode::load; }
    }
  }

  if (m == Mode::nil || m == Mode::repl) { repl(cx, cin, cout); }
  if (cx.debug) { cx.deinit(); }
  return 0;
}
