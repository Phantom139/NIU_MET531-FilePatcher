#include "includes.h"
#include "tools.h"
#include "data.h"

#ifndef CLEANFILE_H
#define CLEANFILE_H

//cleanFile(): Command "1": Clean 'M' from file.
void cleanFile(string &curLine, int counter, fstream *in, string oName) {
	vector<string> repLine;
	size_t tPos, tPos2, tPos3;
	bool okRead = false;
	string temp, temp2, temp3;
	int max, min;
	char cTmp[8];
	std::cout << "Cleaning File..." << endl;
	while (!in->eof()) {
		getline(*in, curLine);
		//std::cout << "Read: " << curLine.c_str();
		//Replace "M" with " "
		if (curLine.find(",M") != string::npos) {
			//std::cout << "Found 'M': " << curLine.c_str() << endl;
			counter++;
		}
		Tools::find_and_replace(curLine, ",M", ",NA()");
		repLine.push_back(curLine);
	}
	//Write...
	std::cout << "Fixing Bad Data and Writing File..." << endl;
	fstream out(oName.c_str(), ios::out);
	for (int i = 0; i < repLine.size(); i++) {
		//Filter out bad zeros...
		if (okRead) {
			tPos = repLine[i].find(",");
			tPos = repLine[i].find(",", tPos + 1);
			tPos = repLine[i].find(",", tPos + 1);
			tPos = repLine[i].find(",", tPos + 1);
			tPos3 = tPos;
			tPos2 = repLine[i].find(",", tPos + 1);
			temp = repLine[i].substr(tPos + 1, tPos2 - (tPos + 1)).c_str();
			tPos = repLine[i].find(",", tPos2 + 1);
			temp2 = repLine[i].substr(tPos2 + 1, tPos - (tPos2 + 1)).c_str();
			max = atoi(temp.c_str());
			min = atoi(temp2.c_str());
			//Validate it...
			if (!Tools::okCountThreshold(repLine[i], temp, temp2, max, min, true)) {
				std::cout << "Bad data point thrown..." << endl;
				counter++;
				temp = "=NA()";
				temp2 = "=NA()";
			}
			else {
				sprintf(cTmp, "%i", max);
				temp = cTmp;
				sprintf(cTmp, "%i", min);
				temp2 = cTmp;
			}
			//Write
			if (temp.compare("0") != 0 || temp2.compare("0") != 0) {
				temp3 = repLine[i].substr(0, tPos3) + "," + temp + "," + temp2 + ",";
			}
			else {
				temp3 = repLine[i];
			}
			out << temp3.c_str() << "\n";
		}
		else {
			if (repLine[i].substr(0, 5).compare("Date,") == 0) {
				std::cout << "First Line Found, Engaging Data Cleaning..." << endl;
				okRead = true;
			}
			out << repLine[i].c_str() << "\n";
		}
	}
	out.close();
	std::cout << "Replaced " << counter << " occurances..." << endl;
	std::cout << "Command '1' Completed..." << endl << endl;
}

#endif