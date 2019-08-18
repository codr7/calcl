#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/ops/dispatch.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"

#include "calcl/read.hpp"

namespace calcl {  
  optional<Val> read_next(Cx &cx, Pos &pos, istream &in) {
    cidk::skip_ws(pos, in);
    
    if (char c(0); in.get(c)) {
      if (c == '(') {
        pos.col++;
        return read_group(cx, pos, in);
      }
      
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
    out.emplace_back(cx, pos, ops::Dispatch, *op);
    return true;
  }

  bool read_expr3(Cx &cx, Pos &pos, istream &in, Ops &out) {
    return read_val(cx, pos, in, out) ? read_expr2(cx, pos, in, out) : false;
  }

  void read(Cx &cx, Pos &pos, istream &in, Ops &out) {
    if (read_expr3(cx, pos, in, out)) { while (read_expr2(cx, pos, in, out)); }
  }

  Val read_group(Cx &cx, Pos &pos, istream &in) {
    Pos p(pos);
    Expr *out(cx.expr_type.pool.get(cx));
    char c(0);
    
    for (;;) {
      skip_ws(pos, in);
      if (!in.get(c)) { throw ESys(pos, "Open group"); }

      if (c == ')') {
        pos.col++;
        break;
      }

      in.unget();
      
      if (out->ops.empty()) {
        if (!read_expr3(cx, pos, in, out->ops)) { throw ESys(pos, "Open group"); }
      } else {
        if (!read_expr2(cx, pos, in, out->ops)) { throw ESys(pos, "Open group"); }
      }
    }

    return Val(cx.expr_type, out);
  }
}
