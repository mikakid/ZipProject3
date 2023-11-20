/*
*block.cpp
*/
#include "block.h"

block::block(){
	active = 0;
	recCount = 0;
	currentSize = headerSize() + 1;
	highestZip = 0;
	prev = 0; 
	next = 0;
	records.clear();
}

block::block(const block& old){

	active = old.active;
	recCount = old.recCount;
	currentSize = old.currentSize;
	highestZip = old.highestZip;
	prev = old.prev;
	next = old.next;
	for (int i = 0; i < old.records.size(); i++)

		records.push_back(old.records[i]);

}

block::block(block& b1, block& b2) {

	if (b2.highestZip < b1.highestZip)
		swap(b1, b2);


	for (int i = 0; i < b1.records.size(); i++) {
		this->records[i] = b1.records[i];
	}
	int j = 0;
	for (int i = b1.records.size(); i < (b1.records.size() + b2.records.size()); i++) {
		this->records[i] = b2.records[j];
		j++;
	}

	active = true;
	this->recCount = b1.recCount + b2.recCount;
	this->currentSize = b1.currentSize + b2.currentSize;
	this->prev = b1.prev;
	this->next = b2.next;
	b2.active = false;
}

bool block::addRecord(zip& newzip){

	int tempsize = headerSize();

	int count = zipSize(newzip);
	if (count + currentSize < 512) {
		currentSize += count;
		
		if (recCount == 0) {
			records.push_back(newzip);
			recCount++;
		}
		else{
			if (records.size() == recCount) {
				bool placed = false;
				for (int i = 0; i < records.size() && !placed; i++) {
					if (newzip.getNum() < records[i].getNum()) {

						records.insert(records.begin() + i, newzip);
						recCount++;
						placed = true;

					}
				}
				if (!placed) {
					records.push_back(newzip);
					recCount++;
				}
			}
			else
				records.push_back(newzip);
		}

		findHighestZip();

		currentSize = (currentSize - tempsize) + headerSize();

		return true;
	}
	else
		return false;

}

void block::split(block& newBlock){
	
	int change = 0;
	int start = (records.size() / 2);

	for(int i = start; i < records.size(); i++){
		newBlock.addRecord(records[i]);
		change++;
	}

	for(int j = 0; j < change; j++){
		delRecord(records[start].getNum());
	}
	newBlock.setNext(getNext());
	findHighestZip();
	newBlock.findHighestZip();
}

bool block::delRecord(int zip){

	for(int i = 0; i < recCount; i++){
		if(records[i].getNum() == zip && records.size() >= recCount){
			int count = zipSize(records[i]);
			records.erase(records.begin() + i);
			recCount--;
			currentSize -= count;
			findHighestZip();
			if (currentSize == 0)
				active = false;
			return true;
		}
	}

	return false;
}

int block::findHighestZip(){

	highestZip = 0;
	for (int i = 0; i < records.size(); i++) {
		if (highestZip < records[i].getNum())
			highestZip = records[i].getNum();
	}
	return highestZip;
}

int block::zipSize(zip& zipper){

	string size = "";
	string temp = "";

	temp.append(to_string(zipper.getNum()));
	size.push_back(',');
	temp.append(zipper.getCity());
	size.push_back(',');
	temp.append(zipper.getStateCode());
	size.push_back(',');
	temp.append(zipper.getCounty());
	size.push_back(',');
	temp.append(to_string(zipper.getLat()));
	size.push_back(',');
	temp.append(to_string(zipper.getLon()));

	size.append(to_string(temp.size()));
	size.push_back(',');
	size.append(temp);

	return size.size();
}

int block::headerSize()
{
	string size = "";

	size.append(to_string(prev) + ',');
	size.append(to_string(next) + ',');
	size.append(to_string(recCount) + ',');
	size.append(to_string(currentSize) + ',');
	size.append(to_string(highestZip) + ';');

	return size.size();
}

void block::getRecords(vector<zip>& rtn){
	for (int i = 0; i < records.size(); i++) {
		rtn.push_back(records[i]);
	}
}

bool block::getZip(zip& rtn, int target){
	for(int i = 0; i < records.size(); i++){
		if(records[i].getNum() == target){
			rtn.setNum(records[i].getNum());
			rtn.setCity(records[i].getCity());
			rtn.setStateCode(records[i].getStateCode());
			rtn.setCounty(records[i].getCounty());
			rtn.setLon(records[i].getLon());
			rtn.setLat(records[i].getLat());
			return true;
		}
	}
	return false;
}

