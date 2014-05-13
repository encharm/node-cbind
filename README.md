node-tov8
=========
[![Build Status](https://travis-ci.org/CodeCharmLtd/node-cbind.svg?branch=master)](https://travis-ci.org/CodeCharmLtd/node-tov8)

Streamlined automatic C/C++ v8 bindings generator for Node.JS inspired by tolua++ (requires C++11)

Core idea is the ability to easily bind to C/C++ by providing a header-like description of the native interface that is a subset of C++ (format is called Native Interface Description https://github.com/CodeCharmLtd/NID):

```c++

int global_variable;

std::string global_function(int a, float b);

struct Rect {
  int x;
  int y;
  int w;
  int h;
}
bool rectangles_intersect(Rect a, Rect b);

```

## Supported and tested features

* Handling basic types int, float, double, char*, const char*, std::string
* Handling pointers to basic types. Contains functions to create pointer values and dereference them.
* Binding to global variables with getter/setter.
* Binding to global functions.
* Type checking that throws exception to javascript in case of unmatched types.
* Attributes for specifying automatic free of returned arguments.
* Attributes for handling input buffer automatically and passing it to javascript as a return value.

## Implemented and initially tested

* Handling function pointer types. Function pointers are automatically converted to javascript functions and vice-versa. Function pointer variables as getter/setters and function pointers as arguments are equally handled.
* Handling constructors of structs/classes and binding them to javascript new.
* Handling class/struct variables and methods.
* Passing struct and class pointers to arbitrary functions.

## Planned features

* Attributes for calling native code asynchronously and returning values as promises (bluebird).
* Attributes for automatically handling native callbacks that have returned from a separate thread.

## Requirements

* C++11 support (GCC 4.8 or clang 3.3)
* Node 0.8, 0.10 or 0.11.13+

## How-to

Please open issues or follow example tests. README will be gradually expanded.
