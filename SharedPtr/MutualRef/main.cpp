#include <stdio.h>
#include <map>
#include <tr1/memory>
#include <tr1/shared_ptr.h>
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

#ifdef USE_STD_SHARED_PTR
class A: public std::tr1::enable_shared_from_this<A> 
#else
class A
#endif
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

class B 
{
    public:
        B() {}

        void addA( A_ptr a )
        {
            A::Category c = a->getCategory();
            as[c] = a;
        }

    private:
        std::map< A::Category, A_ptr > as;
};

void A::apply( B_ptr &b )
{
#ifdef USE_STD_SHARED_PTR
    b->addA( shared_from_this() );
#else
    b->addA( A_ptr(this) );
#endif
}

class oneA : public A
{
    public:
        oneA(): A(one) {}
};

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

int main( int argc, char **argv )
{
    B_ptr b( new B );
    A_ptr a( new oneA);

    a->apply( b );

    return 0;
}
