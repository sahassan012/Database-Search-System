# University Database Search System

## About
This search system is a database-like program that takes SQL SELECT queries as input and retrieves the data requested by user.

## How it works:
First, the program uses a metadata file that consists of data that indicates which columns are indexed. 
It then builds and uses trees to find the data that is index or uses linear search for data that isn't indexed. 
