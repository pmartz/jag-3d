#ifndef categorized_set_def
#define categorized_set_def

#include <vector>
#include <map>
#include <set>
#include <algorithm>

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

#endif

