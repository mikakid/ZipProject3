#pragma once


#include "zip.h"
#include <string>
#include <vector>
using namespace std;


#ifndef BLOCK
#define BLOCK



class block {
public:
	/*
	* @brief Default, copy and merge constructors for the block. 
	* @pre Requires instances of the block class to be copied or merged. 
	* @post Creates a new block.
	*/
	block();
	block(const block& old);
	block(block&, block&);

	/*
	* @brief adds a new record onto the existing block.
	* @pre Requires that the block not be over bytes. 
	* @post A record is added onto an existing block. Returns true if added or false if it failed. 
	*/
	bool addRecord(zip& newZip);

	/*
	* @brief Deletes a record from the block. 
	* @pre Requires a valid zip code to be removed from the block.  
	* @post Removes a record from the block and decreases the block size. Returns true if deleted or false if it failsed. 
	*/
	bool delRecord(int zip);

	/*
	* @brief block split function
	* @pre a calling block to be split and a new block to be split to
	* @post halves the block into two blocks
	*/
	void split(block& newBlock);

	/*
	* @brief Gives the next block in the list. 
	* @post Returns the next block index.
	*/
	int getNext() { return next; };

	/*
	* @brief Gives the previous block in the list. 
	* @post Returns the previous block index.
	*/
	int getPrev() { return prev; };

	/*
	* @brief Gives if the block is active
	* @post Returns true if the block is active
	*/
	bool getActive() { return active; };

	/*
	* @brief Gives the record count of the calling block 
	* @post Returns the record count as an int
	*/
	int getRecCount() { return recCount; };

	/*
	* @brief Gives the current size of the calling block 
	* @post Returns the current size as an int 
	*/
	int getCurrentSize() { return currentSize; };

	/*
	* @brief Gives the highest zip of the calling block  
	* @post Returns the highest zip as an int
	*/
	int getHighestZip() { return highestZip; };

	/*
	* @brief Gives the full list of records from the block 
	* @post Returns the vector of zips that holds the records
	*/
	void getRecords(vector<zip>& rtn);

	/*
	* @brief searches the block for a zip and returns the record
	* @pre A zip code to search for and a zip code object to be overwritten   
	* @post A boolean value that returns true if the target is found 
	*/
	bool getZip(zip& rtn, int target);

	/**
	* @brief
	* @pre
	* @post
	*/
	void setActive(bool state) { active = state; };

	/*
	* @brief Sets the next value of the block to a given int  
	* @pre An int to assign the next value to  
	* @post Returns void
	*/
	void setNext(int next) { this->next = next; };

	/*
	* @brief Sets the prev value of the block to a given int  
	* @pre An int to assign the prev value to 
	* @post Returns void
	*/
	void setPrev(int prev) { this->prev = prev; };

	/*
	* @brief Sets the record count of the block to a given int  
	* @pre An int to assign the record count to
	* @post Returns void
	*/
	void setRecCount(int recCount) { this->recCount = recCount; };

	/*
	* @brief Sets the current size of the block to a given int   
	* @pre An int to assign the current size to
	* @post Returns void
	*/
	void setCurrentSize(int currentSize) { this->currentSize = currentSize; };

	/*
	* @brief Sets the highest zip of the block to a given int 
	* @pre An int to assign the highest zip to
	* @post Returns void
	*/
	void setHighestZip(int highestZip) { this->highestZip = highestZip; };

	/*
* @brief Sets the highest zip for a block.
* @pre Requires that the block is not empty.
*/
	int findHighestZip();

private:


	/*
	* @brief Gives the size of a zip record.  
	* @pre Takes in a zip.
	* @post Returns the size of the zip object.
	*/
	int zipSize(zip& zipper);

	/**
	* @brief
	* @pre
	* @post
	*/
	int headerSize();

	bool active;
	int prev, next;
	int highestZip, recCount, currentSize;
	
	
	vector<zip> records;
};

#endif