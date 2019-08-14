#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/ops/call_bin.hpp"
#include "cidk/read.hpp"

#include "calcl/read.hpp"

namespace calcl {
  optional<Val> read_next(Cx &cx, Pos &pos, istream &in) {
    cidk::skip_ws(pos, in);
    
    if (char c(0); in.get(c)) {
      if (c == '-') {
        char c1(in.get());
        in.unget();
        in.unget();
        if (isdigit(c1)) { c = c1; }
      } else {
        in.unget();
      }
      
      if (isdigit(c)) { return read_num(cx, pos, in); }
      if (isgraph(c)) { return read_id(cx, pos, in); }
      throw ESys(pos, "Invalid input: ", c);
    }

    return {};
  }

  bool read_val(Cx &cx, Pos &pos, istream &in, Ops &out) {
    auto v(read_next(cx, pos, in));
    if (!v) { return false; }
    out.emplace_back(cx, pos, ops::Push, *v);
    return true;
  }

  bool read_expr2(Cx &cx, Pos &pos, istream &in, Ops &out) {
    Pos vp(pos);
    auto op(read_next(cx, pos, in));
    if (!op) { return false; }
    if (op->type != &cx.sym_type) { throw ESys(vp, "Invalid operation: ", *op); }
    
    vp = pos;
    if (!read_val(cx, pos, in, out)) { throw ESys(vp, "Missing right operand"); }
    out.emplace_back(cx, pos, ops::CallBin, *op);
    return true;
  }

  bool read_expr3(Cx &cx, Pos &pos, istream &in, Ops &out) {
    return read_val(cx, pos, in, out) ? read_expr2(cx, pos, in, out) : false;
  }

  void read(Cx &cx, Pos &pos, istream &in, Ops &out) {
    if (read_expr3(cx, pos, in, out)) { while (read_expr2(cx, pos, in, out)); }
  }
}
