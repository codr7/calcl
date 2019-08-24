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

  calc(7 + (a = (3 + 4) a * 5));
  check({cp;}) {is 42;};

  calc(a = 1 (a = 2 a) + a);
  check({cp;}) {is 3;};

  calc(const pi = 3.14 pi);
  check({cp;}) {is 3.14;};
};

dump;
dump stack;
mark; 
sweep;
