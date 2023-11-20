/**
* ZIP.CPP
* Member function definitions for the zip class.  
*/

#include <iostream>
#include <string>
#include <new>
#include "zip.h"

using namespace std;

//Default Constructor
zip::zip()
{
	num = -1;
	city = "";
	stateCode = "";
	county = "";
	lat = 0;
	lon = 0;
}

//Specified Constructor
zip::zip(int newNum, string newCity, string newStateCode, string newCounty, float newLat, float newLon)
{
	num = newNum;
	city = newCity;
	stateCode = newStateCode;
	county = newCounty;
	lat = newLat;
	lon = newLon;
}

// copy constructor
zip::zip(const zip& oldZip) 
{
	num = oldZip.num;
	city = oldZip.city;
	stateCode = oldZip.stateCode;
	county = oldZip.county;
	lat = oldZip.lat;
	lon = oldZip.lon;
}

const int zip::getSize() {

	string size = "";
	string temp = "";

	temp.append(to_string(num));
	size.push_back(',');
	temp.append(city);
	size.push_back(',');
	temp.append(stateCode);
	size.push_back(',');
	temp.append(county);
	size.push_back(',');
	temp.append(to_string(lat));
	size.push_back(',');
	temp.append(to_string(lon));

	size.append(to_string(temp.size()));
	size.push_back(',');
	size.append(temp);

	return size.size();
}