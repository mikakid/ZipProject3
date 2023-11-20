/**
* LIBuffer.H
* Class for reading in values from a specified file to a LIBuffer.
*/

#ifndef LIBUFFER_h
#define LIBUFFER_h

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/**
@brief class to store each record and parse each field
*/
class LIBuffer {
public:

	/**
	@brief Constructor for the LIBuffer class
	@pre Takes in the address to the us_postal_codes.csv file
	@post inFile, index and buf are all initialized
	*/
	LIBuffer();
	LIBuffer(char, int);

	/**
	@brief reads from csv file and places on string
	@post returns the string of one line of us_postal_codes.csv
	*/
	bool read(fstream& inFile, unsigned long offset);

	void write(fstream& outFile);

	/**
	@brief Seperates each field from the line on the LIBuffer
	@pre LIBuffer must not be empty
	@post Makes parameter string equal to correct field in record
	*/
	bool unpack(string& field);

	void pack(string& field);

	/**
	@brief Gives the LIBuffer string
	@post Returns the LIBuffer string
	*/
	string getBuffer() { return buf; }
	

	int getSize() { return buf.size(); }


private:

	int size;
	char delim;
	int maxsize;
	int index;
	string buf;
};
#endif