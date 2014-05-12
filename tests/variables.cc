
#include <string>

int globalInt;

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

#include <tov8_variables.h>


