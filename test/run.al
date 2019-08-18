link "/usr/local/lib/libcalcl.so";

clock 1000 {
  calcl (35 + 7);
  check({cp;}) {is 42;};

  calcl ((1 + 2) * (3 + 4) * 2);
  check({cp;}) {is 42;};

  do {
    let foo 35;
    calcl (foo + 7);
    check({cp;}) {is 42;};
  };
};

dump;
dump stack;
mark; 
sweep;
