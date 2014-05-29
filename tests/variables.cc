
#include <string>

int globalInt;

namespace testns {
  int variableInNamespace;
}

namespace ns1 {
  int var1 = 1;
  namespace ns2 {
    int var2 = 2;
    namespace ns3 {
      int var3 = 3;
      namespace ns4 {
        int var4 = 4;
      }
    }
  }
}


bool verifyGlobalInt42() {
  return globalInt == 42;
}

float globalFloat;

bool verifyGlobalFloat43() {
  return globalFloat == 43.0;
}

double globalDouble;

bool verifyGlobalDouble44() {
  return globalDouble == 44.0;
}

const char* globalCString;

bool verifyGlobalCString45() {
  return globalCString && (strcmp(globalCString, "45") == 0);
}

std::string globalStdString;

bool verifyGlobalStdString46() {
  return globalStdString == "46";
}

int* globalPointerToInt;

bool verifyPointerToInt42() {
  return globalPointerToInt && *globalPointerToInt == 42;
}

float* globalPointerToFloat;

bool verifyPointerToFloat43() {
  return globalPointerToFloat && *globalPointerToFloat == 43;
}

double* globalPointerToDouble;

bool verifyPointerToDouble44() {
  return globalPointerToDouble && *globalPointerToDouble == 44;
}

#include <cbind_variables.h>


