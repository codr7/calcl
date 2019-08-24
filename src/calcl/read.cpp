#include "cidk/cx.hpp"
#include "cidk/ops/dispatch.hpp"
#include "cidk/ops/do.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"

#include "calcl/read.hpp"

namespace calcl {
  void read(Cx &cx, Pos &pos, istream &in, Ops &out) {
    if (read3(cx, pos, in, out)) { while (read2(cx, pos, in, out)); }
  }

  bool read3(Cx &cx, Pos &pos, istream &in, Ops &out) {
    bool ok(read_val(cx, pos, in, out));
    if (ok) { read2(cx, pos, in, out); }
    return ok;
  }

  bool read2(Cx &cx, Pos &pos, istream &in, Ops &out) {
    Pos vp(pos);
    auto op(read_next(cx, pos, in, out));
    if (!op) { return false; }
    
    vp = pos;
    if (!read_val(cx, pos, in, out)) { throw ESys(vp, "Missing right operand"); }
    out.emplace_back(cx, pos, ops::Dispatch, *op);
    return true;
  }

  bool read_val(Cx &cx, Pos &pos, istream &in, Ops &out) {
    auto v(read_next(cx, pos, in, out));
    if (!v) { return false; }
    out.emplace_back(cx, pos, ops::Push, *v);
    return true;
  }

  optional<Val> read_next(Cx &cx, Pos &pos, istream &in, Ops &out) {
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

      if (c == ')') { return {}; }

      if (isdigit(c)) { return read_num(cx, pos, in); }

      if (isgraph(c)) {
        Val id(read_id(cx, pos, in));
        cidk::skip_ws(pos, in);

        if (in.get(c)) {
          if (c == '=') {
            pos.col++; 
            Pos vp(pos);
            auto v(read_next(cx, pos, in, out));
            if (!v) { throw ESys(vp, "Missing value"); }
            out.emplace_back(cx, pos, ops::Let, id, *v);
            return read_next(cx, pos, in, out);
          }

          in.unget();
          return id;
        } else {
          return id;
        }
      }
      
      throw ESys(pos, "Invalid input: ", c);
    }

    return {};
  }

  Val read_group(Cx &cx, Pos &pos, istream &in) {
    Pos p(pos);
    Expr *out(cx.expr_type.pool.get(cx)), *body(cx.expr_type.pool.get(cx));
    out->ops.emplace_back(cx, p, ops::Do, Val(cx.expr_type, body));
    auto &bops(body->ops);
    char c(0);
    
    for (;;) {
      skip_ws(pos, in);
      if (!in.get(c)) { throw ESys(pos, "Open group"); }

      if (c == ')') {
        pos.col++;
        break;
      }

      in.unget();
      
      if (bops.empty()) {
        if (!read3(cx, pos, in, bops)) { throw ESys(pos, "Open group"); }
      } else {
        if (!read2(cx, pos, in, bops)) { throw ESys(pos, "Open group"); }
      }
    }

    return Val(cx.expr_type, out);
  }
}
