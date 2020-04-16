/*util.h*/

//
// Utility functions for myDB project
//

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void EchoData(string tablename, int recordSize, int numColumns);

vector<string> GetRecord(string tablename, streamoff pos, int numColumns);

vector<streamoff> LinearSearch(string tablename, int recordSize, int numColumns, string matchValue, int matchColumn);
