#include "cidk/cx.hpp"

#include "calcl/ops/calc.hpp"
#include "calcl/plugin.hpp"

using namespace std;
using namespace calcl;

void cidk_pluginit(Cx &cx) {
  cx.op_types.emplace("calc", &ops::Calc);
}
