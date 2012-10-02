#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>


//////////////////////////////////////

// This is just another form of typedef (with templates)
template <class T>
struct priority_list: public std::vector<T> {};

// T = category (key to maps)
// U = values of maps
template <class T, class U>
class categorized_set: public std::map<T,U>
{
    public:
        bool contains( T t) const
        {
            typename categorized_set<T,U>::const_iterator p = this->find( t );
            return p == this->end() ? false: true;
        }
    
        bool contains( T t, U &u) const
        {
            typename categorized_set<T,U>::const_iterator p = find( t );
            if( p == this->end() )
                return false;
            u = p->second;
            return true;
        }

        /*
         * This operator does two things.  
         * 1) it applies the lhs to the rhs (like +)
         * 2) it returns only the deltas
         */
        categorized_set<T,U> operator >>( categorized_set<T,U> &rhs )
        {
            std::set<T> l;
            for( typename categorized_set<T,U>::const_iterator p = (*this).begin(); p != (*this).end(); p++ )
                l.insert( p->first );

            for( typename categorized_set<T,U>::const_iterator p = rhs.begin(); p != rhs.end(); p++ )
                l.insert( p->first );

            categorized_set<T,U> ret;
            for( typename std::set<T>::iterator p = l.begin(); p != l.end(); p++ )
            {
                unsigned int n = 0;
                U a, b;
                if( (*this).contains(*p, a) ) n |= 1<<0;
                if( rhs.contains(*p, b) ) n |= 1<<1;
                switch( n )
                {
                    case 0: break;
                    case 1: rhs[*p] = a; ret[*p] = a; break;
                    case 2: break;
                    case 3: 
                        {
                            U c = rhs[*p];
                            rhs[*p] = b + a; // Backwards, because a replaces b

                            // No change
                            if( rhs[*p] == c )
                                break;

                            ret[*p] = rhs[*p];
                        }
                        break;
                }
            }
            return ret;
        }

        categorized_set<T,U> operator +( const categorized_set &rhs )
        {
            std::set<T> l;
            for( typename categorized_set<T,U>::const_iterator p = (*this).begin(); p != (*this).end(); p++ )
                l.insert( p->first );
            for( typename categorized_set<T,U>::const_iterator p = rhs.begin(); p != rhs.end(); p++ )
                l.insert( p->first );

            categorized_set<T,U> ret;
            for( typename std::set<T>::iterator p = l.begin(); p != l.end(); p++ )
            {
                unsigned int n = 0;
                U a, b;
                if( (*this).contains(*p, a) ) n |= 1<<0;
                if( rhs.contains(*p, b) ) n |= 1<<1;
                switch( n )
                {
                    case 0: break;
                    case 1: ret[*p] = a; break;
                    case 2: ret[*p] = b; break;
                    case 3: ret[*p] = a + b; break;
                }
            }

            return ret;
        }
};

template <class T,class U>
class categorized_set_sorter
{
    public:

        categorized_set_sorter() {}

        categorized_set_sorter( priority_list<T> &plist ):
            m_priority_list( plist)
        {}

        categorized_set_sorter( const categorized_set_sorter &c ):
            m_priority_list( c.m_priority_list )
        {}

        bool operator() ( const categorized_set<T,U> &lhs, const categorized_set<T,U> &rhs )
        {
            for( typename priority_list<T>::const_iterator p = m_priority_list.begin() ; p != m_priority_list.end(); p++ )
            {
                unsigned int n = 0;
                U a, b;
                if( lhs.contains(*p, a) ) n |= 1<<0;
                if( rhs.contains(*p, b) ) n |= 1<<1;
                switch( n )
                {
                    case 0: continue;
                    case 1: return false;
                    case 2: return true;
                }
                // both categories exist in the list.  Test for < or > 
                if( a < b ) { return true; }
                if( a > b ) { return false; }

                // attributes in this category are equivalent.  Continue to next category.
            }

            // return an aribitrary true.  
            return true;
        }

    protected:
        priority_list<T> m_priority_list;

};


//////////// test implementation ////////////////

// Debug convenience

enum Category{ 
    ID,
    S,
    T,
    U,
    V,
};

Category c[] = { ID, S, T, U, V };

std::map<Category,std::string> catname;

class Start
{
    public:
        Start()
        {
            catname[ID] = "ID";
            catname[S] = "S";
            catname[T] = "T";
            catname[U] = "U";
            catname[V] = "V";
        }
}start;


class Attribute
{
    public:
        Attribute(): m_val(0) {}

        Attribute( int n ):
            m_val(n)
        {}

        Attribute( const Attribute &a ):
            m_val(a.m_val)
        {}

        virtual bool operator < (const Attribute &rhs) const
        {
            return m_val < rhs.m_val;
        }

        virtual bool operator > (const Attribute &rhs) const
        {
            return m_val > rhs.m_val;
        }

        virtual bool operator == ( const Attribute &rhs ) const
        {
            return m_val == rhs.m_val;
        }

        Attribute operator + (const Attribute &a )
        {
            return a;
        }

        int getVal() const { return m_val; }

    private:
        int m_val;
};


typedef categorized_set<Category,Attribute> StateSet;

void printHead()
{
    printf("%10s - %4s %4s %4s %4s %4s\n", "Name", "ID", "S", "T", "U", "V" );
    printf("%10s - %4s %4s %4s %4s %4s\n", "----", "--", "-", "-", "-", "-" );
}

void printStateSet( const std::string &name, const StateSet &s )
{
    printf("%10s   ", name.c_str() );
    for( unsigned int i = 0; i < sizeof(c)/sizeof(Category); i++ ) 
    { 
        StateSet::const_iterator q = s.find(c[i]); 
        if( q == s.end() ) 
            printf("%4s ", " " ); 
        else 
            printf("%4u ", q->second.getVal() ); 
    } 
    printf("\n" );
}

void printList( std::vector<StateSet> &l )
{
    printf("%2s - %4s %4s %4s %4s %4s\n", " ", "ID", "S", "T", "U", "V" );
    printf("%2s - %4s %4s %4s %4s %4s\n", " ", "--", "-", "-", "-", "-" );
    int n = 0;
    for( std::vector<StateSet>::iterator p = l.begin(); p != l.end(); p++ )
    {
        printf("%2d", n );
        n++;
        for( unsigned int i = 0; i < sizeof(c)/sizeof(Category); i++ )
        {
            StateSet::const_iterator q = p->find(c[i]);
            if( q == p->end() )
                printf("%4s ", " " );
            else
                printf("%4u ", q->second.getVal() );
        }
        printf("\n" );
    }
}

int main( int argc, char **argv )
{
    /*
    //std::vector<Category> priorityList;
    priority_list<Category>priorityList;
    //priorityList.push_back( ID );
    priorityList.push_back( S );
    priorityList.push_back( T );
    priorityList.push_back( U );
    priorityList.push_back( V );
    */

    printHead();

    StateSet def;

    def[ID] = 1;
    def[S] = Attribute(1);

    printStateSet( "Default", def );

    StateSet first;
    first[ID] = 2;
    first[S] = Attribute(2);
    first[T] = Attribute(4);

    printStateSet( "first", first );

    StateSet cursor = def + first;
    printStateSet( "cursor", cursor );

    StateSet current; 
    StateSet changes;
    changes = cursor >> current;

    printStateSet("current", current );
    printStateSet("changes", changes );


    StateSet second;
    second[ID] = 3;
    second[S] = Attribute(2);
    second[T] = Attribute(4);
    second[V] = Attribute(2);

    printf("\n");

    cursor = def + second;
    printStateSet("second", second );
    printStateSet("cursor", cursor );

    changes = cursor >> current;
    printStateSet("current", current );
    printStateSet("changes", changes );


    StateSet ss1;
    ss1[ID] = 1;
    ss1[S] = Attribute(1);
    ss1[T] = Attribute(2);
    ss1[U] = Attribute(3);
    ss1[V] = Attribute(1);

    StateSet ss2;
    ss2[ID] = 2;
    ss2[S] = Attribute(1);
    ss2[T] = Attribute(1);


    StateSet ss3;
    ss3[ID] = 3;
    ss3[S] = Attribute(1);
    ss3[T] = Attribute(1);
    ss3[U] = Attribute(1);
    ss3[V] = Attribute(1);


    std::vector<StateSet> l;
    l.push_back( ss3 );
    l.push_back( ss1);
    l.push_back( ss2);
    printf("B4 sort: (%lu)\n", l.size()  );
    printList( l );

    //std::vector<Category> priorityList;
    priority_list<Category>priorityList;
    //priorityList.push_back( ID );
    priorityList.push_back( S );
    priorityList.push_back( T );
    priorityList.push_back( U );
    priorityList.push_back( V );

//    puts("--BEGIN Sort---" );

    categorized_set_sorter<Category,Attribute> css( priorityList );

    std::sort( l.begin(), l.begin() + l.size(), css );

//    puts("--END Sort---" );

    printf("After sort: (%lu)\n", l.size() );

    printList( l );


    return 0;
}
