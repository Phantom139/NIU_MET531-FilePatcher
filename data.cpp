/*
data.cpp
*/
#include "data.h"

Data::Data(int count) {
	if (count != -1) {
		fieldCount = count;
	}
}

//Scan the first line of data to fetch the input.
int Data::scanFirstDataLine(string line) {
	int counter = 0;
	string temp = line;
	while (temp.find(",") != string::npos) {
		counter++;
		temp = temp.substr(temp.find(","), temp.length());
	}
	return counter;
}

void Data::addData(double doubleVal) {
	fields.push_back(doubleVal);
}

void Data::emptyData() {
	fields.clear();
}

bool Data::lineIsEnd(string line) {
	int counter = 0;
	string temp = line;
	while (temp.find(",") != string::npos) {
		counter++;
		temp = temp.substr(temp.find(","), temp.length());
	}
	return fieldCount > counter;
}