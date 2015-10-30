/*
calculators.h

Provides all math methods used.
*/

#include <vector>
#include <math.h>

using namespace std;

#ifndef TOOLS_H
#define TOOLS_H

// --- Asset Functions
namespace Tools {
	//find_and_replace(): String support tool to replace all instances of a string with another
	void find_and_replace(string& source, string const& find, string const& replace) {
		for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;) {
			source.replace(i, find.length(), replace);
			i += replace.length();
		}
	}

	//sumIt(): Sum of vector<int>
	int sumIt(vector<int> nums) {
		int T = 0;
		for (int i = 0; i < nums.size(); i++) {
			T += nums[i];
		}
		return T;
	}

	//getAverage(): Calculate Average
	double getAverage(vector<int> nums) {
		int deduction = 0;
		double sum = (double)sumIt(nums);
		for (int i = 0; i < nums.size(); i++) {
			if (nums[i] == 999) {
				deduction++;
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
};

#endif