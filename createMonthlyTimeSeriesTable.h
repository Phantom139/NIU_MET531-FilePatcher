#include "includes.h"
#include "tools.h"

#ifndef CREATEMTST_H
#define CREATEMTST_H

//createMonthlyTimeSeriesTables(): Command "5": Create monthly time series tables using the data. 
void createMonthlyTimeSeriesTables(string &curLine, fstream *in, string oName, bool automate) {
	//Define Specific Parameters
	bool okOpen = false, reading = false, inSeason = false, caughtInitialValues = false;
	int month, day, year, pMonth, pYear, t1, t2, t3, startMonth, startYear;
	string temp, temp2;
	size_t tPos, tPos2;
	vector<string> repLine, write;
	vector<int> monthlyHValues, monthlyLValues, thresholdCount1, thresholdCount2, thresholdCount3, totals1, totals2, totals3;
	char tempWrite[512];
	//Prepare vectors.
	monthlyHValues.reserve(31);
	monthlyLValues.reserve(31);
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
					if (!caughtInitialValues) {
						startYear = year;
						startMonth = month;
						caughtInitialValues = true;
					}
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
						thresholdCount1.push_back(t1);
						thresholdCount2.push_back(t2);
						thresholdCount3.push_back(t3);
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
					}
					else {
						monthlyHValues.push_back(atoi(temp.c_str()));
					}
					tPos = curLine.find(",", tPos2 + 1);
					temp2 = curLine.substr(tPos2 + 1, tPos - (tPos2 + 1)).c_str();
					if (temp2.compare("=NA()") == 0) {
						monthlyLValues.push_back(999);
					}
					else {
						monthlyLValues.push_back(atoi(temp2.c_str()));
					}
				}
				//End Of X Capture
				if (month == 4 && day == 30) {
					//End the season
					inSeason = false;
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
		//Clean First...
		for (int i = 0; i < thresholdCount1.size(); i++) {
			if (thresholdCount1[i] < 0 || thresholdCount1[i] > 31) {
				thresholdCount1[i] = 0;
			}
			if (thresholdCount2[i] < 0 || thresholdCount2[i] > 31) {
				thresholdCount2[i] = 0;
			}
			if (thresholdCount3[i] < 0 || thresholdCount3[i] > 31) {
				thresholdCount3[i] = 0;
			}
		}
		int thresholds[3] = { 9, 0, -10 };
		int tTotal = 0;
		int seasonalCounter = 0;
		int yearCounter = 0;
		for (int i = 0; i < 3; i++) {
			tTotal = 0;
			yearCounter = 0;
			seasonalCounter = 0;
			sprintf(tempWrite, "%i", thresholds[i]);
			temp = "Monthly Amount of Times for " + string(tempWrite) + "F Threshold,\nYear,Oct.,Nov.,Dec.,Jan.,Feb.,Mar.,Apr.,Total,\n";
			write.push_back(temp);
			//Loop
			switch (i) {
			case 0:
				for (int k = 0; k < thresholdCount1.size(); k++) {
					if (seasonalCounter == 0) {
						sprintf(tempWrite, "%i,", startYear + yearCounter);
						temp = tempWrite;
					}
					tTotal += thresholdCount1[k];
					sprintf(tempWrite, "%i,", thresholdCount1[k]);
					temp += tempWrite;
					seasonalCounter++;
					if (seasonalCounter == 7) {
						seasonalCounter = 0;
						yearCounter++;
						sprintf(tempWrite, "%i,", tTotal);
						temp += string(tempWrite) + ",\n";
						totals1.push_back(tTotal);
						tTotal = 0;
						write.push_back(temp);
					}
				}
				break;
			case 1:
				for (int k = 0; k < thresholdCount2.size(); k++) {
					if (seasonalCounter == 0) {
						sprintf(tempWrite, "%i,", startYear + yearCounter);
						temp = tempWrite;
					}
					tTotal += thresholdCount2[k];
					sprintf(tempWrite, "%i,", thresholdCount2[k]);
					temp += tempWrite;
					seasonalCounter++;
					if (seasonalCounter == 7) {
						seasonalCounter = 0;
						yearCounter++;
						sprintf(tempWrite, "%i,", tTotal);
						temp += string(tempWrite) + ",\n";
						totals2.push_back(tTotal);
						tTotal = 0;
						write.push_back(temp);
					}
				}
				break;
			case 2:
				for (int k = 0; k < thresholdCount3.size(); k++) {
					if (seasonalCounter == 0) {
						sprintf(tempWrite, "%i,", startYear + yearCounter);
						temp = tempWrite;
					}
					tTotal += thresholdCount3[k];
					sprintf(tempWrite, "%i,", thresholdCount3[k]);
					temp += tempWrite;
					seasonalCounter++;
					if (seasonalCounter == 7) {
						seasonalCounter = 0;
						yearCounter++;
						sprintf(tempWrite, "%i,", tTotal);
						temp += string(tempWrite) + ",\n";
						totals3.push_back(tTotal);
						tTotal = 0;
						write.push_back(temp);
					}
				}
				break;
			}
			write.push_back("\n\n");
		}
		//Final Calculations
		write.push_back("Final Calculations,\nCalculations for 9F:,\n");
		sprintf(tempWrite, "Total: ,%i,\n", Tools::sumIt(totals1));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "Average: ,%f,\n", Tools::getAverage(totals1));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "Std. Deviation: ,%f,\n", Tools::stdDeviation(totals1));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "CoV: ,%f,\n", Tools::coefficientOfVarience(totals1));
		write.push_back(string(tempWrite));
		write.push_back("Calculations for 0F:,\n");
		sprintf(tempWrite, "Total: ,%i,\n", Tools::sumIt(totals2));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "Average: ,%f,\n", Tools::getAverage(totals2));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "Std. Deviation: ,%f,\n", Tools::stdDeviation(totals2));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "CoV: ,%f,\n", Tools::coefficientOfVarience(totals2));
		write.push_back(string(tempWrite));
		write.push_back("Calculations for -10F:,\n");
		sprintf(tempWrite, "Total: ,%i,\n", Tools::sumIt(totals3));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "Average: ,%f,\n", Tools::getAverage(totals3));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "Std. Deviation: ,%f,\n", Tools::stdDeviation(totals3));
		write.push_back(string(tempWrite));
		sprintf(tempWrite, "CoV: ,%f,\n", Tools::coefficientOfVarience(totals3));
		write.push_back(string(tempWrite));

		//Write the output file...
		std::cout << "Opening file: " << oName.c_str() << endl;
		fstream out(oName.c_str(), ios::out);
		for (int i = 0; i < write.size(); i++) {
			out << write[i];
		}
		out.close();
	}
	else {
		std::cout << "Cannot perform this operation, file is unpatched..." << endl;
	}
	std::cout << "Command '5' Completed..." << endl << endl;
}

#endif