#include "foo.h"
#include "stddef.h"

int test_foo()
{
  size_t array[] = {0,1,2,3};
  size_t i = 2;
  return array[i];
}
