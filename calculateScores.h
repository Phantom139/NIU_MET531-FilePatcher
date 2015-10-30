#include "includes.h"
#include "tools.h"

//calculateScores(): Command "2": Score thresholds and prepare a "plot-ready" file.
void calculateScores(string &curLine, char *charInput, int counter, fstream *in, string oName, bool automate) {
	string temp, temp2;
	int tMax = 999, tMin = 999, scor1 = 0, scor2 = 0, scor3 = 0;
	size_t tPos, tPos2;
	vector<string> repLine;
	//End
	repLine.push_back("PATCHED");
	while (!in->eof()) {
		getline(*in, curLine);
		//Patch the first line (Titles)
		if (curLine.find(",TMIN") != string::npos) {
			std::cout << "Patching First Line: " << curLine.c_str() << endl;
			curLine += ",THLD9,THLD0,THLD-10,SCOR9,SCOR0,SCOR-10,";
			repLine.push_back(curLine);
			repLine.push_back("START,");
			counter = 1; //<- Flag
		}
		else {
			//To patch the remainder of the file, read in each line and add the three threshold values until we
			// hit the blank line before the summary results.
			temp = curLine;
			Tools::find_and_replace(temp, ",", "");
			if (counter == 1 && strcmp(temp.c_str(), "") != 0) {
				//To calculate the scores, we look at the TMAX & TMIN fields, which are conviniently located in the back.
				// If one value is below a threshold, the score is 1. If both are, the score is 2, and if neither are, 0.
				// We need to find four commas.
				tPos = curLine.find(",");
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos2 = curLine.find(",", tPos + 1);
				temp = curLine.substr(tPos + 1, tPos2 - (tPos + 1)).c_str();
				if (temp.compare("=NA()") == 0) {
					tMax = 999;
				}
				else {
					tMax = atoi(temp.c_str());
				}
				tPos = curLine.find(",", tPos2 + 1);
				temp2 = curLine.substr(tPos2 + 1, tPos - (tPos2 + 1)).c_str();
				if (temp2.compare("=NA()") == 0) {
					tMin = 999;
				}
				else {
					tMin = atoi(temp2.c_str());
				}
				//Calculate Scores.
				//Robert: Updated code (10/21) to include bad data point filter/flagging
				scor1 = 0;
				scor2 = 0;
				scor3 = 0;
				if (Tools::okCountThreshold(curLine, temp, temp2, tMax, tMin, automate)) {
					//std::cout << " counting..." << endl;
					scor1 = (tMax <= 9 || tMin <= 9) ? 1 : 0;
					scor2 = (tMax <= 0 || tMin <= 0) ? 1 : 0;
					scor3 = (tMax <= -10 || tMin <= -10) ? 1 : 0;
				}
				curLine += ",9,0,-10,";        //0,0,0,";
				//
				delete charInput;
				charInput = new char[256];
				sprintf(charInput, "%i,%i,%i,", scor1, scor2, scor3);
				curLine += charInput;
				repLine.push_back(curLine);
			}
			else {
				//Do nothing...
				if (counter == 1) {
					repLine.push_back("END,");
				}
				counter = 0; //<- Disable
				repLine.push_back(curLine);
			}
		}
	}
	//Clean & Write...
	fstream out(oName.c_str(), ios::out);
	for (int i = 0; i < repLine.size(); i++) {
		temp = repLine[i];
		Tools::find_and_replace(temp, ",,", ",");
		out << temp << "\n";
	}
	out.close();
	std::cout << "Command '2' Completed..." << endl << endl;
}