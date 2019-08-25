![Logo](logo.png)

### Intro
calcl is a programmatic calculator, an extensible programming language designed for calculations.

```
$ rlwrap calcl
calcl v0.1

Press Return to evaluate.
Empty input clears result and Ctrl+D exits.

  2 + (4 * 10)
42
```

### Setup
calcl requires CMake, a C++17 compiler and [cidk](https://github.com/codr7/cidk) to build.

```
$ git clone https://github.com/codr7/cidk.git
$ cd cidk
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
$ cidk ../test/run.al
39
()
```

```
$ git clone https://github.com/codr7/calcl.git
$ cd calcl
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
$ calcl ../test/run.al
3
()
```

### Semantics
Operands are required to be separated by whitespace and are read and processed left to right.

```
  1 + 2 * 3
9
```

Groups may be used to control evaluation order.

```
  1 + (2 * 3)
7
```

The result may be referenced using `_`.

```
  1
1
  _ + (2 * 3)
7
```

Decimal numbers are implemented as fix point, and infer precision from literals.

```
  41.90 + 0.1
42.00
```


### Bindings
Values may be bound to names within the current group using `=`.

```
  7 + (foo = (3 + 4) foo * 5)
42
```

#### Constants
Prefixing names with `const` defines compile time constants.

```
  const pi = 3.14 pi
3.14
```

Rebinding a constant results in a compile time error.

```
  (const foo = 7 (foo = 42 foo))
Error in "n/a" at row 1, col 16:
Const let: foo
n/a

  (foo = 42 foo)
42
```

### Linking
calcl implements a [cidk](https://github.com/codr7/cidk)-plugin with a custom opcode that may be linked into any program for easy integration. 

test.al
```
  link "/usr/local/lib/libcalcl.so";
  calc(35 + 7);
  dump;
```

```
$ cidk test.al
42
```

### Extending
The functionality of calcl may be extended using [cidk](https://github.com/codr7/cidk) assembler, or any language capable of emitting the same. The following example implements a modulo operator in assembler.

[lib/abc.al](lib/abc.al)
```
defun %(x Int y Int)(Int) {
  push x;
  cp;

  push y;
  call /[Int Int];

  push y;
  call *[Int Int];
  call -[Int Int];
};
```

```
$ rlwrap calcl lib/abc.al -repl
calcl v0.1

Press Return to evaluate.
Empty input clears result and Ctrl+D exits.

  10 % 3
1
```

### License
MIT