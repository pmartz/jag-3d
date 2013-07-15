/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License version 2.1 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*************** <auto-copyright.pl END do not edit this line> ***************/


#include <jagSG/Node.h>
#include <jagDraw/Node.h>
#include <jagBase/UserDataOwner.h>


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
