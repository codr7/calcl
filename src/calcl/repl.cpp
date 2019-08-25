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
      "calcl v" << VERSION << endl << endl <<
      "Press Return to evaluate." << endl <<
      "Empty input clears result and Ctrl+D exits." << endl << endl <<
      "  ";

    Env env(cx, cx.env);
    string line;

    while (getline(in, line)) {
      if (line.empty()) {
        cx.stackp = &cx.stack[0];
      } else {
        try {
          stringstream in(line);

          Pos p("n/a");
          Ops ops;
          Opts opts;
          read(cx, p, in, ops);
          cx.compile(ops, opts, env);

          for (auto &eid: opts.ext_ids) {
            throw ESys(eid.pos, "Unknown id: ", eid.id);
          }
          
          cx.eval(ops, env, opts);
        } catch (const exception &e) {
          out << e.what() << endl;
        }
      }
        
      if (cx.stackp > &cx.stack[0]) {
        out << cx.peek(Pos::_);
      } else {
        out << "n/a";
      }

      out << endl << "  ";
    }
  }
}
