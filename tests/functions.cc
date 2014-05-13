#include <string>

int callCounter = 0;

void _voidFunction() {
  callCounter++;
}

int _intFunction() {
  callCounter++;
  return 42;
}

float _floatFunction() {
  callCounter++;
  return 42.0;
}

double _doubleFunction() {
  callCounter++;
  return 42.0;
}

const char* cStringFunction() {
  callCounter++;
  return "hello";
}

std::string stdStringFunction() {
  callCounter++;
  return "hello";
}

int functionTakingInt(int a) {
  callCounter++;
  return a;
}

float functionTakingFloat(float a) {
  callCounter++;
  return a;
}

double functionTakingDouble(double a) {
  callCounter++;
  return a;
}

const char* functionTakingCString(const char* a) {
  callCounter++;
  return a;
}

std::string functionTakingStdString(std::string a) {
  callCounter++;
  return a;
}

std::string functionConcat(int a, float b, double c, const char* d, std::string e) {
  callCounter++;
  char buffer[4096];
  snprintf(buffer, sizeof(buffer), "%i,%1.0f,%1.0lf,%s,%s",a, b, c, d, e.c_str());
  return buffer;
}

void custom_free(void* ptr) {
  callCounter++;
  free(ptr);
}

char* functionConcat2(int a, float b, double c, const char* d, std::string e) {
  char buffer[4096];
  snprintf(buffer, sizeof(buffer), "%i,%1.0f,%1.0lf,%s,%s",a, b, c, d, e.c_str());
  return strdup(buffer);
}

void functionConcat3(int a, float b, double c, const char* d, std::string e, char* out, size_t outLen) {
  callCounter++;
  snprintf(out, outLen, "%i,%1.0f,%1.0lf,%s,%s",a, b, c, d, e.c_str());  
}

int functionConcat4(int a, float b, double c, const char* d, std::string e, char* out, size_t outLen) {
  callCounter++;
  return snprintf(out, outLen, "%i,%1.0f,%1.0lf,%s,%s",a, b, c, d, e.c_str());  
}


int getCounter() {
  return callCounter;
}

void resetTestData() {
  callCounter = 0;
}


#include <cbind_functions.h>