/**
* ZIP.H
* Class encapsulating the zip code, city name, state code,
* county, latitude and longitude for every postal code.   
*/


#ifndef ZIP
#define ZIP

#include <iostream>
#include <string>
using namespace std;

/**
@brief class to store each zip code as an object
*/
class zip {
public:

	/**
	@brief default constructor
	@post initializes zip object to be empty
	*/
	zip();

	/**
	@brief specified constructor
	@pre Takes in the zipcode, city of zipcode, 2 character string statecode,
	*string for the county, floating point of the latitude, and floating point of the longitude.
	*/
	zip(int newNum, string newCity, string newStateCode, string newCounty, float newLat, float newLon);

	/**
	@brief copy constructor
	*/
	zip(const zip& oldZip);

	/**
	@brief Inline setters and getters
	*/

	void setNum(int newNum) { num = newNum; };

	const int getNum() { return num; };

	void setCity(string newCity) { city = newCity; };

	const string getCity() { return city; };

	void setStateCode(string newStateCode) { stateCode = newStateCode; };

	const string getStateCode() { return stateCode; };

	void setCounty(string newCounty) { county = newCounty; };

	const string getCounty() { return county; };

	void setLat(float newLat) { lat = newLat; };

	const float getLat() { return lat; };

	void setLon(float newLon) { lon = newLon; };

	const float getLon() { return lon; };

	const int getSize();

	void print() {
		cout << "\nZip Code:" << to_string(num) << ", City: " << city << ", County: " << county 
			<< ", stateCode: " << stateCode << ", Lat: " << to_string(lat) << ", Lon: " << to_string(lon) << "\n";
	}
 
private:
	int num;
	float lat;
	float lon;
	string stateCode;
	string city;
	string county;
};
#endif