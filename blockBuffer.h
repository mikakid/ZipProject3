//BlockBuf.h
#pragma once

#ifndef BLOCKBUF
#define BLOCKBUF

#include <vector>
#include <iostream>
#include <string>
#include "zip.h"
#include "recBuf.h"
#include "block.h"

using namespace std;

const int BUFSIZE = 512;


class blockBuf {
public:
	/*
	* @brief Default Constructor.
	*/
	blockBuf() { blockText = ""; index = 0; };


	/*
	* @brief Reads from the file a specific block. 
	* @pre Takes the file to be read from and the relative block number for a specific block.   
	* @post BlockText is built with a block.
	*/
	void read(ifstream& infile, int RBN);

	/*
	* @brief Translates block object into text.
	* @pre Receives a block object.
	* @post BlockText is built with a block.
	*/
	void pack(block& b);

	/*
	* @brief Writes blocktext to the file at the indicated RBN.
	* @pre Receves a file to be written to and relative block number for position.  
	*/
	void write(ofstream& outfile, int RBN);

	/*
	* @brief Translates text into a block object.
	* @pre Receive an empty block object.
	* @post The blockText string is put into a block object. 
	*/
	void unpack(block& b);

	/**
	* @brief Returns the blockText. 
	*/
	string getText() { return blockText; };

	/**
	* @brief
	* @pre
	* @post
	*/

	void clear() { blockText.clear(); index = 0; };



private:

	/*
	* @brief Parses the header data of a block object. 
	* @pre Recieves an empty block object.
	* @post The header data is stored into the block object.  
	*/
	void readHeader(block& b);

	/**
	* @brief
	* @pre
	* @post
	*/
	string writeHeader(block& b);

	string blockText;
	block obj;
	int index;

};
#endif
