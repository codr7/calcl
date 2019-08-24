![Logo](logo.png)

### Intro
calcl is a programmatic calculator, an extensible programming language designed for calculations.

### Setup
calcl requires CMake, a C++17 compiler and [cidk](https://github.com/codr7/cidk) to build.

```
$ git clone https://github.com/codr7/cidk.git
$ cd cidk
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
$ cd ../..
$ git clone https://github.com/codr7/calcl.git
$ cd calcl
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
$ rlwrap calcl
calcl v0.1

Press Return to evaluate.
Empty input clears result and Ctrl+D exits.

  2 + (4 * 10)
42
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

Previous result may be accessed using `$`.

```
  1
1
  $ + (2 * 3)
7
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
Error in "n/a" at row 1, col 23:
Const let: foo
n/a

  (foo = 42 foo)
42
```

### License
MIT