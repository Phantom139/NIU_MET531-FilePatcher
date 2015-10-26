/*
 MET 531 Patcher
 By Robert C. Fritzen
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

// --- Constants
const int dim[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const char *mon[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

// --- Asset Functions

//find_and_replace(): String support tool to replace all instances of a string with another
void find_and_replace(string& source, string const& find, string const& replace) {
	for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;) {
		source.replace(i, find.length(), replace);
		i += replace.length();
	}
}

//getAverage(): Calculate Average
double getAverage(vector<int> nums) {
	int deduction = 0;
	double sum = 0;
	for (int i = 0; i < nums.size(); i++) {
		if (nums[i] == 999) {
			deduction++;
		}
		else {
			sum += (double)nums[i];
		}
	}
	return sum / (double)(nums.size() - deduction);
}

//stdDeviation(): Calculate Standard Deviation
double stdDeviation(vector<int> nums) {
	int deduction = 0; 
	double average = 0, innerClc = 0, OneOverNMinus1 = 0;
	average = getAverage(nums);
	for (int i = 0; i < nums.size(); i++) {
		if (nums[i] == 999) {
			deduction++;
		}
		else {
			innerClc += powf((double)nums[i] - average, 2.0);
		}
	}
	OneOverNMinus1 = (double)(1) / ((nums.size() - deduction) - 1);
	return sqrt(OneOverNMinus1 * innerClc);
}

//coefficientOfVarience(): Calculate CoV
double coefficientOfVarience(vector<int> nums) {
	double ave, stdDev;
	ave = getAverage(nums);
	stdDev = stdDeviation(nums);
	return stdDev / ave;
}

//okCountThreshold(): Quick string to number test to make sure the data isn't counting a #N/A# as a zero.
bool okCountThreshold(string fullInput, string strMax, string strMin, int maxTemp, int minTemp, bool automate = false) {
	if (fullInput.compare("") != 0) {
		find_and_replace(strMax, "#", "");
		find_and_replace(strMax, "/", "");
		find_and_replace(strMin, "#", "");
		find_and_replace(strMin, "/", "");
		if (strMax.compare("NA") == 0 || strMax.compare("") == 0) {
			std::cout << "Data Point: " << fullInput << " Has Been Filtered as Bad TMax(N/A, '')" << endl;
			return false;
		}
		if (strMin.compare("NA") == 0 || strMin.compare("") == 0) {
			std::cout << "Data Point: " << fullInput << " Has Been Filtered as Bad TMin(N/A, '')" << endl;
			return false;
		}
	}
	//Silly case, dead giveaway of bad data...
	if (minTemp > maxTemp) {
		std::cout << "Data Point: " << fullInput << " Has Been Filtered as Bad TMin > TMax" << endl;
		return false;
	}
	//Numerical Throw out. If min == 0 and the difference between min & max is > 30 (Exceptionally unlikely case), throw it out.
	if (minTemp == 0 && (maxTemp) > 30) {
		std::cout << "Data Point: " << fullInput << " thrown out based on 30 point difference tMin / tMax." << endl;
		return false;
	}
	//User Decision...
	if (minTemp == 0 || maxTemp == 0) {
		if (!automate) {
			if (fullInput.compare("") != 0) {
				std::cout << "Potential bad data point flagged: " << fullInput << " due to TMax (" << maxTemp << ") or TMin (" << minTemp << ") equalling zero." << endl;
			}
			else {
				std::cout << "Potential bad data-pair flagged: " << minTemp << ", " << maxTemp << endl;
			}
			std::cout << "Would you like to include this point? (Y/N): ";
			char *charInput = new char[2];
			cin.getline(charInput, 2);
			while (strcmp(charInput, "Y") != 0 && strcmp(charInput, "N") != 0) {
				delete charInput;
				std::cout << endl << "Invalid... (Y/N)?: ";
				charInput = new char[2];
				cin.getline(charInput, 2);
			}
			if (strcmp(charInput, "N") == 0) {
				std::cout << "Data flagged as bad, it will not be used in threshold calculation." << endl;
				return false;
			}
			else {
				std::cout << "Data flagged as good, it will be used in threshold calculation." << endl;
			}
		}
		else {
			std::cout << "Potential Bad Data... ";
			if (maxTemp < 20 && minTemp < 20) {
				std::cout << "Passes automation test, included" << endl;
			}
			else {
				std::cout << "Failed automation test, removed" << endl;
				return false;
			}
		}
	}
	return true;
}

//splitMD(): Splits a string input of MM/DD/YYYY or DD/MM/YYYY into it's correct integer components
void splitMD(string input, int *day, int *mo, int *yr) {
	string orig, temp, temp2;
	orig = input.substr(0, input.find(","));
	if (orig.find("/") != string::npos) {
		//MM/DD/YYYY Format
		temp = orig.substr(0, orig.find("/"));
		*mo = atoi(temp.c_str());
		temp = orig.substr(orig.find("/") + 1, orig.length());
		temp2 = temp.substr(0, temp.find("/"));
		*day = atoi(temp2.c_str());
		temp = temp.substr(temp.find("/") + 1, temp.length());
		if (*yr != -1) {
			*yr = atoi(temp.c_str());
		}
	}
	else {
		//XX-YY-ZZ Format
		temp = orig.substr(0, orig.find("-"));
		//Check if YYYY is first...
		if (temp.length() > 2) {
			//Are we catching the year value?
			if (*yr != -1) {
				*yr = atoi(temp.c_str());
			}
			temp2 = temp.substr(temp.find("-") + 1, temp.length());
			temp = temp2;
		}
		*mo = atoi(temp.c_str());
		temp2 = temp2.substr(temp2.find("-") + 1, temp2.length());
		temp = temp2.substr(0, temp2.find("-"));
		*day = atoi(temp.c_str());
	}
}

//acptlVlue(): Test character input to ensure it's a valid input command
bool acptlVlue(char *v) {
	if (strcmp(v, "1") == 0 ||
		strcmp(v, "2") == 0 ||
		strcmp(v, "3") == 0 ||
		strcmp(v, "4") == 0 || 
	    strcmp(v, "5") == 0 ) {
		return true;
	}
	return false;
}

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
			std::cout << "Found 'M': " << curLine.c_str() << endl;
			counter++;
		}
		find_and_replace(curLine, ",M", ",#N/A#");
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
			if (!okCountThreshold(repLine[i], temp, temp2, max, min, true)) {
				std::cout << "Bad data point thrown..." << endl;
				counter++;
				temp = "#N/A#";
				temp2 = "#N/A#";
			}
			else {
				sprintf(cTmp, "%i", max);
				temp = cTmp;
				sprintf(cTmp, "%i", min);
				temp2 = cTmp;
			}
			temp3 = repLine[i].substr(0, tPos3) + "," + temp + "," + temp2 + ",";
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
			find_and_replace(temp, ",", "");
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
				find_and_replace(temp, "#", "");
				find_and_replace(temp, "/", "");
				if (temp.compare("NA") == 0) {
					tMax = 999;
				}
				else {
					tMax = atoi(temp.c_str());
				}
				tPos = curLine.find(",", tPos2 + 1);
				temp2 = curLine.substr(tPos2 + 1, tPos - (tPos2 + 1)).c_str();
				find_and_replace(temp2, "#", "");
				find_and_replace(temp2, "/", "");
				if (temp2.compare("NA") == 0) {
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
				if (okCountThreshold(curLine, temp, temp2, tMax, tMin, automate)) {
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
		find_and_replace(temp, ",,", ",");
		out << temp << "\n";
	}
	out.close();
	std::cout << "Command '2' Completed..." << endl << endl;
}

//createThresholdTable(): Command "3": Create a threshold table file using the scores generated by "2".
void createThresholdTable(string &curLine, fstream *in, string oName) {
	//Define Specific Parameters
	bool read = false, okOpen = false;
	size_t tPos, tPos2;
	string temp, temp2;
	int month, day, index, score, yr = -1;
	int dailyScore1[366] = { 0 };
	int dailyScore2[366] = { 0 };
	int dailyScore3[366] = { 0 };
	int totalScore[366] = { 0 };
	vector<string> repLine;
	//end
	while (!in->eof()) {
		getline(*in, curLine);
		//std::cout << curLine.c_str() << endl;
		if (!okOpen) {
			//Check first line for "PATCHED"
			if (strcmp(curLine.c_str(), "PATCHED") != 0) {
				std::cout << "Cannot perform post-fix on unpatched file, break." << endl;
				break;
			}
			else {
				std::cout << "Patched file detected, proceeding..." << endl;
				okOpen = true;
			}
		}
		repLine.push_back(curLine);
	}
	if (okOpen) {
		//Using the repLine grouping, sort by each individual day.
		for (int i = 0; i < repLine.size(); i++) {
			curLine = repLine[i];
			if (strcmp(curLine.c_str(), "END,") == 0) {
				std::cout << "End Point Found..." << endl;
				read = false;
			}
			//If we need to read the line, then do it.
			if (read) {
				index = 0;
				splitMD(curLine, &day, &month, &yr);
				for (int y = 0; y < month - 1; y++) {
					index += dim[y];
				}
				index += day - 1;
				totalScore[index] += 1;
				//Find the scores for this line...
				tPos = curLine.find(",");
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos2 = curLine.find(",", tPos + 1);
				//Score 1
				score = atoi(curLine.substr(tPos + 1, tPos2 - (tPos + 1)).c_str());
				dailyScore1[index] += score;
				//Score 2
				tPos = curLine.find(",", tPos2 + 1);
				score = atoi(curLine.substr(tPos2 + 1, tPos - (tPos2 + 1)).c_str());
				dailyScore2[index] += score;
				//Score 3
				tPos2 = curLine.find(",", tPos + 1);
				score = atoi(curLine.substr(tPos + 1, tPos2 - (tPos + 1)).c_str());
				dailyScore3[index] += score;
			}
			//
			if (strcmp(curLine.c_str(), "START,") == 0) {
				std::cout << "Starting Point Found..." << endl;
				read = true;
			}
		}
		std::cout << "Opening file: " << oName.c_str() << endl;
		fstream out(oName.c_str(), ios::out);
		out << "Threshold Analysis,\nDay,Total Score (9F),Total Score (0F),Total Score (-10F),Maximum Score,Probability (9F),Probability (0F),Probability (-10F),\n";
		for (int i = 0; i < 12; i++) {
			for (int k = 0; k < dim[i]; k++) {
				index = 0;
				for (int l = 0; l < i; l++) {
					index += dim[l];
				}
				index += k;
				out << (i + 1 < 10 ? "0" : "") << i + 1 << "/" << k + 1 << " (" << index << ")," << dailyScore1[index] << "," << dailyScore2[index]
					<< "," << dailyScore3[index] << "," << totalScore[index] << "," << ((double)dailyScore1[index] / (double)totalScore[index]) * 100
					<< "," << ((double)dailyScore2[index] / (double)totalScore[index]) * 100 << ","
					<< ((double)dailyScore3[index] / (double)totalScore[index]) * 100 << "\n";
			}
		}
		out.close();
	}
	else {
		std::cout << "Failed Post-Analysis" << endl;
	}
	std::cout << "Command '3' Completed..." << endl << endl;
}

//createTimeSeriesTables(): Command "4": Create time series tables using the data. 
void createTimeSeriesTables(string &curLine, fstream *in, string oName, bool automate) {
	//Define Specific Parameters
	bool okOpen = false, reading = false, inSeason = false;
	int month, day, year, pMonth, pYear, t1, t2, t3, startS;
	string temp, temp2;
	size_t tPos, tPos2;
	vector<string> repLine, writeMonthly, writeSeasonally;
	vector<int> monthlyHValues, monthlyLValues, seasonalHValues, seasonalLValues;
	char tempWrite[512];
	//Prepare vectors.
	monthlyHValues.reserve(31);
	monthlyLValues.reserve(31);
	seasonalHValues.reserve(183);
	seasonalLValues.reserve(183);
	writeMonthly.push_back("Month & Year,Avg. High (F),Avg. Low (F),Std. Dev. High,Std. Dev. Low,CoV High,CoV Low,9F Crossed,0F Crossed,-10F Crossed,\n");
	writeSeasonally.push_back("Season,Avg. High (F),Avg. Low (F),Std. Dev. High,Std. Dev. Low,CoV High,CoV Low,9F Crossed,0F Crossed,-10F Crossed,\n");
	//Open The File, Read in values month by month, but only do so for October 30th - April 30th
	while (!in->eof()) {
		getline(*in, curLine);
		if (!okOpen) {
			//Check first line for "PATCHED"
			if (strcmp(curLine.c_str(), "PATCHED") != 0) {
				std::cout << "Cannot perform post-fix on unpatched file, break." << endl;
				break;
			}
			else {
				std::cout << "Patched file detected, proceeding..." << endl;
				okOpen = true;
			}
		}
		repLine.push_back(curLine);
	}
	if (okOpen) {
		for (int i = 0; i < repLine.size(); i++) {
			curLine = repLine[i];
			if (strcmp(curLine.c_str(), "END,") == 0) {
				std::cout << "End Point Found..." << endl;
				reading = false;
			}
			//
			if (reading) {
				splitMD(curLine, &day, &month, &year);
				if (day == 1) {
					//New Month... Calculate Averages and Write to data...
					for (int i = 0; i < monthlyHValues.size(); i++) {
						if (okCountThreshold("", "", "", monthlyHValues[i], monthlyLValues[i], automate)) {
							t1 += (monthlyHValues[i] <= 9 || monthlyLValues[i] <= 9) ? 1 : 0;
							t2 += (monthlyHValues[i] <= 0 || monthlyLValues[i] <= 0) ? 1 : 0;
							t3 += (monthlyHValues[i] <= -10 || monthlyLValues[i] <= -10) ? 1 : 0;
						}
					}
					sprintf(tempWrite, "%i/%i,%f,%f,%f,%f,%f,%f,%i,%i,%i,\n", pMonth, pYear, getAverage(monthlyHValues), getAverage(monthlyLValues),
						stdDeviation(monthlyHValues), stdDeviation(monthlyLValues), coefficientOfVarience(monthlyHValues),
						coefficientOfVarience(monthlyLValues), t1, t2, t3);
					writeMonthly.push_back(tempWrite);
					//Clear old data...
					monthlyHValues.clear();
					monthlyLValues.clear();
					t1 = 0;
					t2 = 0;
					t3 = 0;
				}
				if (month == 10 && day == 30) {
					//Begin the season
					startS = year;
					inSeason = true;
				}
				//Capture the T. Values
				tPos = curLine.find(",");
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos = curLine.find(",", tPos + 1);
				tPos2 = curLine.find(",", tPos + 1);
				temp = curLine.substr(tPos + 1, tPos2 - (tPos + 1)).c_str();
				find_and_replace(temp, "#", "");
				find_and_replace(temp, "/", "");
				if (temp.compare("NA") == 0) {
					monthlyHValues.push_back(999);
					if (inSeason) {
						seasonalHValues.push_back(999);
					}
				}
				else {
					monthlyHValues.push_back(atoi(temp.c_str()));
					if (inSeason) {
						seasonalHValues.push_back(atoi(temp.c_str()));
					}
				}
				tPos = curLine.find(",", tPos2 + 1);
				temp2 = curLine.substr(tPos2 + 1, tPos - (tPos2 + 1)).c_str();
				find_and_replace(temp2, "#", "");
				find_and_replace(temp2, "/", "");
				if (temp2.compare("NA") == 0) {
					monthlyLValues.push_back(999);
					if (inSeason) {
						seasonalLValues.push_back(999);
					}
				}
				else {
					monthlyLValues.push_back(atoi(temp2.c_str()));
					if (inSeason) {
						seasonalLValues.push_back(atoi(temp2.c_str()));
					}
				}
				//End Of X Capture
				if (month == 4 && day == 30) {
					//End the season
					inSeason = false;
					for (int i = 0; i < seasonalHValues.size(); i++) {
						if (okCountThreshold("", "", "", seasonalHValues[i], seasonalLValues[i], automate)) {
							t1 += (seasonalHValues[i] <= 9 || seasonalLValues[i] <= 9) ? 1 : 0;
							t2 += (seasonalHValues[i] <= 0 || seasonalLValues[i] <= 0) ? 1 : 0;
							t3 += (seasonalHValues[i] <= -10 || seasonalLValues[i] <= -10) ? 1 : 0;
						}
					}
					sprintf(tempWrite, "Winter %i/%i,%f,%f,%f,%f,%f,%f,%i,%i,%i,\n", startS, year, getAverage(seasonalHValues), getAverage(seasonalLValues),
						stdDeviation(seasonalHValues), stdDeviation(seasonalLValues), coefficientOfVarience(seasonalHValues),
						coefficientOfVarience(seasonalLValues), t1, t2, t3);
					writeSeasonally.push_back(tempWrite);
					//Clear old data...
					seasonalHValues.clear();
					seasonalLValues.clear();
					t1 = 0;
					t2 = 0;
					t3 = 0;
				}
				//Previous Values...
				pMonth = month;
				pYear = year;
			}
			//
			if (strcmp(curLine.c_str(), "START,") == 0) {
				std::cout << "Starting Point Found..." << endl;
				reading = true;
			}
		}
		//Write the output file...
		std::cout << "Opening file: " << oName.c_str() << endl;
		fstream out(oName.c_str(), ios::out);
		out << "Time Series Analysis,\n\nMonthly Analysis,\n";
		for (int i = 0; i < writeMonthly.size(); i++) {
			out << writeMonthly[i];
		}
		out << "\nSeasonal Analysis.\n";
		for (int i = 0; i < writeSeasonally.size(); i++) {
			out << writeSeasonally[i];
		}
		out.close();
	}
	else {
		std::cout << "Cannot perform this operation, file is unpatched..." << endl;
	}
	std::cout << "Command '4' Completed..." << endl << endl;
}

int main() {
	char *charInput = new char[256];
	strcpy(charInput, "FILLFILLFILL");
	int counter = 0, dotPos = 0;
	string curLine, temp, temp2;
	vector<string> fName;
	vector<string> oName;
	vector<string> repLine;

	do {
		if (strcmp(charInput, "") != 0) {
			delete charInput;
		}

		oName.clear();
		fName.clear();
		repLine.clear();

		charInput = new char[256];
		std::cout << "MET 531 CSV File Patcher" << endl << "By Robert F." << endl;
		std::cout << "Version a0.26" << endl << endl;
		std::cout << "Please place the CSV file you wish to open in the same folder as this .exe" << endl;
		std::cout << "Input CSV File Name (Include the .csv): ";
		cin.getline(charInput, 256);
		temp = charInput;
		fName.push_back(temp);
		delete charInput;

		find_and_replace(temp, "_cleaned", "");
		find_and_replace(temp, "_scored", "");
		find_and_replace(temp, "_tanalysis", "");
		find_and_replace(temp, "_timesrs", "");

		fName.push_back(temp);
		temp = "";

		dotPos = fName[1].find(".");
		temp = fName[1].substr(0, dotPos) + "_cleaned" + fName[1].substr(dotPos, fName[1].length());
		oName.push_back(temp);
		temp = fName[1].substr(0, dotPos) + "_scored" + fName[1].substr(dotPos, fName[1].length());
		oName.push_back(temp);
		temp = fName[1].substr(0, dotPos) + "_tanalysis" + fName[1].substr(dotPos, fName[1].length());
		oName.push_back(temp);
		temp = fName[1].substr(0, dotPos) + "_timesrs" + fName[1].substr(dotPos, fName[1].length());
		oName.push_back(temp);
		std::cout << endl << "Got " << fName[0] << endl << "Correct (Y/N)?: ";

		charInput = new char[256];
		cin.getline(charInput, 256);
		while (strcmp(charInput, "Y") != 0 && strcmp(charInput, "N") != 0) {
			delete charInput;
			std::cout << endl << "Invalid... (Y/N)?: ";
			charInput = new char[256];
			cin.getline(charInput, 256);
		}
	} while (strcmp(charInput, "N") == 0);

	std::cout << "\nAccepted File...\n";

	do {
		if (strcmp(charInput, "") != 0) {
			delete charInput;
			charInput = new char[256];			
		}
		std::cout << endl;
		std::cout << "Select Mode:\n1: Clean Missing Data (Delete 'M')\n2: Append Thresholds to Data, Calculate Scores\n3: Create Score Table" << 
			"\n4: Create Monthly & Yearly Average Tables (Time Series)\n\n5: Perform all Tasks" << endl;
		std::cout << "Selection: ";
		cin.getline(charInput, 256);
	} while (!acptlVlue(charInput));

	bool autom = false;
	char *autoInput = new char[2];
	if (strcmp(charInput, "2") == 0 || strcmp(charInput, "4") == 0 || strcmp(charInput, "5") == 0) {
		std::cout << endl << "Your selection has bad-data detection code embedded" << endl <<
			"Would you like to automate the user validation process (Temp = 0F)?" << endl <<
			"Automation will mark any values with a TDelta of 20 as bad. Continue? (Y/N) ";
		if (strcmp(autoInput, "") != 0) {
			delete autoInput;
		}
		autoInput = new char[2];
		cin.getline(autoInput, 2);
		while (strcmp(autoInput, "Y") != 0 && strcmp(autoInput, "N") != 0) {
			delete autoInput;
			std::cout << endl << "Invalid... (Y/N)?: ";
			autoInput = new char[2];
			cin.getline(autoInput, 2);
		}
		if (strcmp(autoInput, "Y") == 0) {
			autom = true;
		}
	}
	std::cout << endl;

	fstream *in = new fstream(fName[0], ios::in);

	//Modes:
	//1: Clean File
	if (strcmp(charInput, "1") == 0) {
		cleanFile(curLine, counter, in, oName[0]);
	}
	//2: Append Thresholds to Data
	else if (strcmp(charInput, "2") == 0) {
		//Define Specific Parameters
		calculateScores(curLine, charInput, counter, in, oName[1], autom);
	}
	//3: Post-Fix Sorting & Analysis [Generates Second File]
	else if (strcmp(charInput, "3") == 0) {
		createThresholdTable(curLine, in, oName[2]);
	}
	//4: Create Time Series Tables
	else if (strcmp(charInput, "4") == 0) {
		createTimeSeriesTables(curLine, in, oName[3], autom);
	}
	//5: Perform all tasks
	else if (strcmp(charInput, "5") == 0) {
		cleanFile(curLine, counter, in, oName[0]);
		fstream *in2 = new fstream(oName[0], ios::in);
		calculateScores(curLine, charInput, counter, in2, oName[1], autom);
		in2->close();
		delete in2;
		fstream *in3 = new fstream(oName[1], ios::in);
		createThresholdTable(curLine, in3, oName[2]);
		in3->close();
		delete in3;
		fstream *in4 = new fstream(oName[1], ios::in);
		createTimeSeriesTables(curLine, in4, oName[3], autom);
		in4->close();
		delete in4;
	}
	in->close();
	delete in;

	std::cout << "Program Done." << endl;
	cin.get();

	return 0;
}