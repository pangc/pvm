#include <iostream>
#include "vm.h"
#define vmerror(x) printf(x)
using namespace std;

int main()
{
    VMachine v;
    v.Reset();
    v.Read();
    v.Execute();
    v.Reset();

    return 0;
}
