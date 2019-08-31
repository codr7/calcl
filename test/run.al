clock 1000 {
  calc(35 + 7);
  check({cp;}) {is 42;};

  calc((1 + 2) * (3 + 4) * 2);
  check({cp;}) {is 42;};

  push 35;
  calc(_ + 7);
  check({cp;}) {is 42;};
  
  do {
    let foo 35;
    calc(foo + 7);
    check({cp;}) {is 42;};
  };

  calc(7 + (a = (3 + 4) a * 5));
  check({cp;}) {is 42;};

  calc((a = 1 (a = 2 a) + a));
  check({cp;}) {is 3;};

  calc(const pi = 3.14 pi);
  check({cp;}) {is 3.14;};

  do {
      calc(add-seven(x) = (x + 7));
      calc(add-seven(35));
      check({cp;}) {is 42;};
  };

  do {
      calc(mod(x y) = (x - ((x / y) * y)));
      calc(1.0 mod 0.3);
      check({cp;}) {is 0.1;};
  };

  calc(7 ^ 2);
  check({cp;}) {is 49;};

  calc(sqrt(49));
  check({cp;}) {is 7;};
};

dump;
dump stack;
mark; 
sweep;
