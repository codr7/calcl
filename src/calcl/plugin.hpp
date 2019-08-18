#ifndef CALCL_PLUGIN_HPP
#define CALCL_PLUGIN_HPP

namespace cidk {
  struct Cx;
}

using namespace cidk;

extern "C"
void cidk_pluginit(Cx &cx);

#endif
