/**
* blockIndex.h 
* Class containing the block index for the highest zip in each block
* 
*/
#pragma once

#ifndef BLOCKINDEX
#define BLOCKINDEX

#include <fstream>
#include <vector>
#include "block.h"

using namespace std;

struct bIndexElement {
	int zip;
	int RBN;
	bool active;
};

class blockIndex {
public:
	/*
	* @brief default constructor
	* @pre
	* @post
	*/
	blockIndex() { index.clear(); }


	/*
	* @brief search function
	* @pre Takes an integer ZIP to search through the index 
	* @post Returns the block number that would contain this zip as an int
	*/
	int search(int zip); 

	/*
	*
	*
	*
	*/
	int findHighest();

	/*
	* @brief add function
	* @pre adds the first parameter as a zip and second as a RBN onto the back of the index
	* @post returns void
	*/
	void add(block& b, int r); 
	
	/*
	* @brief delete function
	* @pre takes a block number to delete from the index  
	* @post returns void 
	*/
	void del(int r); 

	/*
	* @brief Print to file Function
	* @post Outputs the content of the index to the file  
	*/
	void printToFile(string);

	/**
	* @brief
	* @post
	*/
	void readFromFile(string);

	/*
	* @brief get number blocks function
	* @post Returns the number of blocks as an int 
	*/
	int getNumBlocks() { return numBlocks; };

	/*
	* @brief get number available function
	* @post Returns the number of blocks available as an int 
	*/
	int getNumAvail() { return numAvail;  }

private:

	int numBlocks, numAvail;
	vector<bIndexElement> index;
};
#endif
