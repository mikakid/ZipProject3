/**
* LIBuffer.CPP
* Member function definitions for the LIBuffer class.
*/
#include "LIBuffer.h"
#include "zip.h"
#include <iostream>
#include <string> 

using namespace std;

/*
@brief General constructor for theLIBuffer 
*/
LIBuffer::LIBuffer() {

	delim = ',';
	index = 0;
	buf = "";
	size = 0;
	maxsize = 1000;

}

/*
@brief Builds the specified constructor. 
@pre Receives the delimeter, and the maximum integer.
@param1 delim a character which is a comma.  
@param2 maxsize an int which is the maxsizef the buffer.
*/
LIBuffer::LIBuffer(char delim = ',', int maxsize = 1000) {

	index = 0;
	buf = "";
	size = 0;
}

/*
@brief Reads from the length-indicated file. 
@pre Receives a file and the and the offset for a record. 
@param1 inFile an ifstream variable which contains the address of the length-indicated file.  
@param2 offset an integer variable which contains the offset for the specific record.
*/
bool LIBuffer::read(fstream& dFile, unsigned long offset) {
	char temp1, temp2;
	int length;
	index = 0;
	buf = "";
	size = 0;
	
	dFile.seekg(offset);	// seek to start of record
	
	temp1 = dFile.get();  // get first two characters to decode length
	temp2 = dFile.get();
	buf.push_back(temp1); // add characters to buffer
	buf.push_back(temp2);

	length = stoi(buf); // convert length ascii to int

	buf = "";

	for (int i = 0; i < length; i++) { // copy as many characters as the length indicates
		dFile.get(temp1);
		buf.push_back(temp1);
		size++;
	}

	return true;
}

/*
@brief Writes to the index file. 
@pre Receives the index file and a instance of zip class. 
@param1 outFile an ofstream variable that contains the index file address.  
@param2 record a zip class variable.
@post returns true or false if the file wrote correctly.
*/
void LIBuffer::write(fstream& outFile) {
	buf.insert(0, to_string(buf.size()));
	outFile << buf;
	buf = "";
}

/*
@brief Unpacks a string of record fields. 
@pre Receives a string of fields. 
@param1 field a string reference of record fields.  
@post Returns true if string was unpacked else it returns false.
*/
bool LIBuffer::unpack(string& field) {
	if (index != size && size != 0) { // execute only when LIBuffer is not empty

		while (index < size && buf[index] != delim) { // 
			field.push_back(buf[index++]);
		}
		if (buf[index] == delim)
			index++;
		return true;

	}
	return false;
}

void LIBuffer::pack(string& field) {
	buf.append(field);
}