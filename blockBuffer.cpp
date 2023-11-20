/*
* BlockBuf.cpp
*/

#include "blockBuf.h"


void blockBuf::read(ifstream& inFile, int RBN) {
	
	unsigned long NBR(RBN * 512);
	int index = 0;
	inFile.seekg(NBR);
	while(!inFile.eof() && index != BUFSIZE) {
		blockText.push_back(inFile.get());
		index++;
	}
	index = 0;
}

void blockBuf::pack(block &b){

	recBuf rec;
	vector<zip> records;

	string temp = writeHeader(b);
	blockText.append(temp);
	
	//Store records
	b.getRecords(records);
	for(int i = 0; i < records.size(); i++){
		rec.pack(records[i]);
		rec.write(blockText);
	}

}


void blockBuf::write(ofstream& outfile, int RBN){
	unsigned long NBR = (RBN * 512);
	outfile.seekp(NBR);

	for (int i = 0; i < 512; i++) {
		if (i < blockText.size()) {
			outfile << blockText[i];
		}
		else
			outfile << " ";
	}
	outfile.flush();
	blockText = "";
}


void blockBuf::unpack(block& b) {


	readHeader(b);

	zip tempZip;
	string temp;
	recBuf rec;
	int recSize;
	int count;
	int numRecs = b.getRecCount();
	int recCounter = 0;
	int tempCurrentSize = b.getCurrentSize();

	b.setCurrentSize(0);

	while (index != tempCurrentSize) {
		if (recCounter == numRecs)
			break;

		while (blockText[index] != ',') {
			temp.push_back(blockText[index++]);
		}
		index++;
		recSize = stoi(temp) - 3;
		temp = "";
		count = 0;
		while (index != blockText.size() && count != recSize) {
			temp.push_back(blockText[index++]);
			count++;
		}

		rec.read(temp);
		rec.unpack(tempZip);

		b.addRecord(tempZip);
		recCounter++;
		temp = "";
	}
	index = 0;
	blockText = "";
	b.setCurrentSize(tempCurrentSize);
}


void blockBuf::readHeader(block& b) {

	int headerSize;
	index = 0;
	string temp;

	for (index; blockText[index] != ','; index++) {
		temp.push_back(blockText[index]);
	}
	index++;
	b.setPrev(stoi(temp));
	temp = "";

	for (index; blockText[index] != ','; index++) {
		temp.push_back(blockText[index]);
	}
	index++;
	b.setNext(stoi(temp));
	temp = "";

	for (index; blockText[index] != ','; index++) {
		temp.push_back(blockText[index]);
	}
	index++;
	b.setRecCount(stoi(temp));
	temp = "";

	for (index; blockText[index] != ','; index++) {
		temp.push_back(blockText[index]);
	}
	index++;
	b.setCurrentSize(stoi(temp));
	temp = "";

	for (index; blockText[index] != ';'; index++) {
		temp.push_back(blockText[index]);
	}
	index++;
	b.setHighestZip(stoi(temp));
	temp = "";
}

string blockBuf::writeHeader(block& b){
	string header, temp;
	
	// store block attributes as ascii
	header.append(to_string(b.getPrev()));
	header.push_back(',');
	header.append(to_string(b.getNext()));
	header.push_back(',');
	header.append(to_string(b.getRecCount()));
	header.push_back(',');
	header.append(to_string(b.getCurrentSize()));
	header.push_back(',');
	header.append(to_string(b.getHighestZip()));
	header.push_back(';');
	return header;
}
