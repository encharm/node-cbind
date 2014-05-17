
#include <thread>
#include <future>

void (*cb)();

void callbackAfterMs(int ms) {
  std::async(std::launch::async, [=]{ 
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    cb();
  });
}

#include <build/cbind_asyncVariableCallback.h>