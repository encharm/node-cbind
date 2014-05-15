node-![cbind](https://raw.githubusercontent.com/CodeCharmLtd/node-cbind/master/cbind.png)
==============
[![Build Status](https://travis-ci.org/CodeCharmLtd/node-cbind.svg?branch=master)](https://travis-ci.org/CodeCharmLtd/node-cbind)
[![Package version](https://img.shields.io/npm/v/cbind.svg)](https://www.npmjs.org/package/cbind)
[![Package version](https://img.shields.io/npm/dm/cbind.svg)](https://www.npmjs.org/package/cbind)
[![Dependcies status](http://img.shields.io/gemnasium/CodeCharmLtd/node-cbind.svg)](https://gemnasium.com/CodeCharmLtd/node-cbind)

Streamlined automatic C/C++ v8 bindings generator for Node.JS inspired by tolua++ (requires C++11)

Core idea is the ability to easily bind to C/C++ by providing a header-like description of the native interface that is a subset of C++ (format is called Native Interface Description https://github.com/CodeCharmLtd/NID):

Simple things:
```c++

int global_variable;

std::string global_function(int a, float b);

struct Rect {
  int x;
  int y;
  int w;
  int h;
};
bool rectangles_intersect(Rect a, Rect b);

```

More complex things:
```c++
#include <stdio.h>

int errno; // make errno available as getter and setter

// make fopen available and mark FILE* pointers for automatic closing when gced
FILE * [[free(fclose)]] fopen(const char* filename, const char* mode);

// make fread available and automatically set one of its arguments
size_t fread(void* ptr [[buffer]], size_t size, size_t nmemb [[set(ptr.length / size)]], FILE* stream [[handle]]);

// tell cbind to mark the object containing pointer as invalid
int fclose(FILE* f [[clear_free,unref]]);
```


<a name="complex_weird_cases_function_pointer_handling">
## Complex weird cases function pointer handling
Consider this declaration (valid C and valid NID):
```c++
void (*catch_and_return(void (*callback)(char* a, char* b, int* c), char *name_one, char* name_two, int* number))(char* a, char * b, int c); 
```
Do you know what it does? Actually it is a declaration of function taking 4 arguments:
* `callback` - pointer to function `void(char* a, char* b, int* c)`
* `name_one` - string
* `name_two` - string
* `number` - pointer to number

This function returns another function pointer `void(char* a, char* b, int)`

It turns out cbind can generate bindings for this that fully work:
```js
bindings.catch_and_return(function(a, b, c) {
  console.log(a, b, cBind.derefInt(c));
}, "str1", "str2", cBind.createInt(30))("FINAL", "TEST", 44);
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

* Handle binary buffers as Buffer.
* Handling function pointer types. Function pointers are automatically converted to javascript functions and vice-versa. Function pointer variables as getter/setters and function pointers as arguments are equally handled.
* Handling constructors of structs/classes and binding them to javascript new.
* Handling class/struct variables and methods.
* Passing struct and class pointers to arbitrary functions.

## Planned features / Not supported yet

* Function overloading.
* Attributes for calling native code asynchronously and returning values as promises (bluebird).
* Attributes for automatically handling native callbacks that have returned from a separate thread.

## Requirements

* C++11 support (GCC 4.8 or clang 3.3)
* Node 0.8, 0.10 or 0.11.13+

## How-to use

* `npm install --save cbind`
* Create `example.nid` file
Put your native interface definition there, for example:

```
void hello(int b);
```

* Have your `binding.gyp` follow this example:
```
{
  "targets": [
    {
      "target_name": "cbind_example",
      "sources": [
        "src/addon.cc"
      ],
      "include_dirs"  : [
          "<!(node -e \"require('nan')\" 2> /dev/null)",
          "<!(node -e \"require('cbind')('example.nid')\" 2> /dev/null)"
      ],
      "cflags": ["-g", "-std=c++11"],
			"cflags_cc!": [ '-fno-exceptions' ]
    }
  ]
}
```
* Have your `addon.cc` the following content
```c++

#include <cstdio>

void hello(int a) {
  printf("Hello world: %i\n", a);
}

#include <cbind_example.h>

void init(v8::Handle<v8::Object> exports) {
  
  cbind::init_example(exports);
}

NODE_MODULE(tov8_example, init);
```

* Run `node-gyp rebuild`

* With hello.js having the following content:
```
var bindings = require('./build/Release/cbind_example.node');
bindings = hello(42);
```

* `node hello.js` should print "Hello world 42`

The above is in the repository https://github.com/CodeCharmLtd/cbind-example


Please open issues or follow example tests. README will be gradually expanded.


## Special thanks

https://github.com/celer/fire-ts - awesome templates for code generation


<a name="authors"/>
## Authors
* Damian Kaczmarek <damian@codecharm.co.uk> <rush@rushbase.net>

<a name="sponsors"/>
## Sponsors
Please open an issue if you would like a specific feature to be implemented and sponsored.

<a name="license"/>
## License
Copyright (c) 2013-2014 [Code Charm Ltd](http://codecharm.co.uk)

Licensed under the MIT license, see `LICENSE` for details.



