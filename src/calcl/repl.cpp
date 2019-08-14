#include "cidk/cx.hpp"
#include "cidk/env.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/pos.hpp"

#include "calcl/calcl.hpp"
#include "calcl/read.hpp"
#include "calcl/repl.hpp"

namespace calcl {
  void repl(Cx &cx, istream &in, ostream &out) {
    out <<
      "calcl v" << VERSION[0] << '.' << VERSION[1] << endl << endl <<
      "Press Return to evaluate." << endl <<
      "Empty input clears stack and Ctrl+D exits." << endl << endl <<
      "  ";

    Env env(cx, cx.env);
    Pos p("n/a");
    string line;

    while (getline(in, line)) {
      if (line.empty()) {
        cx.stackp = &cx.stack[0];
      } else {
        try {
          Ops ops;
          Opts opts;
          stringstream in(line);
          read(cx, p, in, ops);
          cx.compile(ops, opts, env);
          cx.eval(ops, env, opts, cx.regp);
        } catch (const exception &e) {
          out << e.what() << endl;
        }
      }
        
      out << cx.peek(p) << endl << "  ";
    }
  }
}
