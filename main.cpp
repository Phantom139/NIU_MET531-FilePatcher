/*
 MET 531 Patcher
 By Robert C. Fritzen
*/

#include "includes.h"
#include "tools.h"
#include "commands.h"

//acptlVlue(): Test character input to ensure it's a valid input command
bool acptlVlue(char *v) {
	if (strcmp(v, "1") == 0 ||
		strcmp(v, "2") == 0 ||
		strcmp(v, "3") == 0 ||
		strcmp(v, "4") == 0 || 
	    strcmp(v, "5") == 0 ||
		strcmp(v, "6") == 0) {
		return true;
	}
	return false;
}

int main() {
	char *charInput = new char[256];
	strcpy(charInput, "FILLFILLFILL");
	int counter = 0, dotPos = 0;
	string curLine, temp, temp2;
	vector<string> fName;
	vector<string> oName;
	vector<string> repLine;

	std::cout << "MET 531 CSV File Patcher" << endl << "By Robert F." << endl;
	std::cout << "Version b1.05" << endl << endl;

	do {
		if (strcmp(charInput, "") != 0) {
			delete charInput;
		}

		oName.clear();
		fName.clear();
		repLine.clear();

		charInput = new char[256];
		std::cout << "Please place the CSV file you wish to open in the same folder as this .exe" << endl;
		std::cout << "Input CSV File Name (Include the .csv): ";
		cin.getline(charInput, 256);
		temp = charInput;
		fName.push_back(temp);
		delete charInput;

		Tools::find_and_replace(temp, "_cleaned", "");
		Tools::find_and_replace(temp, "_scored", "");
		Tools::find_and_replace(temp, "_tanalysis", "");
		Tools::find_and_replace(temp, "_timesrs", "");
		Tools::find_and_replace(temp, "_monthlytimesrs", "");

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
		temp = fName[1].substr(0, dotPos) + "_monthlytimesrs" + fName[1].substr(dotPos, fName[1].length());
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
			"\n4: Create Monthly & Seasonal Average Tables (Time Series)\n5: Create Monthly Average Tables (Time Series)\n\n6: Perform all Tasks" << endl;
		std::cout << "Selection: ";
		cin.getline(charInput, 256);
	} while (!acptlVlue(charInput));

	bool autom = false;
	char *autoInput = new char[2];
	if (strcmp(charInput, "2") == 0 || strcmp(charInput, "4") == 0 || strcmp(charInput, "5") == 0 || strcmp(charInput, "6") == 0) {
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
	//5: Create Overall Monthly Time Series Tables
	else if (strcmp(charInput, "5") == 0) {
		createMonthlyTimeSeriesTables(curLine, in, oName[4], autom);
	}
	//6: Perform all tasks
	else if (strcmp(charInput, "6") == 0) {
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
		fstream *in5 = new fstream(oName[1], ios::in);
		createMonthlyTimeSeriesTables(curLine, in5, oName[4], autom);
		in5->close();
		delete in5;
	}
	in->close();
	delete in;

	std::cout << "Program Done." << endl;
	cin.get();

	return 0;
}