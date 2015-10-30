/*
data.h

provides the Data class
*/

#include <string>
#include <vector>

using namespace std;

#ifndef DATA_H
#define DATA_H

class Data {
	public:
		Data(int count = -1);

		void scanFirstDataLine(string line);
		void addData(double doubleVal = -999);
		void emptyData();
		bool lineIsEnd(string line);

		int getFields() { return fieldCount; }
		vector<double> getData() { return fields; }

	private:
		int fieldCount;
		vector<double> fields;
};

#endif