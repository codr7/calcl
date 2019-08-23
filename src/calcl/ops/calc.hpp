#ifndef CALCL_OP_CALC_HPP
#define CALCL_OP_CALC_HPP

#include "cidk/op.hpp"

namespace calcl::ops {
  using namespace cidk;
  
  struct CalcType: OpType {
    CalcType(const string &id);

    void init(Cx &cx, Op &op, const Val &expr) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const CalcType Calc;
}

#endif
