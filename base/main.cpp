/*main.cpp*/

//
// myDB project using AVL trees
//
// Syed Hassan
//
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

#include "avl.h"
#include "util.h"

using namespace std;


//
// tokenize:
//
// Given a string, breaks the string into individual tokens (words) based
// on spaces between the tokens.  Returns the tokens back in a vector.
//
//
vector<string> tokenize(string line)
{
  vector<string> tokens;
  stringstream  stream(line);
  string token;

  while (getline(stream, token, ' '))
  {
    tokens.push_back(token);
  }

  return tokens;
}

//
//
// findTree:
//   Given the metadata, and name of column, 
//   returns the position of the avl tree that matches user's input of column,
//   returns -1 if a column exists but it is not indexed 
//   or returns -2 if the column does not exist in table
//
//
int findTree(vector< pair<string, string> > &metadata, const string& s){   
    
    //Initialize int variable that depicts the positioning of the matching tree
    // in the vector of avl trees
    int treePos = 0;
    
    //Go through the metadata vector
    for (unsigned int i = 0; i < metadata.size(); i++){
        
        //If the first of the pair matches string AND it is indexed
        //this means a tree exists, return the position of the tree
        if (metadata[i].second == s && metadata[i].first == "1"){
            return treePos;
        }
        
        //If the first of the pair matches string AND 
        //it is not indexed, this means a linear search is needed, return -1
        if (metadata[i].second == s && metadata[i].first != "1"){
            return -1;
        }
        
        //If the name of the column does not match, but the current column is indexed
        //this means there is an AVL that exists, so increment treePos
        if (metadata[i].second != s && metadata[i].first == "1"){
           treePos++; 
        }
    }
    
    //If we get here, the column does not exist
    return -2;
}

//
//
//  findColumnNum:
//   Given the metadata, and name of column 
//   returns the position of the matching column 
//   or if column is not found, returns -1
//     
//     
int findColumnNum(vector<pair<string, string>> &metadata, const string& s){
    
    //Go through the metadata vector
    for (unsigned int i = 0; i < metadata.size(); i++){
        
        //If the first of the pair matches string AND it is indexed
        //this means a tree exists, return the position of the tree
        if (metadata[i].second == s){
                return i;
        }
    }
    return -1;
}

//
//
//  runQuery:
//   Given the vector of AVL trees, the datafile, and the metadata information
//   runs a search and displays the information based on the query input
//
//
void runQuery(vector<pair<string, string>> &metadata, vector <avltree<string, streamoff>> &vectorOfTrees, ifstream &datafile, string tablename, string offset){
  
  //Variable that will temporarily store data
  string value;
  
  //Get the number of columns
  int col = metadata.size();  
    
  //Loop to input and execute queries from the user
  string query;
  
  cout << endl;
  cout << "Enter query> ";
  getline(cin, query);

  while (query != "exit")
  {
    vector<string> tokens = tokenize(query);
    
    //Error-handling, checks and makes sure that the query consists of correct inputs
    if (tokens.at(0) != "select" && tokens.at(0) != "exit"){
        cout << "Unknown query, ignored..." << endl;
    }
    else if (tokens.size() != 8)
    {
        cout << "Invalid select query, ignored..." << endl;
    }
    else if (tokens.at(0) == "select" && tokens.at(1) != "*" && findColumnNum(metadata, tokens.at(1)) == -1){
        cout << "Invalid select column, ignored..." << endl;
    }
    else if (tokens.at(2) != "from"){
        cout << "Invalid\n";
    }
    else if (tokens.at(3) != tablename){
        cout << "Invalid table name, ignored..." << endl;
    }
    else if(tokens.at(4) != "where"){
        cout << "where' clause not found, ignored..." << endl;
    }
    else if (findColumnNum(metadata, tokens.at(5)) == -1){
        cout << "Invalid where column, ignored..." << endl;
    }
    else if (tokens.at(6) != "="){
        cout << "Invalid\n";
    }
    else{  //if no errors were triggered, proceed 

        //Check if tree exists for the column specified in query
        int treePos = findTree(metadata, tokens.at(5));   
        
        //If a tree does not exist, but the column does, use linear search
        if (treePos == -1){
            //Acquire the column number based off of the where clause in the query
            int columnNum = findColumnNum(metadata, tokens.at(5));
            
            //Do a linear search by calling LinearSearch witj tje 
            vector<streamoff> vec = LinearSearch(tablename, stoi(offset), col, tokens.at(7), columnNum+1);
            if (vec.size() != 0){
                //If query uses *, get all columns 
                if (tokens.at(1) == "*"){

                   //Go through vector of streamoff data type
                   for (unsigned int i = 0; i < vec.size(); i++){

                       //Jump to the positioning specified by streamoff variable
                       datafile.seekg(vec[i], datafile.beg);

                       //Print out each column information
                       for (int z = 0; z < col; z++ ){
                           datafile >> value;
                           cout << metadata[z].second << ": " << value << endl;
                       }
                   }
                }
                else{  //Query specifies a column name
                    
                     //Acquire the column number based off of the where clause in the query
                    int columnNum = findColumnNum(metadata, tokens.at(1));
                    
                    //Go through vector of streamoff data type
                    for (unsigned int i = 0; i < vec.size(); i++)
                    {
                        //Jump to the positioning specified by streamoff variable
                        datafile.seekg(vec[i], datafile.beg);
                         
                         //Go through columns before the one specified
                         for (int j = 0; j < columnNum ; j++)
                         {
                            datafile >> value;
                         }
                         
                         //Here is where the column specified is, so get the data
                         datafile >> value;
                        
                         //Print out the data
                         cout << metadata[columnNum].second << ": " << value << endl;
                    }

                }
            }
            else{  //Otherwise, show not found
                cout << "Not found..." << endl;
            }    
        }
        else if (treePos == -2){ //Column does not exist
            cout << "Not found..." << endl;
        }
        else{ //Tree exists, perform search using AVL tree at the index 
           
            //Store the tree that exists in the vector of AVL trees into local AVL tree object
            avltree <string,streamoff> avl = vectorOfTrees.at(treePos);
            
            //Search the avl using the key that was entered
            streamoff *pos = avl.search(tokens.at(7));
            
            //If the returned streamoff pointer is not null
            if (pos != nullptr){
                
                //Use the streamoff value to get the record
                vector<string> vec = GetRecord(tablename, *pos, col);
                
                //If user chooses all columns to be displayed 
                if (tokens.at(1) == "*"){
                    for (int z = 0; z < col; z++ ){
                        cout << metadata[z].second << ": " << vec[z] << endl;
                    }
                }
                else{  //Query specifies a column for where condition
                    
                    //Find the column number in the metadata vector
                    int columnNum = findColumnNum(metadata, tokens.at(1));
                    
                    //Print out information at the column number
                    cout << metadata[columnNum].second << ": " << vec[columnNum] << endl;
                }
            }
            else{ //Otherwise, show that a match for the specified column was not found
                cout << "Not found...\n";
            }
        }
    }
    
    cout << endl;
    
    //Prompt user for query
    cout << "Enter query> ";
    getline(cin, query);
  }
  //End of while loop
}

//
//
//  readDataFile:
//   Given the metadata and name of table, reads the datafile, 
//   builds AVL trees for columns that need indexing, and stores the AVL trees in a vector  
//   
//
void readDataFile(string offset, vector<pair<string, string>> &metadata, string tablename){
   
  //Value where each data will be stored temporarily
  string value;
    
  //Declare vector where avl trees will be stored for indexed columns
  vector < avltree<string, streamoff> > vectorOfTrees;  

  cout << "Building index tree(s)..." << endl;
  
  //Opening the metadata file for the table that was entered
  string filename = tablename + ".data";
  ifstream datafile(filename, ios::in | ios::binary);

  //Return if the file doesn't exit
  if (!datafile.good())
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return;
  }
  
  datafile.seekg(0, datafile.end);  // move to the end of file and store length:
  streamoff length = datafile.tellg();
    
  //For every column in the metadata file, check if the column is indexed 
  for(unsigned int i = 0; i < metadata.size(); i++){
      
      streamoff pos2 = 0;
      
      //If a column is indexed, create an avl and store all values of that column 
      //from the data file and their positions into an avl tree 
      if (metadata[i].first == "1"){
          
          //Declare AVL tree to store into vector of trees
          avltree<string, streamoff> avltree; 
          
          cout << "Index column: " << metadata[i].second << endl; 
          //Extract the data from the column that needs indexing and store it in avl
          while (pos2 < length)
          {
            datafile.seekg(pos2, datafile.beg);  // move to start of record:
            
            for (unsigned int x = 0; x < i; ++x)  // Go through columns that exist before matching column
            {
              datafile >> value;    
            }
           
            datafile >> value; //Get value from column and store into value
            
            avltree.insert(value, pos2); //Store value into avl tree along with its position
           
            pos2 += stoi(offset);  // move offset to start of next record:
          }
          
          vectorOfTrees.push_back(avltree); //Store the AVL tree into vector of AVL trees
          
          //Display tree size and height
          cout << "  Tree size: " << avltree.size() << endl;
          cout << "  Tree height: " << avltree.height() << endl;
      }
  }
  
  runQuery(metadata, vectorOfTrees, datafile, tablename, offset);
    
}

//
//
//  readMetadataFile:
//   Given the metadata file and tablename, reads the metadata file and stores information into vector
//
//
void readMetadataFile(ifstream &data, string tablename){
  
  string value, columnName;
  data.seekg(0, data.beg);  // move to the begginning of file:
  data >> value;

  //Store the offset value 
  string offset = value;

  //Store the number of columns and convert into an int
  data >> value;
  int col = stoi(value);
  
  //Declare position and a vector of pairs
  int pos = 0;
  vector< pair<string, string> > metadata;
  
  //While the position is less than # of columns
  while (pos < col)
  {
      data >> columnName;  //Get name of column
      data >> value;   // Get index value
      metadata.push_back(make_pair(value, columnName));  //Store the column name and index value into vector of pairs
      pos++;  // Move to next column
  }
       
  //Reading the data file using the metadata
  readDataFile(offset, metadata, tablename); 
}

//
//
//  fetchMetadata:
//   Given the name of table specified, open the metadata file if it exists in directory,
//   or return with error message if it doesn't
//   
//
void fetchMetadata(string tablename){
  
  //Opening the metadata file for the table that was entered
  string   filename = tablename + ".meta";
  ifstream data(filename, ios::in | ios::binary);

  //Return if the file doesn't exit
  if (!data.good())
  {
    cout << "**Error: couldn't open metadata file '" << filename << "'." << endl;
    return;
  }
  
  //Function call to read in metadata from file
  readMetadataFile(data, tablename);    
    
}

//
//
//  main:
//
//
int main()
{
  string tablename; // = "students";

  cout << "Welcome to myDB, please enter tablename> ";
  getline(cin, tablename);

  cout << "Reading meta-data..." << endl;
  
  //Call function to open metadata file using the tablename specified
  fetchMetadata(tablename);                             
    
  return 0;
}
