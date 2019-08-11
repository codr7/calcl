#include "cidk/cx.hpp"
#include "cidk/read.hpp"

#include "calcl/read.hpp"

namespace calcl {
  void read(Cx &cx, Pos &pos, istream &in, Ops &out) {
    while (!in.eof()) { read_next(cx, pos, in, out); }
  }

  void read_next(Cx &cx, Pos &pos, istream &in, Ops &out) {
    cidk::skip_ws(pos, in);
    
    if (char c(0); in.get(c)) {
      in.unget();
      
      if (isdigit(c)) {
        cx.push(pos, read_num(cx, pos, in));
      } else if (isgraph(c)) {
        auto id(cidk::read_id(cx, pos, in));
        cout << "id: " << id << endl;
      } else {
        throw ESys(pos, "Invalid input: ", c);
      }
    }
  }
}
