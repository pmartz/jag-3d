#ifndef categorized_set_def
#define categorized_set_def

#include <map>
#include <vector>

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
};

template <class T>
class priority_list: public std::vector<T>
{
};


template <class T, class U>
class sortable_categorized_set: public categorized_set<T,U>
{
    public:
        sortable_categorized_set( ) {}

        sortable_categorized_set( priority_list<T> priorityList ):
            m_plist(priorityList)
        {
        }

        sortable_categorized_set( const sortable_categorized_set &s ):
            categorized_set<T,U>( s ),
            m_plist(s.m_plist)
        {
        }

        sortable_categorized_set& operator=(sortable_categorized_set &s) 
        {
            m_plist = s.m_plist;
            ::categorized_set<T,U>::operator =( s );
            return *this;
        }

        /*
        bool operator < ( const sortable_categorized_set &rhs ) const
        {
            for( typename priority_list<T>::const_iterator p = m_plist.begin() ; p != m_plist.end(); p++ )
            {
                unsigned int n = 0;
                U a, b;
                if( (*this).contains(*p, a) ) n |= 1<<0;
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

                // attributes in this category are equivalent.  Check continue to next category.
            }
            // return an aribitrary true.  Reasons could be many
            return true;
        }
        */

    protected:
        priority_list<T> m_plist;
};

#endif
