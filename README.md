# University Database Search System

## About
This search system is a database-like program that takes SQL SELECT queries as input and retrieves the data requested by user.

## How it works:
First, the program uses a metadata file that consists of data that indicates which columns are indexed. 
It then builds and uses trees to find the data that is indexed or uses linear search for data that isn't indexed. 

A table is represented using 2 files:
* meta-data file(.meta)
  - Line 1 : record size(offset)
  - Line 2 : # of columnss
  - ..rest of the lines: column name + integer (1 if indexed, 0 if not) 
  - Example:
    ```console
      82
      5
      uin 1
      firstname 0
      lastname 0
      netid 1
      email 0
    ```
* data file(.data)
  - Each line consists: uin, first name, last name, netid, and email address
  - Example:
    ```console
        123456 pooja sarkar psarka12 pooja@piazza.com ..................................
        456789 frank zizza fzizza2 zizza@ucla.edu ......................................
        789123 mee kim mkim16 mkim16@uic.edu ...........................................
        238117 lucy johnson ljohns21 lucy.johnson@gmail.com ............................
        556178 drago kolar dkolar8 drago1998@yahoo.com .................................
        732290 soo kim skim21 skim21@uic.edu ...........................................
    ```
## How to run(from command line)
Run program:
```console
./program.exe
```

You will then be prompted to enter a table name:
```console
Welcome to myDB, please enter tablename> students
```

This will read from the metadata file and build tree data structures for the indexed columns.
```console
Reading meta-data...
Building index tree(s)...
Index column: uin
  Tree size: 6
  Tree height: 2
Index column: netid
  Tree size: 6
  Tree height: 2
```
