#ifndef II_DRAW_DO_DEF
#define II_DRAW_DO_DEF

#include <vector>
#include <map>

enum Category{ 
    S,
    T,
    U,
    V,
};

class Attribute 
{
    public:
        Attribute( unsigned int n=0 ): m_value(n) {}

        Attribute( const Attribute &a ):
            m_value(a.m_value)
        {}


        void setValue( unsigned int n ) { m_value = n; }
        unsigned int getValue() const { return m_value; }

        bool operator < ( const Attribute &a ) const
        {
            return m_value < a.getValue();
        }

        bool operator <= ( const Attribute &a ) const
        {
            return m_value <= a.getValue();
        }

        bool operator == ( const Attribute &a ) const
        {
            return m_value == a.getValue();
        }

        bool operator != ( const Attribute &a ) const
        {
            return m_value != a.getValue();
        }

        bool operator > ( const Attribute &a ) const
        {
            return m_value > a.getValue();
        }

    private:
        unsigned int m_value;

};

typedef std::vector<Category> CategoryList;
typedef std::map< Category, Attribute > AttributeByCategory;

class DO 
{
    public:
        DO() {}

        DO(const DO &d):
            m_attributes(d.m_attributes) {}


        size_t getNumAttributes() { return m_attributes.size();}

        void addAttribute( Category c, const Attribute &a )
        {
            m_attributes[c] = a;
        }

        Attribute getAttribute( const Category &c ) const
        {
            AttributeByCategory::const_iterator p;
            if( (p = m_attributes.find(c)) == m_attributes.end() )
                return Attribute();

            return p->second;
        }

        const AttributeByCategory &getAttributes() { return m_attributes; }

        /*
        void operator + ( const DO &rhs )
        {
            const AttributeByCategory &rhsAttributes = rhs.getAttributes();
            for( AttributeByCategory::const_iterator p = rhsAttributes.begin(); p != rhsAttributes.end(); p++ )
            {

            }
        }
        */

    private:
        AttributeByCategory m_attributes;
};

class SortedDO
{
    public:
        SortedDO( const CategoryList &c, DO *d ):
            m_categoryList(c),
            m_do(d)
        {}

        SortedDO( const SortedDO &s ):
            m_categoryList(s.m_categoryList),
            m_do(s.m_do)
        {}

        const DO *getDO() const { return m_do; }

        bool operator < ( const SortedDO &d ) const
        {
            for( CategoryList::const_iterator p = m_categoryList.begin(); p != m_categoryList.end(); p++ )
            {
                if( m_do->getAttribute( *p ) >  d.getDO()->getAttribute( *p )  )
                    return false;

                if( m_do->getAttribute( *p ) != d.getDO()->getAttribute( *p )  )
                    break;
            }
            return true;

            /*
            if( m_do->getAttribute( zero ) > d.getDO()->getAttribute(zero) )
                return false;

            if( m_do->getAttribute( zero ) == d.getDO()->getAttribute(zero) )
            {
                if( m_do->getAttribute( one ) > d.getDO()->getAttribute(one) )
                    return false;

                if( m_do->getAttribute( one ) == d.getDO()->getAttribute(one) )
                {
                    if( m_do->getAttribute( two ) > d.getDO()->getAttribute(two) )
                        return false;

                    if( m_do->getAttribute( two ) == d.getDO()->getAttribute(two) )
                    {
                        if( m_do->getAttribute(three) > d.getDO()->getAttribute(three))
                            return false;
                    }
                }
            }
            return true;
            */
        } 

    private:
        CategoryList m_categoryList;
        DO *m_do;

};



#endif
