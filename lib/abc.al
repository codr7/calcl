defun %(x Int y Int)(Int) {
  push x;
  cp;

  push y;
  call /[Int Int];

  push y;
  call *[Int Int];
  call -[Int Int];
};
