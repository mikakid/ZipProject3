/**
* primaryindex.cpp
* Member functions for the primaryindex class. 
*/


#include "primaryindex.h"


using namespace std;

static const short numStates = 57; // number of possible states/regions


void primaryIndex::getIndex(vector<indexElement>& rtn){

	indexElement temp;

	for (int i = 0; i < index.size(); i++) {

		temp.zip = index[i].zip;
		temp.offset = index[i].offset;

		rtn.push_back(temp);

	}
}
/*
* @brief Adds the zip code and byte offset to the primaryindex class.    
* @param1 z an integer containing the zipcode.
* @param2 o an integer containing the byte offset.
*/

void primaryIndex::add(int z, unsigned long o) {

	indexElement temp = {z, o};
	int i = 0;

	if (recCount == 0) {

		index.push_back(temp);

	} else {

		while (i < recCount && index[i].zip < z) {

			i++;

		}
		if (i == recCount) {

			index.push_back(temp);

		}

		else if (i == 0) {

			index.insert(index.begin(), temp);

		} else {

			//i--;
			vector<indexElement>::iterator it;
			it = index.begin();
			index.insert(it + i, temp);

		}
	}

	recCount++;

}

/*
* @brief simple binary search algorithm.
* @pre target value must be specified as first argument.
* @post -1 return means value not in list.
* @param target value, left-most index, right-most index.
*/

unsigned long primaryIndex::search(int target) {

	int off = binSearch(target, 0, recCount - 1);
	return off;

}

/*
*@brief binary search for index file
*@pre target value must be valid zip or -1 is returned
*@post returns offset of zip if zip is valid
*/

unsigned long primaryIndex::binSearch(int target, int l, int r) {

	if (l > r) { 
		return 0; 
	}

	if (r >= l) {

		int mid = (l + r) / 2;		//nothing to see here

		if (index[mid].zip == target) {

			cout << "Target hit!: " << index[mid].zip << ' ' << index[mid].offset << endl;
			return index[mid].offset;

		}

		else if (index[mid].zip > target)

			binSearch(target, l, mid - 1);

		else   //index[mid].getZip() < target

			binSearch(target, mid + 1, r);	
	}
}



/*
* @brief simple binary search algorithm.
* @pre target value must be specified as first argument.
* @post -1 return means value not in list.
* @param target value, left-most index, right-most index.
*/

void primaryIndex::writeToFile(){
	
	ofstream outFile;

	outFile.open("IndexFile.index");

	outFile << recCount << "\n";

	for (int i = 0; i < recCount; i++) {
		outFile << index[i].zip << "," << index[i].offset << "\n";
	}
}

/*
* @brief reads entire index from storage into memory
* @pre The ifile is not at the end of file marker.  
*/

void primaryIndex::readIndex() {
	if (!iFile.eof()) {
		int itemp;
		iFile >> itemp;
		cout << itemp << " records in the file." << endl;

		int z;
		unsigned long int o;
		char temp;

		while (!iFile.eof()) {
			
			iFile >> z >> temp >> o;
			add(z, o);

		}


	}
}

/*
* @brief places csv file contents onto vector of vector of zip objects, prints the
*  most north, south, east, and west, and calls transfer to create a length indicated csv
* @pre ifile object must be created
* @post
* @param ifile object passed by reference
*/

void primaryIndex::readCSV(ifstream& infile){

	iFile.open("IndexFile.index");
	dFile.open("DataFile.licsv");

	vector<vector<zip>> states;
	states.resize(numStates);
	string headerData = readIn(infile, states);
	cout << endl << printTable(states) << endl;

	transfer(states, headerData);

	writeToFile();

	
}

/*
@brief Builds the header architecture.
@pre Receives a a data file and the input file.
@param1 oFileD an ofstream variable which contains the address of the length-indicated file.
@param2 oFileI an ofstream variable which contains the address of the index file.
*/

string primaryIndex::buildHeader(string headerData) {
	string record;
	int count = 1, temp;


	record.append("Structure Type: Length Indicated Comma Separated File\n");
	record.append("Version: 1.0\n");

	//Determine size of record
	record.append("Record Size: ");
	record.append(to_string(headerData.size()));
	record.push_back('\n');

	//Size format type
	record.append("Size Format: 2-digit ASCII\n");

	//Index File Name
	record.append("Index File: IndexFile.index\n");

	//Index File Schema Information
	record.append("Index File Schema: Listed by zip code then corresponding offset, sorted by zip code\n");

	//Record Count
	record.append("Record Count: 40933\n");

	//Count of fields per record 
	for (int j = 0; j < headerData.size(); j++) {
		if (headerData[j] == ',') {
			count++;
		}
	}

	record.append("Fields per Record: ");
	record.append(to_string(count));
	record.push_back('\n');

	//Name of each field
	record.append("Name of Fields: ");
	record.append(headerData);
	record.push_back('\n');

	//Type Schema
	record.append("Type Schema: Zip Code is an integer. Lon and Lat are floating point decimals. County, State Code, and City are strings.\n");

	//Indicate which field is primary key
	record.append("First Key: Zip Code\n");
	return record;

}

/*
@brief Sends data from vector of vectors to length-indicated file.
@pre Takes in the length-indicated data file, the index file, the vector of vectors, and the list of indices.
@param1 oFileD an ofstream variable which contains the address of the length-indicated file.
@param2 oFileI an ofstream variable which contains the address of the index file.
@param3 states a vector<vector<zip>> which contains all of records from the given csv.
@param4 indexList a primaryIndex class reference which stores the indices
*/
void primaryIndex::transfer(vector<vector<zip>>& states, string headerData) {
	if (!dFile.is_open())(dFile.open("DataFile.licsv"));

	string header = buildHeader(headerData);
	dFile << header;

	string temp;
	LIBuffer buf;
	unsigned long count = 0;
	unsigned long offsetSum = header.size() + 11;

	for (int i = 0; i < numStates; i++) {
		for (int j = 0; j < states[i].size(); j++) {
			count = 0;

			temp = to_string(states[i][j].getNum());
			temp.push_back(',');
			temp.append(states[i][j].getCity());
			temp.push_back(',');
			temp.append(states[i][j].getStateCode());
			temp.push_back(',');
			temp.append(states[i][j].getCounty());
			temp.push_back(',');
			temp.append(to_string(states[i][j].getLat()));
			temp.push_back(',');
			temp.append(to_string(states[i][j].getLon()));

			count = temp.size();
			
			buf.pack(temp);
			buf.write(dFile);

			
			add(states[i][j].getNum(), offsetSum);
			offsetSum += count + 2;
		}
	}
}

/**
@brief Read in data from the csv, place on buffer,
*and parse onto zip class data members;
@pre Receives address of the file stream,
*receives a pointer to an array of state vectors.
@post zip code records have been read into zip objects,
*zip objects have been sorted to their respective state vectors.
*/
string primaryIndex::readIn(ifstream& inFile, vector<vector<zip>>& states) {

	zip temp; // temporary storage for parsed record
	string item; // stores one field at a time before it's added to temp
	string headerData;
	vector<string> headerElement;
	delimBuffer b; // create buffer object

	// read header record and append to headerData string
	for (int i = 0; i < 3; ++i) {
		b.read(inFile);
		headerData.append(b.getBuffer());

	}

	// places each element of the entire header onto seperate elements of a vector
	//
	for (int i = 0; i < headerData.size(); i++) {
		headerData[i] = tolower(headerData[i]);
	}
	for (int i = 0, j = 0, k = 0; i < headerData.size(); ++i, k = 0) {
		string temps = "";
		while (headerData[i] != ',' && i < headerData.size()) {
			if (isalpha(headerData[i])) {
				temps.push_back(headerData[i]);
			}
			i++;
		}

		headerElement.resize(headerElement.size() + 1);
		headerElement[j++] = temps;
	}

 	int tag = 0;

	while (b.read(inFile)) { // loop until end of file

		while (b.unpack(item)) {
			if (tag > 5) tag = 0;

			if (headerElement[tag] == "zipcode") {
				temp.setNum(stoi(item));	//If it is a zip code
			}
			else if (headerElement[tag] == "placename") {
				temp.setCity(item);			//If it is a city
			}
			else if (headerElement[tag] == "state") {
				temp.setStateCode(item);	//If it is a state code
			}
			else if (headerElement[tag] == "county") {
				temp.setCounty(item);		//if it is a county
			}
			else if (headerElement[tag] == "lat") {
				temp.setLat(stof(item));	//If it is a latitude
			}
			else if (headerElement[tag] == "long") {
				temp.setLon(stof(item));	//If it is a longitude
			}
			++tag;
			item = "";
		}
		states[stateChooser(temp.getStateCode())].push_back(zip(temp));
		// new zip object with same values as temp is added to the end of the corresponding state vector
		// cooresponding entry is placed into primary index
	}

	return headerData;

}



/**
@brief Prints the state arrays zip code state code
@pre Receives the array of state objects
@post prints a table of the most north, south, east,
* and west zip codes of each state
*/
string primaryIndex::printTable(vector<vector<zip>>& states) {

	string output;

	output.append("*****************************************************\n");
	output.append("*State\t|East\t\t|West\t\t|North\t\t|South\t*\n");
	output.append("*****************************************************\n");
	
	for (int i = 0; i < numStates; i++) {

		output.append("*");
		output.append(states[i][0].getStateCode());
		output.append("\t|");
		output.append(to_string(states[i][mostEast(states[i])].getNum()));
		output.append("\t\t|");
		output.append(to_string(states[i][mostWest(states[i])].getNum()));					// MoWestuth
		output.append("\t\t|");
		output.append(to_string(states[i][mostNorth(states[i])].getNum()));
		output.append("\t\t|");
		output.append(to_string(states[i][mostSouth(states[i])].getNum()));					// MosSouthst
		output.append("\t*\n");
	}
	output.append("*****************************************************\n");
	output.append("*State\t|East\t\t|West\t\t|North\t\t|South\t*\n");
	output.append("*****************************************************");
	
	return output;
}



/**
 * @brief Finds the most north zipcode of a given state
 * @pre Takes an element of the state array.
 * @post returns the index of the most north zipcode.
 */
short primaryIndex::mostNorth(vector<zip> state) {
	short x = 0;
	for (int i = 1; i < state.size(); i++) {
		if (state[i].getLat() > state[i - 1].getLat())
			x = i;
	}
	return x;
}

/**
 * @brief Finds the most south zipcode of a given state
 * @pre Takes an element of the state array.
 * @post returns the index of the most south zipcode.
 */
short primaryIndex::mostSouth(vector<zip> state) {
	short x = 0;
	for (int i = 1; i < state.size(); i++) {
		if (state[i].getLat() < state[i - 1].getLat())
			x = i;
	}
	return x;
}

/**
 * @brief Finds the most "esta" zipcode of a given state
 * @pre Takes an element of the state array.
 * @post returns the index of the most east zipcode.
 */
short primaryIndex::mostEast(vector<zip> state) {
	short x = 0;
	for (int i = 1; i < state.size(); i++) {
		if (state[i].getLon() < state[i - 1].getLon())
			x = i;
	}
	return x;
}

/**
 * @brief Finds the most west zipcode of a given state
 * @pre Takes an element of the state array.
 * @post returns the index of the most west zipcode.
 */
short primaryIndex::mostWest(vector<zip> state) {
	short x = 0;
	for (int i = 1; i < state.size(); i++) {
		if (state[i].getLon() > state[i - 1].getLon())
			x = i;
	}
	return x;
}





/******************************************STOP SCROLLING FOR YOUR OWN GOOD******************************************/

// You've been warned!!

// Last chance to turn around!

// Point of 58 returns;

/**
* @brief Chooses which state array index is correct
* with the use of a switch statement
* @pre two character state code in a string is used as parameter
* @post Returns the correct array index as an int
*/
short primaryIndex::stateChooser(string x) {

	if (x == "AA")	return 0;	//C++ doesn't like strings and switch statements together ;(. Amenrican Airlines?	
	else if (x == "AK") return 1;
	else if (x == "AL") return 2;
	else if (x == "AP") return 3;
	else if (x == "AR") return 4;
	else if (x == "AZ") return 5;
	else if (x == "CA") return 6;
	else if (x == "CO") return 7;
	else if (x == "CT") return 8;
	else if (x == "DC") return 9;
	else if (x == "DE") return 10;
	else if (x == "FL") return 11;
	else if (x == "FM") return 12;
	else if (x == "GA") return 13;
	else if (x == "HI") return 14;
	else if (x == "IA") return 15;
	else if (x == "ID") return 16;
	else if (x == "IL") return 17;
	else if (x == "IN") return 18;
	else if (x == "KS") return 19;
	else if (x == "KY") return 20;
	else if (x == "LA") return 21;
	else if (x == "MA") return 22;
	else if (x == "MD") return 23;
	else if (x == "ME") return 24;
	else if (x == "MH") return 25;
	else if (x == "MI") return 26;
	else if (x == "MN") return 27;
	else if (x == "MO") return 28;
	else if (x == "MP") return 29;
	else if (x == "MS") return 30;
	else if (x == "MT") return 31;
	else if (x == "NC") return 32;
	else if (x == "ND") return 33;
	else if (x == "NE") return 34;
	else if (x == "NH") return 35;
	else if (x == "NJ") return 36;
	else if (x == "NM") return 37;
	else if (x == "NV") return 38;
	else if (x == "NY") return 39;
	else if (x == "OH") return 40;
	else if (x == "OK") return 41;
	else if (x == "OR") return 42;
	else if (x == "PA") return 43;
	else if (x == "PW") return 44;
	else if (x == "RI") return 45;
	else if (x == "SC") return 46;
	else if (x == "SD") return 47;
	else if (x == "TN") return 48;
	else if (x == "TX") return 49;
	else if (x == "UT") return 50;
	else if (x == "VA") return 51;
	else if (x == "VT") return 52;
	else if (x == "WA") return 53;
	else if (x == "WI") return 54;
	else if (x == "WV") return 55;
	else if (x == "WY") return 56;
	else /****HELP****/ return -1;

}