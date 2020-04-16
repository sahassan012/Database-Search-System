/*test.cpp*/

//
// Testing for util.cpp?
//

#include <iostream>
#include <vector>

#include "avl.h"
#include "util.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

//Not needed
TEST_CASE("(1) ???") 
{
//     vector<string>  values = GetRecord("students", 0, 5);
//     for (string s : values){
//         cout << s << " " ;
//     }
//     REQUIRE(values.size() == 5);
}

TEST_CASE("(2) ???") 
{
    vector<streamoff>  values = LinearSearch("students", 82, 5, "kim", 3);
    for (streamoff s : values){
        cout << s << " " ;
    }
    REQUIRE(values.size() == 2);
    REQUIRE(values.at(0) == 164);
}
