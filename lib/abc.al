defun %(x Num y Num)(Num) {
  push x;
  cp;

  push y;
  dispatch /;

  push y;
  dispatch *;
  dispatch -;
};

defun %%(x Num y Num)(Num) { calc(x - ((x / y) * y)); };

