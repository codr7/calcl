#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/ops/call.hpp"
#include "cidk/read.hpp"

#include "calcl/read.hpp"

namespace calcl {
  struct ReadCx {
    const Sym *op;
    ValType *lhs_type;

    ReadCx(): op(nullptr), lhs_type(nullptr) {};
  };

  void read_next(Cx &cx, Pos &pos, ReadCx &rcx, istream &in, Ops &out) {
    cidk::skip_ws(pos, in);
    
    if (char c(0); in.get(c)) {
      in.unget();
      
      if (isdigit(c)) {
        Val v(read_num(cx, pos, in));
        out.emplace_back(cx, pos, ops::Push, v);

        if (rcx.lhs_type && rcx.op) {
          ValType *vt(v.type);
          
          auto fid((vt == rcx.lhs_type)
                   ? rcx.op
                   : cx.intern(str(rcx.op, '[', vt->id, ']')));
          
          out.emplace_back(cx, pos, ops::Call, rcx.lhs_type->env.get(pos, fid));
          rcx.op = nullptr;
          rcx.lhs_type = nullptr;
        } else {
          rcx.lhs_type = v.type;
        }
      } else if (isgraph(c)) {
        rcx.op = read_id(cx, pos, in).as_sym;
      } else {
        throw ESys(pos, "Invalid input: ", c);
      }
    }
  }

  void read(Cx &cx, Pos &pos, istream &in, Ops &out) {
    ReadCx rcx;
    while (!in.eof()) { read_next(cx, pos, rcx, in, out); }
  }
}
