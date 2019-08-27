#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/types/expr.hpp"

#include "calcl/ops/calc.hpp"
#include "calcl/read.hpp"

namespace calcl::ops {
  const CalcType Calc("calc");

  CalcType::CalcType(const string &id): OpType(id) {}

  void CalcType::init(Cx &cx, Op &op, const Val &expr) const { op.args[0] = expr; }

  void CalcType::compile(Cx &cx,
                          OpIter &in,
                          const OpIter &end,
                          Env &env,
                          Ops &out,
                          Opts &opts) const {
    auto &p(in->pos);
    auto &expr(in->args[0]);
    expr.compile(p, env, opts);
    if (expr.type != &cx.expr_type) { throw ESys(p, "Invalid group", expr); }
    auto &ops(expr.as_expr->ops);
    copy(ops.begin(), ops.end(), back_inserter(out));
  }

  void CalcType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    skip_ws(pos, in);

    if (char c(0); !in.get(c) || c != '(') {
      throw ESys(p, "Invalid expression: ", c);
    }
    
    Val expr(read_group(cx, pos, in, false));
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, expr);
  }
}
