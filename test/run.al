clock 1000 {
  calc(35 + 7);
  check({cp;}) {is 42;};

  calc((1 + 2) * (3 + 4) * 2);
  check({cp;}) {is 42;};

  do {
    let foo 35;
    calc(foo + 7);
    check({cp;}) {is 42;};
  };
};

dump;
dump stack;
mark; 
sweep;
