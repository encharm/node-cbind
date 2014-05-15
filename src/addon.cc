#include <nan.h>
#include <string>
#include <cstdio>

#include <node_buffer.h>
#include <functional>

template<class T>
T* create(T in) {
  return new T(in);
}

template<class T>
T deref(T* in) {
  return *in;
}

#include <cbind_core.h>

#define CBIND_CONCAT(a, b) a ## b
#define CBIND_QUOTE(str) #str
#define CBIND_EXPAND_AND_QUOTE(str) CBIND_QUOTE(str)

#ifdef DEBUG // defined only in npm test

#include <tests/variables.cc>
#include <tests/functions.cc>
#include <tests/clib.cc>
#include <tests/functionPointers.cc>

#endif


#define INIT_TEST(name) \
  { \
    auto object = NanNew<v8::Object>(); \
    cbind:: CBIND_CONCAT(init_, name)(object); \
    exports->Set(NanNew<v8::String>(CBIND_QUOTE(name)), object); \
  }
void init(v8::Handle<v8::Object> exports) {

  #ifdef DEBUG
  INIT_TEST(variables)
  INIT_TEST(functions)
  INIT_TEST(functionPointers)
  INIT_TEST(clib)
  #endif

  cbind::init_core(exports);
}

NODE_MODULE(cbind_core, init);
