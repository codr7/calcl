#ifndef CALCL_PLUGIN_HPP
#define CALCL_PLUGIN_HPP

namespace cidk {
  struct Cx;
  struct Pos;
}

using namespace cidk;

extern "C"
void cidk_pluginit(Cx &cx, const Pos &);

#endif
