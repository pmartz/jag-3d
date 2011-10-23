#ifndef COMPARITOR_DEF
#define COMPARITOR_DEF

template <class T>
class Comparitor
{
    public:
        virtual bool operator < (const T &rhs) const = 0;
        virtual bool operator > (const T &rhs) const = 0;
        virtual bool operator == (const T &rhs) const = 0;
};

#endif
