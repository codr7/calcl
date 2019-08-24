#include "cidk/cx.hpp"

#include "calcl/ops/calc.hpp"
#include "calcl/plugin.hpp"

using namespace std;
using namespace calcl;

void cidk_pluginit(Cx &cx) {
  cx.add_op_type(ops::Calc);
}
