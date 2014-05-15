


void print_concat(char* a, char* b, int c)
{
  printf("Join 3: %s %s %i\n", a, b, c);
}

void (*custom_print)(char* a, char* b, int c) = print_concat;


void (*catch_and_return(void (*pf)(char* aarg, char* barg, int* ccarg), const char *name_one, const char* name_two, int* number))(char* a, char * b, int c)
{
  static int value = 42;
  pf((char*)"hello", (char*)"my", &value);
  return custom_print;
}


void pf(char* aarg, char* barg, int* ccarg) {

}

void test()
{
  int d = 5;
  (catch_and_return(pf,"b", "c", &d));
}


#include <build/cbind_functionPointers.h>