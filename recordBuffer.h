//recBuf.h
#pragma once
#ifndef RECBUF
#define RECBUF

#include <iostream>
#include <string>
#include <fstream>
#include "zip.h" 


class recBuf {
public:
	/*
	* @brief Default Constructor
	*/
	recBuf() { buf = ""; size = buf.size(); };

	/*
	* @brief Sets the buf to a given string.
	* @pre Takes a string given by the blockBuf class.  
	* @post Adds the string to the buffer.  
	*/
	void read(const string& recText);

	/*
	* @brief Writes the buffer to the blockText string.   
	* @pre Takes a string given by the blockBuf class to be written to.  
	* @post Clears the buffer. 
	*/
	void write(string& blockText);

	/*
	* @brief Parses a zip object onto the buffer.  
	* @pre Requires a zip object.
	* @post The buffer is filled with a record. 
	*/
	void pack(zip&);

	/*
	* @brief Parses the buffer and turns it into a zip object. 
	* @pre Receives an empty zip object.
	* @post Returns true if the buffer was made into a zip object or false if it wasn't.   
	*/
	bool unpack(zip&);

	/**
	* @brief clears the buffer
	* @pre none
	* @post buf is empty
	*/
	void clear() { buf = ""; size = 0; index = 0; };


private:
	string buf = "";
	const char delim = ',';
	int size = 0;
	int index = 0;
};
#endif