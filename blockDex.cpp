/*
blockIndex.cpp
*/
#include "blockIndex.h"

using namespace std;


int blockIndex::search(int zip){
	
	if (index.size() == 0)
		return 0;

	int tempRBN = 0;
	bool found = false;
	int tempZip;

	for (int i = 0; i < index.size(); i++) {
		if(found){
			if (index[i].zip >= zip && index[i].zip < tempZip) {
				tempRBN = index[i].RBN;
				tempZip = index[i].zip;
			}
		}
		else if (index[i].zip >= zip){
			tempRBN = index[i].RBN;
			tempZip = index[i].zip;
			found = true;
		}
	}
	return tempRBN;
}

int blockIndex::findHighest(){
	int tempZip = 0;
	int tempRBN = 0;
	
	if(index.size() == 0) {
		return 0;
	} 

	for(int i = 0; i < index.size(); i++){
		if(index[i].zip > tempZip){
			tempZip = index[i].zip;
			tempRBN = index[i].RBN;
		} 
	}

	return tempRBN;
}

void blockIndex::add( block& b, int r) {
	bIndexElement temp;
	temp.zip = b.getHighestZip();
	temp.RBN = r;
	temp.active = true;
	del(r);
	index.push_back(temp);
}


void blockIndex::del(int r) {
	if(index.size() == 0){
		return;
	}
	for(int i = 0; i < index.size(); i++) {
		if(index[i].RBN == r){
			index.erase(index.begin() + i);
			break;
		}
	}
}

void blockIndex::printToFile(string out = "IndexFile.bindex") {

	ofstream oFile;
	oFile.open(out);

	oFile << numBlocks << ',' << numAvail << ';';

	for (int i = 0; i < index.size(); i++) {
		oFile << index[i].zip << ',' << index[i].RBN << ',' << index[i].active << ';';
	}

}

void blockIndex::readFromFile(string in = "IndexFile.bindex") {

	ifstream iFile;
	iFile.open(in);
	char trash;
	bIndexElement temp;

	if (iFile >> numBlocks >> trash >> numAvail >> trash) {

		index.resize(1);
		iFile >> index[0].zip >> trash >> index[0].RBN >> trash >> index[0].active >> trash;

		for (int i = 0; i < numBlocks && !iFile.eof(); i++) {

			iFile >> temp.zip >> trash >> temp.RBN >> trash >> temp.active >> trash;
			index.push_back(temp);

		}
	}
}
