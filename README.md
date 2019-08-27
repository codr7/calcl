![Logo](logo.png)

### Intro
calcl is a programmatic calculator, an extensible programming language designed for calculations.

```
$ rlwrap calcl

Press Return to evaluate.
Empty input clears stack and Ctrl+D exits.

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
Operands are required to be separated by whitespace and are evaluated left to right.

```
  1 + 2 * 3
9
```

Groups may be used to change evaluation order.

```
  1 + (2 * 3)
7
```

Results may be referenced using `_`.

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

### Functions
Functions may be defined by specifying an (optionally empty) argument list directly following the bound id. Any binary function may be used as operator.

```
  mod(x y) = (x - ((x / y) * y))
n/a
  1.0 mod 0.3
0.1
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
The functionality of calcl may be extended using [cidk](https://github.com/codr7/cidk) assembler, or any language capable of emitting the same. The following example implements a generic modulo operator in assembler.

[lib/abc.al](lib/abc.al)
```
defun %(x Num y Num)(Num) {
  push x;
  cp;

  push y;
  dispatch /;

  push y;
  dispatch *;
  dispatch -;
};
```

```
$ rlwrap calcl lib/abc.al -repl

Press Return to evaluate.
Empty input clears stack and Ctrl+D exits.

  1.0 % 0.3
0.1
```

A more convenient, but slightly slower, option is using the `calc` opcode. The reason for the slowdown is an extra push of `x`, which is simplified to `cp` above; something the current compiler isn't smart enough to do. Besides that minor difference, the generated code looks mostly the same and is inlined at compile time.

```
defun %(x Num y Num)(Num) { calc(x - ((x / y) * y)); };
```

In many cases, the entire definition may be expressed within calcl.

```
calc(%(x y) = (x - ((x / y) * y)))
```

### License
MIT