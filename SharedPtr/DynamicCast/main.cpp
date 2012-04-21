#include <stdio.h>
#include <map>
#include <vector>
#include <tr1/memory>
#include <Chaskii/Mem/ref_ptr.h>

template <typename T>
struct ptr{
#ifdef USE_STD_SHARED_PTR
    typedef std::tr1::shared_ptr<T> shared_ptr;
#elif USE_CHASKII_MEM_REF_PTR
    typedef iiMem::ref_ptr<T> shared_ptr;
#endif
};

class B;
typedef ptr<B>::shared_ptr B_ptr;

class A 
{
    public:
        enum Category {
            one,
            two,
            three
        };

        A( Category cat ): category(cat) {}

        Category getCategory() { return category; }
        virtual void apply( B_ptr & );

    private:
        Category category;
};

typedef ptr<A>::shared_ptr A_ptr;

class oneA : public A
{
    public:
        oneA(): A(one) {}
};

typedef ptr<oneA>::shared_ptr oneA_ptr;

class twoA : public A
{
    public:
        twoA(): A(two) {}
};

class threeA: public A
{
    public:
        threeA(): A(three) {}
};

class B 
{
    public:
        B() {}

        void addA( A_ptr a )
        {
            A::Category c = a->getCategory();
            if( c == A::one )
            {
                oneA_ptr o ( dynamic_cast<oneA *>(a.get()) );
                oneAs.push_back( o );
            }
            as[c] = a;
        }

    private:
        std::map< A::Category, A_ptr > as;
        std::vector< oneA_ptr > oneAs;
};

void A::apply( B_ptr &b )
{
    b->addA( A_ptr(this) );
}


int main( int argc, char **argv )
{
    B_ptr b( new B );
    A_ptr a( new oneA);

    a->apply( b );

    return 0;
}
