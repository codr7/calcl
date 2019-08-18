link "/usr/local/lib/libcalcl.so";

clock 1000 {
  calcl (35 + 7);
  check({cp;}) {is 42;};
};

dump;
dump stack;
mark; 
sweep;
