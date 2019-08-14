#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/ops/call_bin.hpp"
#include "cidk/read.hpp"

#include "calcl/read.hpp"

namespace calcl {
  optional<Val> read_one(Cx &cx, Pos &pos, istream &in) {
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

  bool read_three(Cx &cx, Pos &pos, istream &in, Ops &out) {
    auto x(read_one(cx, pos, in));
    if (!x) { return false; }
    out.emplace_back(cx, pos, ops::Push, *x);

    Pos vp(pos);
    auto op(read_one(cx, pos, in));
    if (!op) { return false; }
    if (op->type != &cx.sym_type) { throw ESys(vp, "Invalid operation: ", *op); }
    
    vp = pos;
    auto y(read_one(cx, pos, in));
    if (!y) { throw ESys(vp, "Missing right operand"); }
    out.emplace_back(cx, pos, ops::Push, *y);

    out.emplace_back(cx, pos, ops::CallBin, *op);
    return true;
  }

  void read(Cx &cx, Pos &pos, istream &in, Ops &out) {
    while (read_three(cx, pos, in, out));
  }
}
