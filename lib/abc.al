defun %(x Num y Num)(Num) {
  push x;
  cp;

  push y;
  dispatch /;

  push y;
  dispatch *;
  dispatch -;
};

defun %2(x Num y Num)(Num) { calc(x - ((x / y) * y)); };

calc(%3(x y) = (x - ((x / y) * y)));

