#include "Vector.h"
#include <iostream>

using namespace std;

int main(void)
{
    vector v(3);
    v.seti(0, 1);
    v.seti(1, 2);
    v.seti(2, 3);
    cout << "v=" << endl;
    v.print();
    vector u(v);
    cout << "u=" << endl;
    u.print();
    return(0);
}
