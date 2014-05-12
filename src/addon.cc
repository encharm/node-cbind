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

#include <tov8_core.h>

#ifdef DEBUG // defined only in npm test

#include <tests/variables.cc>

#endif

void init(v8::Handle<v8::Object> exports) {
  #ifdef DEBUG
  {
    auto object = NanNew<v8::Object>();
    tov8::init_variables(object);
    exports->Set(NanNew<v8::String>("variables"), object);
  }
  #endif

  tov8::init_core(exports);
}

NODE_MODULE(tov8_core, init);
