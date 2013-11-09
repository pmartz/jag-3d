/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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
