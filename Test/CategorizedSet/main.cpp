#include "categorized_set.h"
#include <stdio.h>
#include <string>
#include <map>


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
