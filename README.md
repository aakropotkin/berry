# Berry

## Introduction

Berry is a ultra-lightweight dynamically typed embedded scripting language. Berry is designed to run on lower-performance embedded devices. The Berry interpreter-core's code size is less than 40KiB and can run on less than 4KiB heap (on ARM Cortex M4 CPU, Thumb ISA and ARMCC compiler).

The interpreter of Berry include a one-pass compiler and register-based VM, all the code is written in ANSI C99. In Berry not every type is a class object. Some simple value types, such as int, real, boolean and string are not class object, but list, map and range are class object. This is a consideration about performance. Register-based VM is also based on this consideration.

Berry has the following advantages:

* Lightweight: smaller than mainstream scripting languages, ideal for use on microcontrollers.
* Fast: optimized one-pass bytecode compiler and register-based virtual machine.
* Powerful: supports imperative programming, object-oriented programming, functional programming.
* Flexible: Berry is a dynamic type script, and it's intended for embedding in applications. It can provide good dynamic scalability for the host system.
* Simple: simple and natural syntax, support garbage collection, and easy to use FFI (foreign function interface).

## Documents

[https://skiars.github.io/berry](https://skiars.github.io/berry)

[https://github.com/Skiars/berry_doc](https://github.com/Skiars/berry_doc)

[berry_rm_zh_cn.pdf](https://github.com/Skiars/berry_doc/releases/download/latest/berry_rm_zh_cn.pdf) (Simplified Chinese version of the Reference Manual)

## Features

* Base Type
  * Numerical value: Integer (`int`) and Real (`real`)
  * Boolean: `true` and `false`
  * String: Single quotation-mark string and double quotation-mark string
  * List: Continuous memory list, like `[1, 2, 3]`
  * Map: Hash Map, like `{ 'a': 1, 2: 3, 'map': {} }`
  * Range: include a lower and a upper integer value, like `0..5`
* Operator and Expression
  * Assign operator: `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
  * Relational operator: `<`, `<=`, `==`, `!=`, `>`, `>=`
  * Logic operator: `&&`, `||`, `!`
  * Arithmetic operator: `+`, `-`, `*`, `/`, `%`
  * Bitwise operator: `&`, `|`, `~`, `^`, `<<`, `>>`
  * Field operator: `.`
  * Subscript operator: `[]`
  * Connect string operator: `+`
  * Brackets: `()`
* Control Structure
  * Conditional statement: `if-else`
  * Iteration statement: `while` and `for`
  * Jump statement: `break`
* Function
  * Local variable and block scope
  * Return statement
  * Nested functions definition
  * Closure based on Upvalue
  * Anonymous function
* Class
  * Inheritance (only public single inheritance)
  * Method and Operator Overload
  * Constructor method
* Module Management
  * Statically allocated memory: no RAM used before import.
  * Module nesting.

## Build

1. install the readline library (Linux or MacOS)

``` bash
sudo apt install libreadline-dev # Ubuntu
brew install readline            # MacOS
```

2. Build:

```
make
```

3. Run:

``` bash
./berry # Bash or PowerShell
berry   # Windows CMD
```

## Editor pulgins

[Visual Studio Code](https://code.visualstudio.com/) pulgin are in this directory: [./tools/pulgins/vscode](./tools/pulgins/vscode).

# Examples

After compiling successfully, use the `berry` command with no parameters to enter the REPL environment:
```
Berry 0.0.1 (build in Dec 24 2018, 18:12:49)
[GCC 8.2.0] on Linux (default)
>
```

Now enter this code:

``` lua
print("Hello world!")
```

You will see this output:

```
Hello world!
```

You can copy this code to the REPL:

```ruby
def list_iter(list)
    index = 0
    def it()
        value = list[index]
        index = index + 1
        return value
    end
    return it
end
l = [0, 1, 2, 3, 4, 5]
lout = []
it = list_iter(l)
v = it()
while (v != nil)
    lout.append(v)
    v = it()
end
print(lout)
```

This examples is a simple list iterator. Let's look at the output:

```
[0, 1, 2, 3, 4, 5]
```

You can save the above code to a file (eg test.be) and run:

``` bash
./berry test.be
```

This will also get the correct output.
