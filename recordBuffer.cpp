/*
* recBuf.cpp
*/

#include "recBuf.h"


void recBuf::pack(zip& z) {

	string temp;

	temp.push_back(',');
	temp.append(to_string(z.getNum()));
	temp.push_back(',');
	temp.append(z.getCity());
	temp.push_back(',');
	temp.append(z.getStateCode());
	temp.push_back(',');
	temp.append(z.getCounty());
	temp.push_back(',');
	temp.append(to_string(z.getLat()));
	temp.push_back(',');
	temp.append(to_string(z.getLon())); // get long
	buf.append(to_string(temp.size() + 2));
	buf.append(temp);
}

void recBuf::read(const string& recText) {

	buf = recText;

}

void recBuf::write(string& blockText){

	blockText.append(buf);
	buf = "";
}

bool recBuf::unpack(zip& z) {
	
	string temp = "";
	int size = buf.size();
	temp = "";

	index = 0;

	int fieldNumber = 1;

	if (index != size ) { // execute only when buf is not empty
		for (int i = 0; i < size && index < buf.size() + 1; i++) {
			if (buf[index] != delim){
				temp.push_back(buf[index]);
				if (buf[index + 1] == delim || index + 1 == size) {
					switch (fieldNumber) {
					case 1: z.setNum(stoi(temp)); break;
					case 2: z.setCity(temp); break;
					case 3: z.setStateCode(temp); break;
					case 4: z.setCounty(temp); break;
					case 5: z.setLat(stof(temp)); break;
					default: z.setLon(stof(temp)); break;

					}
					fieldNumber++;
					temp = "";
				}
			}
			index++;
		}
	return true;
	}

	return false;
}