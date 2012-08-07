#include <iostream>
#include "ptr.h"

class A
{
    public:
        A(){ std::cout << "+++ A constructor" << std::endl; }
        ~A(){ std::cout << "--- A destructor" << std::endl; }
};

typedef ptr<A>::shared_ptr A_ptr;
typedef ptr<A>::shared_array_ptr A_array_ptr;

int main( int argc, char **argv )
{
    // Single element shared pointer
    {
        A_ptr a( new A);
    }

    std::cout << "-----------------------" << std::endl;

    // Multiple element (array) shared pointer
    {
        A_array_ptr a( new A[2]);
    }
    return 0;
}


