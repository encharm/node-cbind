#ifndef TOV8_HELPERS
#define TOV8_HELPERS  

  #include "type_literal.h" // TYPE_LITERAL macro for making aligned type string for boxing pointers

  class v8_exception : public std::exception
  {
    std::string msg;
  public:
    v8_exception(const std::string& _msg) : msg(_msg) {
    }
    virtual const char* what() const throw() {
      return msg.c_str();
    }
  };


  template<class T>
  class FunctionWrap : public node::ObjectWrap {
    std::function<void(T)> deleter;
    T m_handle = nullptr;
    FunctionWrap(T handle, std::function<void(T)> _deleter): deleter(_deleter), m_handle(handle) {

    }
    static void wrap(v8::Local<v8::Object>& obj, T in, std::function<void(T)> deleter = [](T){} ) {
      (new FunctionWrap<T>(in, deleter))->Wrap(obj);
    }
  public:

    static T nativeFromV8(v8::Handle<v8::Value> obj, const char* typeId, v8::Persistent<v8::Function>& persistent, T cbNative) {
      printf("<< Native from v8!!\n");
      if(!obj->IsFunction())
        throw v8_exception("Argument needs to be a Function");
      printf(">> Native from v8: %s!!\n", typeId);
      v8::Local<v8::Function> func = obj.As<v8::Function>();
      NanAssignPersistent(persistent, func);

      return cbNative;
    }

    static v8::Handle<v8::Function> create(const char* typeId, T in, NanFunctionCallback cb, std::function<void(T)> deleter = [](T){} ) {
      v8::Local<v8::FunctionTemplate> funTpl = NanNew<v8::FunctionTemplate>(cb);

      v8::Local<v8::Function> fun = funTpl->GetFunction();

      v8::Local<v8::Object> prototype = fun->GetPrototype().As<v8::Object>();

      v8::Local<v8::Function> bind = prototype->Get(NanSymbol("bind")).As<v8::Function>();
      
      v8::Local<v8::ObjectTemplate> objTpl = v8::ObjectTemplate::New();
      objTpl->SetInternalFieldCount(1);
      v8::Local<v8::Object> obj = objTpl->NewInstance();

      wrap(obj, in, deleter);

      v8::Handle<v8::Value> argv[1] = {obj};

      fun = bind->Call(fun, 1, argv).As<v8::Function>();

      fun->Set(NanSymbol("__tov8_wrapped_data"), obj);

      fun->Set(NanSymbol("nativeInterface"), NanNew<v8::String>(typeId));
      printf("Returning function %s\n", typeId);

      return fun;
    }
    void clear_free() {
      m_handle = nullptr;
      deleter = nullptr;
    }
    virtual ~FunctionWrap() {
      if(deleter)
        deleter(m_handle);
    }

    T get() const {
      return m_handle;
    }
  };


  template<class T>
  class HandleWrap : public node::ObjectWrap {
    std::function<void(T)> deleter;
    T m_handle;

    static void wrap(v8::Local<v8::Object>& obj, T in, std::function<void(T)> deleter = [](T){} ) {
      (new HandleWrap<T>(in, deleter))->Wrap(obj);
    }
  public:
    HandleWrap(T handle, std::function<void(T)> _deleter): deleter(_deleter), m_handle(handle) {

    }


    static v8::Handle<v8::Object> createSafe(const char* typeString, T in, std::function<void(T)> deleter = [](T){} ) {
      v8::Local<v8::ObjectTemplate> objTpl = v8::ObjectTemplate::New();
      objTpl->SetInternalFieldCount(2);

      v8::Local<v8::Object> obj = objTpl->NewInstance();
//      obj->Set(NanNew<v8::String>("tov8Type"), NanNew<v8::String>(typeString.c_str()));

      NanSetInternalFieldPointer(obj, 1, (void*)typeString);

      wrap(obj, in, deleter);
      return obj;
    }

    static v8::Handle<v8::Object> create(T in, std::function<void(T)> deleter = [](T){} ) {
      return createSafe("", in, deleter);
    }

    void clear_free() {
      m_handle = nullptr;
      deleter = nullptr;
    }
    virtual ~HandleWrap() {
      if(deleter)
        deleter(m_handle);
    }

    static T Unwrap(v8::Handle<v8::Object> obj, const char* key = NULL) {
      void *typeStringPtr = (obj->InternalFieldCount() == 2)?NanGetInternalFieldPointer(obj, 1):nullptr;
      if(!typeStringPtr) {
        throw v8_exception("Unable to unbox pointer from object, not created by tov8");
      }
      if(key) {
        if(strcmp(key, (char*)typeStringPtr) != 0) {
          throw v8_exception(std::string("Unable to unbox pointer ") + key + " from pointer of type " + (const char*)typeStringPtr);
        }
      }
      return node::ObjectWrap::Unwrap<HandleWrap<T>>(obj)->get();
    }

    T get() const {
      return m_handle;
    }
  };

  template<typename T>
  struct auto_delete {
    T obj;
    auto_delete(T _obj) : obj(_obj) {}
    operator T() {
      return obj;
    }
    ~auto_delete() {
      delete obj;
    }
  };
  template<typename T>
  struct auto_deleteA {
    T obj;
    auto_deleteA(T _obj) : obj(_obj) {}
    operator T() {
      return obj;
    }
    ~auto_deleteA() {
      delete[] obj;
    }
  };
  template<typename T>
  struct auto_free {
    T obj;
    auto_free(T _obj) : obj(_obj) {}
    operator T() {
      return obj;
    }
    ~auto_free() {
     free((void*)obj);
    }
  };

// types

template<typename T>
  bool checkArgument(v8::Local<v8::Value> obj) {
    return true;
  }
  template<typename T>
  T getArgument(v8::Local<v8::Value> obj) {
    return T();
  }
  template<typename T>
  v8::Handle<v8::Value> toV8Type(T input) {
    return NanUndefined();
  }

  // type int
  template<>
  bool checkArgument<int>(v8::Local<v8::Value> obj) {
    if(!obj->IsNumber())
      return false;
    return true;
  }
  template<>
  int getArgument<int>(v8::Local<v8::Value> obj) {
    return obj->NumberValue();
  }
  
  v8::Handle<v8::Value> toV8Type_int(int input) {
    return NanNew<v8::Number>(input);
  }

  // type int
  template<>
  bool checkArgument<bool>(v8::Local<v8::Value> obj) {
    if(!obj->IsBoolean())
      return false;
    return true;
  }
  template<>
  bool getArgument<bool>(v8::Local<v8::Value> obj) {
    return obj->BooleanValue();
  }
  
  v8::Handle<v8::Value> toV8Type_bool(bool input) {
    return NanNew<v8::Boolean>(input);
  }

  // type int
  template<>
  bool checkArgument<size_t>(v8::Local<v8::Value> obj) {
    if(!obj->IsNumber())
      return false;
    return true;
  }
  template<>
  size_t getArgument<size_t>(v8::Local<v8::Value> obj) {
    return obj->NumberValue();
  }
  
  v8::Handle<v8::Value> toV8Type_size_t(size_t input) {
    return NanNew<v8::Number>(input);
  }

  // type float
  template<>
  bool checkArgument<float>(v8::Local<v8::Value> obj) {
    if(!obj->IsNumber())
      return false;
    return true;
  }
  template<>
  float getArgument<float>(v8::Local<v8::Value> obj) {
    return obj->NumberValue();
  }
  
  v8::Handle<v8::Value> toV8Type_float(float input) {
    return NanNew<v8::Number>(input);
  }
  // type double
  template<>
  bool checkArgument<double>(v8::Local<v8::Value> obj) {
    if(!obj->IsNumber())
      return false;
    return true;
  }
  template<>
  double getArgument<double>(v8::Local<v8::Value> obj) {
    return obj->NumberValue();
  }
 
  v8::Handle<v8::Value> toV8Type_double(double input) {
    return NanNew<v8::Number>(input);
  }

  // type string
  template<>
  bool checkArgument<std::string>(v8::Local<v8::Value> obj) {
    if(!obj->IsString())
      return false;
    return true;
  }
  template<>
  std::string getArgument<std::string>(v8::Local<v8::Value> obj) {
    size_t count;
    const char* str = NanCString(obj, &count);
    std::string out = std::string(str);
    delete[] str;
    return out;
  }
  
  v8::Handle<v8::Value> toV8Type_std__string(std::string input) {
    return NanNew<v8::String>(input.c_str());
  }




  // type const char*
  template<>
  bool checkArgument<const char*>(v8::Local<v8::Value> obj) {
    if(!obj->IsString())
      return false;
    return true;
  }
  template<>
  const char* getArgument<const char*>(v8::Local<v8::Value> obj) {
    size_t count;
    return NanCString(obj, &count);
  }
  
  v8::Handle<v8::Value> toV8Type_const_char_ptr(const char* input) {
    if(!input) {
      return NanNull();
    }
    return NanNew<v8::String>(input);
  }



  template<>
  bool checkArgument<char*>(v8::Local<v8::Value> obj) {
    if(!obj->IsString())
      return false;
    return true;
  }
  template<>
  char* getArgument<char*>(v8::Local<v8::Value> obj) {
    size_t count;
    return NanCString(obj, &count);
  }
  
  v8::Handle<v8::Value> toV8Type_char_ptr(char* input) {
    return NanNew<v8::String>(input);
  }



#endif // TOV8_HELPERS