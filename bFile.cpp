

#include "blockFile.h"



blockFile::blockFile() {

	string index = "IndexFile.index"; 
	string data = "DataFile.licsv";

	oData.open("DataFile.bli");
	iData.open("DataFile.bli");

	for (int i = 0; i < 1024; i++) {
		oData << '0';
	}

	liToBlock(index, data); // dtatata

}


void blockFile::liToBlock(string in, string liData) {

	recBuf recParser; // parse LI records onto zip object 
	LIBuffer libuf;
	zip z;
	primaryIndex pi(in, liData);

	vector<indexElement> ind;
	pi.getIndex(ind);

	fstream lid;
	lid.open(liData);

	

	for (int i = 0; i < ind.size(); i++) {

		libuf.read(lid, ind[i].offset);
		recParser.read(libuf.getBuffer());
		recParser.unpack(z);

		addRecord(z);

	}
	oData.seekp(0);
	oData << writeHeader();
}

bool blockFile::delRecord(string zip){

	block temp, tempPrev, tempNext;
	int rbn = index.search(stoi(zip));
	int rbnPrev;
	int rbnNext;

	if (rbn == 0)
		return false;

	else{

		buf.read(iData, rbn);
		buf.unpack(temp);

		rbnPrev = temp.getPrev();
		rbnNext = temp.getNext();

		if (temp.delRecord(stoi(zip))){
			if(temp.getCurrentSize() < 256){
				if (rbnPrev != 0) {

					buf.clear();
					buf.read(iData, rbnPrev);
					buf.unpack(tempPrev);

				}
				if(rbnNext != 0){

					buf.clear();
					buf.read(iData, rbnNext);
					buf.unpack(tempNext);

				}
				if (tempPrev.getCurrentSize() < 256 && temp.getCurrentSize() < 256) {

					block merge(tempPrev, temp);

					index.add(merge, rbnPrev);
					buf.pack(merge);
					buf.write(oData, rbnPrev);

					tempNext.setPrev(rbnPrev);

					buf.clear();
					buf.pack(tempNext);
					buf.write(oData, rbnNext);

					return true;
				}
				
					
				if (tempNext.getCurrentSize() < 256 && temp.getCurrentSize() < 256) {

					block merge(temp, tempNext);

					index.add(merge, rbnNext);
					buf.pack(merge);
					buf.write(oData, rbnNext);

					tempPrev.setNext(rbnNext);

					buf.clear();
					buf.pack(tempPrev);
					buf.write(oData, rbnPrev);

					return true;
				}	
			}

			return true;

		}
		else
			return false;
	}
}

bool blockFile::addRecord(zip& z) {

	block temp1, temp2;
	temp1.setActive(true);
	int rbn = index.search(z.getNum());

	if (rbn == 0) {

		if (numBlocks == 0)
			numBlocks++;
		rbn = index.findHighest();

		if (rbn == 0) {

			temp1.addRecord(z);
			temp1.setPrev(0);
			temp1.setNext(0);
			index.add(temp1, 1);

			buf.pack(temp1);
			buf.write(oData, 1);
			buf.clear();

			return true;
		}
		else {
			
			buf.read(iData, rbn);
			buf.unpack(temp1);
			buf.clear();
			index.add(temp1, rbn);

			if (!temp1.addRecord(z)) {

				split(temp1);
				addRecord(z);

			}
			else {

				buf.pack(temp1);
				buf.write(oData, rbn);
				buf.clear();
				return true;

			}
		}
	}
	else {

		buf.read(iData, rbn);
		buf.unpack(temp1);
		buf.clear();
		index.add(temp1, rbn);

		if (!temp1.addRecord(z)) {

			split(temp1);
			addRecord(z);
		}
		else {

			index.add(temp1, rbn);
			buf.pack(temp1);
			buf.write(oData, rbn);
			buf.clear();

			return true;
		}
	}
	return false;
}
///asdfasdfasdfasdfakljnsdfasdjf; fsadfasdfasfdasdfasdfasdfasdfasdfoja; asodfi; lfija; sodfjia; sldfij; akls j  as; dfj; djf; alskdjf; alksjdf;/ alksjdfsaodsd ofij   oas dj


void blockFile::readHeader() {

	string temp;
	iData.seekg(0);

	for (int i = 0; i < 512; i++) {

		temp.push_back(iData.get());

	}
}

string blockFile::writeHeader() {

	string header;

	//File Structure Type
	header.append("File Structure Type: Blocked sequence set with comma separated fields, and length-indicated records\n");

	//File Version
	header.append("Version: 1.0\n");

	//header record size
	header.append("Header Size: 512 bytes\n");

	//Size Format
	header.append("Format: ASCII\n");

	//Block size
	header.append("Block Size: 512 bytes\n");

	//Minimum block capacity
	header.append("Min Block Capacity: 256 bytes\n");

	//Index File Name
	header.append("Index File: TBD\n");

	//Index File Schema
	header.append("File Schema: Highest zip code and RBN\n");

	//record count
	header.append("Record Count: ");
	header.append(to_string(recCount));
	header.push_back('\n');

	//block count
	header.append("Block Count: ");
	header.append(to_string(numBlocks));
	header.push_back('\n');

	//Fields per record
	header.append("Fields: 6\n");

	//Fields
	header.append("ZipCode, Place Name, State, County, Lat, Long\n");
	
	//Type Schema
	header.append("Type Schema: Zip Code is an integer. Lon and Lat are floating point decimals. County, State Code, and City are strings.\n");

	//Primary key
	header.append("Zip code is the first key\n");

	//RBN link to avail list
	header.append("First Available Block: ");
	header.append(to_string(avail));
	header.push_back('\n');

	//RBN link to active list
	header.append("First Active Block: ");
	header.append(to_string(first)); //heerad
	header.push_back('\n');

	//Stale flag
	header.append("Stale: true");
	header.push_back('\n');

	return header;
}


string blockFile::pdump() // physical dump??
{
	string out = "";
	out.append("List Head: ");
	out.append(to_string(getFirst()));
	out.append("\nAvail Head: ");
	out.append(to_string(getAvail()));
	out.append("\n");
	
	block temp;
	vector<zip> records;

	for (int i = 1; i <= numBlocks; ++i) {

		buf.read(iData, i);
		buf.unpack(temp);
		buf.clear();

		if (temp.getActive()) {

			out.append("RBN Prev: ");
			out.append(to_string(temp.getPrev()));
			temp.getRecords(records);

			for(int j = 0; j < temp.getRecCount(); j++){

				out.push_back(' ');
				out.append(to_string(records[j].getNum()));
				out.push_back(' ');

			}

			out.append("RBN Next: ");
			out.append(to_string(temp.getNext()));
			out.append("\n");

		}
		else {

			out.append("RBN Prev:0\t*AVAILABLE*\tRBN Next: 0\n");

		}

	}

	return out;
}

string blockFile::ldump(){

	int rbn = 1;
	block temp;
	string zips;
	vector<zip> records;

	zips.append("List Head: ");
	zips.append(to_string(getFirst()));
	zips.append("\nAvail Head: ");
	zips.append(to_string(getAvail()));
	zips.append("\n");

	for(int i = 1; i <= numBlocks; ++i){

		if (rbn == 0)
			break; //brek; a

		else{

			buf.read(iData, rbn);
			buf.unpack(temp);
			buf.clear();

			if (temp.getActive()) {

				zips.append("RBN Prev: ");
				zips.append(to_string(temp.getPrev()));
				zips.push_back(' ');
				temp.getRecords(records);

				for (int j = 0; j < records.size(); j++) {

					zips.append(to_string(records[j].getNum()));
					zips.push_back(' ');
				}

				zips.append("RBN Prev: ");
				zips.append(to_string(temp.getNext()));
				zips.push_back('\n');
				rbn = temp.getNext();
			}
			else
				zips.append("RBN Prev:0\t*AVAILABLE*\tRBN Next: 0\n");
		}
	}

	return zips;
}


bool blockFile::split(block& b)
{
	if (b.getActive()) {

		int rbn;
		int tempi;
		block temp1, temp2;

		rbn = index.search(b.findHighestZip());
		b.split(temp1);
		//if (b.getNext() == 0)

		//if (b.getPrev() == 0)


			if (b.getNext() == 0) {
				temp1.setNext(0);

			}
			else {
				tempi = b.getNext();
				buf.read(iData, tempi);
				buf.unpack(temp2);

				temp1.setNext(tempi);
				temp2.setPrev(numBlocks);
				buf.clear();
			}

			b.setNext(++numBlocks);

			temp1.setPrev(rbn);
			temp1.setActive(true);

			b.findHighestZip();
			temp1.findHighestZip();

			buf.pack(b);
			buf.write(oData, rbn);
			buf.clear();
			index.add(b, rbn);

			buf.pack(temp1);
			buf.write(oData, numBlocks);
			buf.clear();
			index.add(temp1, numBlocks);

			return true;

		/*
		else{

			tempi = b.getNext();

			b.split(temp1);

			temp1.setNext(tempi);

			b.setNext(avail);

			buf.read(iData, avail);
			buf.unpack(temp2);
			buf.clear();

			avail = temp2.getNext();

			buf.read(iData, b.getPrev());
			buf.unpack(temp2);
			buf.clear();

			temp1.setPrev(temp2.getNext());
			temp1.setActive(true);

			buf.pack(b);
			buf.write(oData, temp2.getNext());
			buf.clear();

			buf.pack(temp1);
			buf.write(oData, b.getNext());
			buf.clear();

			return true;
		}
		*/
	}

	return false;
}
