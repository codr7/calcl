#include "cidk/cx.hpp"
#include "cidk/ops/defconst.hpp"
#include "cidk/ops/defun.hpp"
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
    out.emplace_back(cx, pos, ops::Dispatch, *op, Val(cx.int_type, Int(2)));
    return true;
  }

  bool read_val(Cx &cx, Pos &pos, istream &in, Ops &out) {
    Pos p(pos);
    auto v(read_next(cx, pos, in, out));
    if (!v) { return false; }
    out.emplace_back(cx, p, ops::Push, *v);
    return true;
  }

  optional<Val> read_next(Cx &cx, Pos &pos, istream &in, Ops &out) {
    skip_ws(pos, in);
    
    if (char c(0); in.get(c)) {
      if (c == '_') {
        pos.col++;
        return cx.$;
      }

      if (c == '(') {
        pos.col++;
        return read_group(cx, pos, in);
      }
      
      if (c == '-') {
        char c1(in.get());
        if (c1) { in.unget(); }
        if (isdigit(c1)) { c = c1; }
      }

      in.unget();

      if (c == ')') {
        pos.col++;
        return {};
      }

      if (isdigit(c)) { return read_num(cx, pos, in); }

      if (isgraph(c)) {        
        Pos p(pos);
        Val id(read_id(cx, pos, in));
        bool is_const = false;
        
        if (id.as_sym == cx.intern(p, "const")) {
          is_const = true;
          skip_ws(pos, in);
          id = read_id(cx, pos, in);
        } else if (in.get(c) && c == '(') {
          pos.col++;
          auto args(read_list(cx, pos, in));
          auto &al(args.as_list->items);
          skip_ws(pos, in);
          
          if (!in.get(c) || c != '=') {
            in.unget();
            Expr *out(cx.expr_type.pool.get(cx));
            out->flags += Expr::INLINE;
            for (auto &v: al) { out->ops.emplace_back(cx, p, ops::Push, v); }
            
            out->ops.emplace_back(cx, p, ops::Dispatch,
                                  id, Val(cx.int_type, Int(al.size())));

            return Val(cx.expr_type, out);
          }
          
          if (!al.empty()) {
            for (auto i(al.begin()+1); i <= al.end(); i++) {
              if ((i = al.emplace(i, cx.meta_type, &cx.num_type)+1) == al.end()) {
                break;
              }
            };
          }

          List *rets(cx.list_type.pool.get(cx));
          rets->items.emplace_back(cx.meta_type, &cx.num_type);
          skip_ws(pos, in);

          if (!in.get(c) || c != '(') {
            throw ESys(p, "Invalid function body: ", c);
          }

          pos.col++;
          auto body(read_group(cx, pos, in));
          
          out.emplace_back(cx, p, ops::Defun,
                           id, args, Val(cx.list_type, rets), body);

          return read_next(cx, pos, in, out);
        } else if (!in.eof()) {
          in.unget();
        }
        
        skip_ws(pos, in);

        if (in.get(c)) {
          if (c == '=') {
            pos.col++; 
            Pos vp(pos);
            auto v(read_next(cx, pos, in, out));
            if (!v) { throw ESys(vp, "Missing value"); }

            if (is_const) {
              out.emplace_back(cx, p, ops::Defconst, id, *v);
            } else {
              out.emplace_back(cx, p, ops::Let, id, *v);
            }

            return read_next(cx, pos, in, out);
          }

          if (is_const) {
            throw ESys(p, "Missing assignment");
          }          
        }

        in.unget();
        return id;
      }
      
      throw ESys(pos, "Invalid input: ", c);
    }

    return {};
  }

  Val read_group(Cx &cx, Pos &pos, istream &in, bool env) {
    Pos p(pos);
    Expr *out(cx.expr_type.pool.get(cx));
    out->flags += Expr::INLINE;
    Ops *bops(nullptr);
    
    if (env) {
      Expr *body(cx.expr_type.pool.get(cx));
      out->ops.emplace_back(cx, p, ops::Do, Val(cx.expr_type, body));
      bops = &body->ops;
    } else {
      bops = &out->ops;
    }
    
    char c(0);
    
    for (;;) {
      skip_ws(pos, in);
      if (!in.get(c)) { throw ESys(p, "Open group"); }

      if (c == ')') {
        pos.col++;
        break;
      }

      in.unget();
      auto r(bops->empty() ? read3 : read2);
      r(cx, pos, in, *bops);
    }

    return Val(cx.expr_type, out);
  }
}
