#pragma once

#ifndef BLOCKFILE
#define BLOCKFILE

#include "blockBuf.h"
#include "recBuf.h"
#include "zip.h"
#include "block.h"
#include "blockIndex.h"
#include "LIBuffer.h"
#include "primaryindex.h"

const int FILESIZE = 512;

class blockFile {
public:

	/*
	* @brief default constructor
	*
	*
	*/
	blockFile();

	/*
	* @brief specified constructor
	* @pre takes a string as parameter
	* @post opens file specified by parameter
	*/
	blockFile(string bFile) { open(bFile); };

	/*
	* @brief length indicated to block function
	* @pre takes a string index and LI  
	* @post
	*/
	void liToBlock(string index, string lidata);


	/**
	* @brief opens the file indicated by filename
	* @pre
	* @post
	*/
	void open(string filename) { iData.open(filename); oData.open(filename);};

	/**
	* @brief
	* @pre
	* @post
	*/
	void close() { iData.close(); oData.close(); };

	/*
	* @brief read header function
	* @pre
	* @post
	*/
	void readHeader();

	/*
	* @brief write header function
	* @pre 
	* @post
	*/
	string writeHeader();

	/**
	* @brief physical dump function
	* @pre
	* @post
	*/
	string pdump();

	/**
	* @brief logical dump function
	* @pre
	* @post
	*/
	string ldump();

	/**
	* @brief split function
	* @pre
	* @post
	*/
	bool split(block& b);

	/**
	* @brief add record function
	* @pre
	* @post
	*/
	bool addRecord(zip& z);

	/**
	* @brief delete record function
	* @pre
	* @post
	*/
	bool delRecord(string zip);

	/**
	* @brief get first function 
	* @post returns the first RBN as a zip 
	*/
	int getFirst() { return first; };

	/**
	* @brief get avail function 
	* @post returns 
	*/
	int getAvail() { return avail; };

private:


	int first, avail, numBlocks, recCount;

	ofstream oData;
	ifstream iData;
	blockBuf buf;
	blockIndex index;
};

#endif BLOCKFILE