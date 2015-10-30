#include "includes.h"
#include "tools.h"

#ifndef CREATETTT_H
#define CREATETTT_H

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
	//Open The File, Read in values month by month, but only do so for October 1st - April 30th
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
				temp = curLine.substr(0, curLine.find(","));
				Tools::splitMD(temp, &day, &month, &year);
				if (month == 10 && day == 1) {
					//Begin the season
					startS = year;
					inSeason = true;
				}
				if (inSeason) {
					if (day == 1) {
						//New Month... Calculate Averages and Write to data...
						for (int i = 0; i < monthlyHValues.size(); i++) {
							if (Tools::okCountThreshold("", "", "", monthlyHValues[i], monthlyLValues[i], automate)) {
								t1 += (monthlyHValues[i] <= 9 || monthlyLValues[i] <= 9) ? 1 : 0;
								t2 += (monthlyHValues[i] <= 0 || monthlyLValues[i] <= 0) ? 1 : 0;
								t3 += (monthlyHValues[i] <= -10 || monthlyLValues[i] <= -10) ? 1 : 0;
							}
						}
						sprintf(tempWrite, "%i/%i,%f,%f,%f,%f,%f,%f,%i,%i,%i,\n", pMonth, pYear, Tools::getAverage(monthlyHValues), Tools::getAverage(monthlyLValues),
							Tools::stdDeviation(monthlyHValues), Tools::stdDeviation(monthlyLValues), Tools::coefficientOfVarience(monthlyHValues),
							Tools::coefficientOfVarience(monthlyLValues), t1, t2, t3);
						writeMonthly.push_back(tempWrite);
						//Clear old data...
						monthlyHValues.clear();
						monthlyLValues.clear();
						t1 = 0;
						t2 = 0;
						t3 = 0;
					}

					//Capture the T. Values
					tPos = curLine.find(",");
					tPos = curLine.find(",", tPos + 1);
					tPos = curLine.find(",", tPos + 1);
					tPos = curLine.find(",", tPos + 1);
					tPos2 = curLine.find(",", tPos + 1);
					temp = curLine.substr(tPos + 1, tPos2 - (tPos + 1)).c_str();
					if (temp.compare("=NA()") == 0) {
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
					if (temp2.compare("=NA()") == 0) {
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
				}
				//End Of X Capture
				if (month == 4 && day == 30) {
					//End the season
					inSeason = false;
					for (int i = 0; i < seasonalHValues.size(); i++) {
						if (Tools::okCountThreshold("", "", "", seasonalHValues[i], seasonalLValues[i], automate)) {
							t1 += (seasonalHValues[i] <= 9 || seasonalLValues[i] <= 9) ? 1 : 0;
							t2 += (seasonalHValues[i] <= 0 || seasonalLValues[i] <= 0) ? 1 : 0;
							t3 += (seasonalHValues[i] <= -10 || seasonalLValues[i] <= -10) ? 1 : 0;
						}
					}
					sprintf(tempWrite, "Winter %i/%i,%f,%f,%f,%f,%f,%f,%i,%i,%i,\n", startS, year, Tools::getAverage(seasonalHValues), Tools::getAverage(seasonalLValues),
						Tools::stdDeviation(seasonalHValues), Tools::stdDeviation(seasonalLValues), Tools::coefficientOfVarience(seasonalHValues),
						Tools::coefficientOfVarience(seasonalLValues), t1, t2, t3);
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

#endif