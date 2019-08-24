#ifndef CALCL_READ_HPP
#define CALCL_READ_HPP

namespace calcl {
  using namespace cidk;

  void read(Cx &cx, Pos &pos, istream &in, Ops &out);
  bool read3(Cx &cx, Pos &pos, istream &in, Ops &out);
  bool read2(Cx &cx, Pos &pos, istream &in, Ops &out);
  bool read_val(Cx &cx, Pos &pos, istream &in, Ops &out);
  optional<Val> read_next(Cx &cx, Pos &pos, istream &in, Ops &out);
  Val read_group(Cx &cx, Pos &pos, istream &in);
}

#endif
