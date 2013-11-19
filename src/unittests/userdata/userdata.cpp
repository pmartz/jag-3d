/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/


#include <jagSG/Node.h>
#include <jagDraw/Node.h>
#include <jagBase/UserDataOwner.h>

#include <Poco/Foundation.h>
#include <boost/foreach.hpp>
#include <gmtl/gmtl.h>

#include <iostream>







bool test()
{
    {
        std::cout << "Testing: User Data setting and retrieval using helper methods. . ." << std::endl;
        {
            jagBase::UserDataOwner udo;
            udo.setUserDataPair("prop1", 1);
            udo.setUserDataPair("prop2", 2);
            if(udo.getUserDataValue("prop2") != 2) {
                std::cerr << "could not retrieve previously set value from string key" << std::endl;
                return false;
            }
        }

        std::cout << "Testing: User Data setting and retrieval using pass by reference get method. . ." << std::endl;
        {
            jagBase::UserDataOwner udo;
            udo.getUserData()["prop1"] = "string test";
            udo.getUserData()["prop2"] = 42;

            if(udo.getUserData()["prop2"] !=  42) {
                std::cerr << "could not retrieve value set from string key using return by reference get method" << std::endl;
                return false;
            }

            if(udo.getUserData()["prop1"] != "string test") {
                std::cerr << "could not retrieve and compare string value" << std::endl;
                return false;
            }

        }

        std::cout << "Testing: User Data setting and retrieval using helper methods with jagDraw::Node. . ." << std::endl;
        {
            jagDraw::Node udo;
            udo.setUserDataPair("prop1", 1);
            udo.setUserDataPair("prop2", 2);
            if(udo.getUserDataValue("prop2") != 2) {
                std::cerr << "could not retrieve previously set value from string key" << std::endl;
                return false;
            }
        }

        std::cout << "Testing: User Data setting and retrieval using pass by reference get method with jagDraw::Node. . ." << std::endl;
        {
            jagDraw::Node udo;
            udo.getUserData()["prop1"] = "string test";
            udo.getUserData()["prop2"] = 42;

            if(udo.getUserData()["prop2"] !=  42) {
                std::cerr << "could not retrieve value set from string key using return by reference get method" << std::endl;
                return false;
            }

            if(udo.getUserData()["prop1"] != "string test") {
                std::cerr << "could not retrieve and compare string value" << std::endl;
                return false;
            }

        }

        std::cout << "Testing: User Data setting and retrieval using helper methods with jagSG::Node. . ." << std::endl;
        {
            jagSG::Node udo;
            udo.setUserDataPair("prop1", 1);
            udo.setUserDataPair("prop2", 2);
            if(udo.getUserDataValue("prop2") != 2) {
                std::cerr << "could not retrieve previously set value from string key" << std::endl;
                return false;
            }
        }

        std::cout << "Testing: User Data setting and retrieval using pass by reference get method with jagSG::Node. . ." << std::endl;
        {
            jagSG::Node udo;
            udo.getUserData()["prop1"] = "string test";
            udo.getUserData()["prop2"] = 42;

            if(udo.getUserData()["prop2"] !=  42) {
                std::cerr << "could not retrieve value set from string key using return by reference get method" << std::endl;
                return false;
            }

            if(udo.getUserData()["prop1"] != "string test") {
                std::cerr << "could not retrieve and compare string value" << std::endl;
                return false;
            }

        }


#if( POCO_OS != POCO_OS_MAC_OS_X )
        std::cout << "Testing: User Data setting and retrieval using pass by reference get method with jagSG::Node and a custom class for data storage. . ." << std::endl;
        {
            class TestClass {
            public:
                const int getA() {return _a;}
                const std::string getB() {return _b;}
                void setA(int a) {_a=a;}
                void setB(std::string b) {_b=b;}
            protected:
                int _a;
                std::string _b;
            };

            jagSG::Node udo;
            TestClass testVar;
            testVar.setA(4242);
            testVar.setB("string test123");
            udo.getUserData()["prop1"] = "string test";
            udo.getUserData()["prop2"] = 42;
            udo.getUserData()["prop3"] = testVar;


            if(udo.getUserData()["prop2"] !=  42) {
                std::cerr << "could not retrieve value set from string key using return by reference get method" << std::endl;
                return false;
            }

            if(udo.getUserData()["prop1"] != "string test") {
                std::cerr << "could not retrieve and compare string value" << std::endl;
                return false;
            }

             TestClass extractedVar = udo.getUserData()["prop3"].extract<TestClass>();

            if(extractedVar.getB() != "string test123") {
                std::cerr << extractedVar.getB() << std::endl;
                std::cerr << "could not use a custom class" << std::endl;    
                return false;
            }
        }

        
        std::cout << "Testing: User Data setting and retrieval using pass by reference get method with jagSG::Node and a large custom class for data storage. . ." << std::endl;
        {
            class TestClass {
            public:
                const int getA() {return _a;}
                const std::string getB() {return _b;}
                void setA(int a) {_a=a;}
                void setB(std::string b) {_b=b;}
            public:
                int _a;
                std::string _b;
                int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,x,y,z,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak,al,am,an,ao,ap,aq,ar,as,at,au,av,ax,ay,
                    az,ba,bb,bc,bd,be,bf,bg,bh,bi,bj,bk,bl,bm,bn,bo,bp,bq,br,bs,bt,bu,bv,bx,by,bz;
            };

            jagSG::Node udo;
            TestClass testVar;
            testVar.setA(4242);
            testVar.setB("string test123");
            testVar.v = 567;
            testVar.bx = 568;
            testVar.av = 569;
            udo.getUserData()["prop1"] = "string test";
            udo.getUserData()["prop2"] = 42;
            udo.getUserData()["prop3"] = testVar;
            

            if(udo.getUserData()["prop2"] !=  42) {
                std::cerr << "could not retrieve value set from string key using return by reference get method" << std::endl;
                return false;
            }

            if(udo.getUserData()["prop1"] != "string test") {
                std::cerr << "could not retrieve and compare string value" << std::endl;
                return false;
            }

             TestClass extractedVar = udo.getUserData()["prop3"].extract<TestClass>();

            if(extractedVar.getB() != "string test123") {
                std::cerr << extractedVar.getB() << std::endl;
                std::cerr << "could not use a custom class" << std::endl;    
                return false;
            }

            if(extractedVar.v != 567) {
                std::cerr << extractedVar.v << std::endl;
                std::cerr << "could not use a large custom class" << std::endl;    
                return false;
            }

            if(extractedVar.bx != 568) {
                std::cerr << extractedVar.bx << std::endl;
                std::cerr << "could not use a large custom class" << std::endl;    
                return false;
            }

            if(extractedVar.av != 569) {
                std::cerr << extractedVar.av << std::endl;
                std::cerr << "could not use a large custom class" << std::endl;    
                return false;
            }
        }
#endif

    }
    return( true );

}

int main( int argc, char** argv )
{
    std::cout << "This is a CTest regression test. To launch under Visual Studio, build the" << std::endl;
    std::cout << "RUN_TESTS target. Under Linux, enter 'make test' at a shell prompt." << std::endl;


    std::cout << std::endl;
    if( test() )
    {
        std::cout << "Test passed." << std::endl;
        return( 0 );
    }
    else
    {
        std::cout << "Test failed." << std::endl;
        return( 1 );
    }
}
