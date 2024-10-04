// 資訊二甲 11127121鄧俊揚 10926125簡紹軒

#define _CRT_SECURE_NO_WARNINGS  //in some compiler "strncpy"in line 115 will error. use this define can avoid.
#include <iostream>
#include <bitset>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

int strToi(string str) {
	int num = 0;
	for (int i = 0; i < str.length(); i++) {
		// turn string to integer, and pass no numbers characters.
		if (str[i] >= '0' && str[i] <= '9') {
			num = num * 10 + str[i] - '0';
		}
	}
	return num;
}

bool inputData(string fileNum, vector<vector<string> >& detail) {	// store input data to detail vector.
	detail.clear();
	fstream file;
	string fileName = "input" + fileNum + ".txt";
	file.open(fileName.c_str());
	if (!file.is_open()) /* Check whether the file is exists */ {
		cout << "\n### input" << fileNum << ".txt does not exist! ###" << endl;
		return false;
	}
	bool noData = true;
	string label;
	string info;                // the string from each input
	string infoData;
	vector<string> detailLine;	// store each infos of line, ex: sid, sname... etc.
	int dataNum = 0, i;
	string continueInfo;        // when data is no continue, use "continueInfo" to connect two string.

	while (getline(file, info)) /* Get the first label's data */ {
		noData = false;
		dataNum = 0;
		detailLine.clear();
		for (i = 0; i < info.length(); i++) {
			infoData = "";
			while (info[i] != '\t' && info[i] != '\n' && i < info.length()) /* Get the data of each labels */ {
				if (info[i] != ',' && info[i] != '\"')
					infoData += info[i];
				i++;
			}

			detailLine.push_back(infoData);
		}
		detail.push_back(detailLine);
	}
	file.close(); // Close the file
	if (noData) {
		cout << "\n### Get nothing from the file input" << fileNum << ".txt ! ###" << endl;
		cout << "\nThere is no data!" << endl;
		return false;
	}
	return true;
}

bool fileExists(const std::string& path) {
	std::ifstream file(path);
	return file.good();
}

int findSmallestPrime(int num) {						// find the smallest prime of num*1.15
	// Corner case
	if (num <= 1) {
		cout << "Error in findPrime." << endl;
		exit(EXIT_FAILURE);
	}
	// find prime
	bool isPrime = true;
	int p = 0;
	for (num; ; num++) {
		isPrime = true;
		for (int i = 2; i <= num / 2; i++) {
			if (num % i == 0) {
				isPrime = false;
			}
		}
		if (isPrime) {
			p = num;
			break;
		}
	}
	return p;
}

//********************************************************* start Tobinary *************************************************
class Tobinary {
private:

public:
	void writeBinaryFile(vector<vector<string> >& input, string fileName) {
		std::ofstream outFile("input" + fileName + ".bin", std::ios::binary);

		if (!outFile) {														// if the file is failing to open
			std::cerr << "Cannot open input" + fileName + ".bin" << std::endl;
			return;
		}
		for (int i = 0; i < input.size(); i++) {
			for (int j = 0; j < input[i].size(); j++) {
				if (j < 2) {													// sid and sname (cast to char array)
					char* buffer = new char[10];
					for (int k = 0; k < 10; k++) buffer[k] = '\0';				// initialize buffer

					strncpy(buffer, input[i][j].c_str(), 10);	// store in char array of legnth 10
					outFile.write(buffer, 10);		   						    // write into bin file
					delete[] buffer;											// release buffer memory space
				}
				else if (j >= 2 && j <= 7) {									// 6 scores (cast to unsigned char)
					unsigned char score = (unsigned char)strToi(input[i][j]);		// casting string -> integer -> unsigned char
					outFile.write(reinterpret_cast<const char*>(&score), sizeof(unsigned char));
				}
				else {															// average score (cast to float)
					float averageScore = stof(input[i][j]);						// casting string -> float
					outFile.put(0x00);
					outFile.put(0x00);
					outFile.write(reinterpret_cast<const char*>(&averageScore), sizeof(float));
				}
			}
		}

		outFile.close();
	}
};
//********************************************************* end Tobinary ***************************************************

//********************************************************* start HashQtable ***************************************************
class HashTable {
private:
	struct student {
		char sid[10];
		char sname[10];
		unsigned char score[6];
		float mean;
		bool empty() {
			return (sid[0] == '\0' && sname[0] == '\0' && score[0] == '\0');
		}
	};
	vector<student> Qtable;            // quadratric hash table
	vector<student> Dtable;            // double hash table
	vector<student> initdata;          // the data want to hash
	int unsuccessQ = 0;                // the number to record unsuccess search on Quadratric hash
	int successD = 0;                  // the number to record success search on Double hash

public:
	void clear() {
		Qtable.clear();
		Dtable.clear();
		initdata.clear();
		unsuccessQ = 0;
		successD = 0;
	}

	int getNumberOfDataInTable() /*not necessary*/ {
		int num = 0;
		for (student v : Dtable) {
			if (!v.empty()) {
				num++;
			}
		}
		return num;
	}

	bool isFull() {											// check if the Qtable is full
		bool isFull = true;
		for (student v : Qtable) {
			if (v.empty()) {
				isFull = false;
				break;
			}
		}

		return isFull;
	}

	void resizeTable(char isQHash) {									// increase Qtable size to minimum prime number >= 1.15 times data numbers
		int size = initdata.size();                         // the input file student number
		int newSize = findSmallestPrime((int)ceil(1.15 * size));
		if (isQHash == 'Q')Qtable.resize(newSize);
		else Dtable.resize(newSize);
	}

	int hashFuncion(string key) {							// calculate hash value of the key (sid)
		int hashValue = 1;
		for (char c : key) {
			hashValue *= c;							       // times ASCII Code of each char of sid
			// because the hashValue will exceed range of int-type, get reminder and multiplicate reminder can keep it in range. 
			hashValue = hashValue % Qtable.size();		    // then module table size
		}

		return hashValue;
	}

	int maxStepFunction() {
		int size = initdata.size();                         // size is not the number in table, is the student number in input data.
		return findSmallestPrime((int)ceil((float)size / 5));
	}

	int stepFunction(int hashValue) {						// calculate step value of the hash value of the key (sid)
		int maxStep = maxStepFunction();                    // separate stepFunction and maxStapFunction
		int stepValue = maxStep - (hashValue % maxStep);
		return stepValue;
	}

	int quadraticProbing(int index) {						// resolve collision by quadratic probing
		int newIndex = 0;
		for (int i = 1; ; i++) {
			newIndex = (index + i * i) % Qtable.size();		// calculate new index (may cause infinite loop, have to resolve)
			unsuccessQ++;                                   // increase unsuccessQ to get unsuccess search average
			if (Qtable[newIndex].empty()) {					// if new index is empty
				break;
			}
		}

		return newIndex;
	}

	int getQSuccess() {
		// to count success search in quadratric need how much time.
		int collision = 0;
		int success = 0;
		for (int j = 0; j < Qtable.size(); j++) {
			collision = 0;
			while (!Qtable[(j + collision * collision) % Qtable.size()].empty()) {
				collision += 1;
				success++;
			}
		}
		return success;
	}

	void tableFileOutput(string fileName, char isQHash, vector<student> table) {
		// output a file when hash table establish
		std::ofstream outFile(fileName.c_str());
		if (isQHash == 'Q') {
			outFile << " --- Hash table created by Quadratic probing ---\n";
		}
		else {
			outFile << " --- Hash table created by Double probing ---\n";
		}
		for (int i = 0; i < table.size(); i++) {
			outFile << "[" << std::setw(3) << std::right << i << "]";     // setw is use to let file clean 
			if (!table[i].empty()) {
				outFile << std::setw(11) << std::right << hashFuncion(table[i].sid) << ",";
				outFile << std::setw(11) << std::right << table[i].sid << ",";
				outFile << std::setw(11) << std::right << table[i].sname << ",";
				outFile << std::setw(11) << std::right << table[i].mean;
			}
			outFile << endl;
		}
		outFile << "-----------------------------------------------------";
		outFile.close();
	}

	void printComparison() {
		cout << "successful search : " << setprecision(5) << float(successD) / float(initdata.size()) << " comparisons on average\n\n";
	}

	void bulidQHash(string fileName) {
		// call by main to do mission 1
		string searchKey;
		resizeTable('Q');
		for (int i = 0; i < initdata.size(); i++) {
			insertQ(initdata[i]);
		}

		tableFileOutput("quadratic" + fileName + ".txt", 'Q', Qtable);
		cout << "\nHash table has been successfully created by Quadratic probing\n";
		cout << "unsuccessful search: " << setprecision(5) << float(getQSuccess()) / float(Qtable.size()) << " comparisons on average\n";
		cout << "successful search : " << setprecision(5) << float(unsuccessQ) / float(initdata.size()) << " comparisons on average\n\n";

		do {
			cout << "Input a student ID to search ([0] quit): ";
			cin >> searchKey;
			searchQHash(searchKey);
		} while (searchKey != "0");
	}

	int doubleHashing(int index, string key) {				  // resolve collision by double hashing
		int newIndex = index;
		int hashValue = 1;
		int maxStep = maxStepFunction();                       // need maxStep to help hashValue in int-type range

		for (char c : key) {
			hashValue *= (int)c;							   // times ASCII Code of each char of sid
			hashValue %= maxStep;                              // only reminder, the other part will out of int-type range 
		}

		int stepValue = stepFunction(hashValue);
		do {
			newIndex = (newIndex + stepValue) % Dtable.size();
			successD++;
		} while (!Dtable[newIndex].empty());				 	// find new index until index of the Qtable is empty

		return newIndex;
	}

	void insertQ(student std) {						// insert element into Qtable, resolve collision by quadratic probing
		int index = hashFuncion(std.sid);					// get index of std by std[0] (sid)
		unsuccessQ++;
		if (!Qtable[index].empty()) {						// if collision occurs
			index = quadraticProbing(index);				// calculate new index by quadratic probing
		}
		Qtable[index] = std;									// insert std into Qtable

		if (isFull()) resizeTable('Q');						// if the Qtable is full, resize the Qtable size

		return;
	}


	void buildDHash(string fileName) {
		// call by main to do mission 2
		string searchKey;
		resizeTable('D');
		for (int i = 0; i < initdata.size(); i++) {
			insertD(initdata[i]);
		}
		tableFileOutput("double" + fileName + ".txt", 'D', Dtable);
		printComparison();

		do {
			cout << "Input a student ID to search ([0] quit): ";
			cin >> searchKey;
			searchDHash(searchKey);
		} while (searchKey != "0");
	}

	void insertD(student std) {						// insert element into Qtable, resolve collision by double hashing
		int index = hashFuncion(std.sid);					// get index of std by std[0] (sid)
		successD++;
		if (!Dtable[index].empty()) {						// if collision occurs
			index = doubleHashing(index, std.sid);			// calculate new index by double hashing
		}
		Dtable[index] = std;									// insert std into Qtable


		if (isFull()) resizeTable('D');						// if the Qtable is full, resize the Qtable size

		return;
	}

	bool ReadBinaryFile(string fileName) {

		std::ifstream file(("input" + fileName + ".bin"), std::ios::binary);


		if (!file.is_open()) /* Check whether the file is exists */ {
			cout << "\n### input" << fileName << ".bin does not exist! ###" << endl;
			return false;
		}

		file.seekg(0, std::ios::end);             // move to file end
		std::streampos fileSize = file.tellg();   // set a point to record last in file
		file.seekg(0, std::ios::beg);             // move back to file start

		student temp;
		while (file.tellg() != fileSize) {
			file.read((char*)&temp, sizeof(student));
			initdata.push_back(temp);
		}

		if (!file.eof() && file.fail()) {
			std::cerr << "Error occurred at reading time!" << std::endl;
			return false;
		}

		file.close();
		return true;
	}




	void searchQHash(string str) {
		if (str == "0") {
			cout << "\n\n";
			return;
		}
		int key = hashFuncion(str);
		bool searchSuccess = false;
		int i = 0;
		for (i = 0; !Qtable[(key + i * i) % Qtable.size()].empty() && i < Qtable.size(); i++) {
			if (Qtable[(key + i * i) % Qtable.size()].sid == str) {
				cout << "\n{ " << Qtable[(key + i * i) % Qtable.size()].sid << ", " << Qtable[(key + i * i) % Qtable.size()].sname;
				cout << ", " << Qtable[(key + i * i) % Qtable.size()].mean << " } is found after " << i + 1 << " probes.";
				searchSuccess = true;
				break;
			}
		}

		if (searchSuccess == false && i < Qtable.size()) {
			cout << endl << str << " is not found after " << i + 1 << " probes.";
		}
		cout << "\n\n";
	}

	void searchDHash(string str) {
		if (str == "0") {
			cout << "\n\n";
			return;
		}
		int index = hashFuncion(str);
		int key = 1;
		int max = maxStepFunction();
		for (char c : str) {
			key = key * c;
			key = key % max;
		}
		int step = stepFunction(key);
		bool searchSuccess = false;
		int i = 0;
		for (i = 0; !Dtable[index].empty() && i < Dtable.size(); i++) {

			if (Dtable[index].sid == str) {
				cout << "\n{ " << Dtable[index].sid << ", " << Dtable[index].sname;
				cout << ", " << Dtable[index].mean << " } is found after " << i + 1 << " probes.";
				searchSuccess = true;
				break;
			}
			index = (index + step) % Dtable.size();
		}
		if (searchSuccess == false && i < Dtable.size()) {
			cout << endl << str << " is not found after " << i << " probes.";
		}
		cout << "\n\n";
	}
};
//********************************************************* end HashQtable ***************************************************


int main() {
	string command;
	string fileName;
	int lastCommand = 0;
	vector<vector<string> > totalData;
	Tobinary tobinary;
	HashTable hash;


	do {
		cout << "******* Hash Qtable *****\n";
		cout << "* 0. QUIT              *\n";
		cout << "* 1. Quadratic probing *\n";
		cout << "* 2. Double hashing    *\n";
		cout << "************************\n";
		cout << "Input a choice(0, 1, 2): ";
		cin >> command;

		if (command == "1") {
			do {
				cout << endl << "Input a file number ([0] Quit): ";
				cin >> fileName;
			} while (fileName != "0" && !inputData(fileName, totalData));

			if (fileName != "0") {
				hash.clear();
				bool fileExist = fileExists("input" + fileName + ".bin");						// check if the file exists
				if (!fileExist) {																// the file does not exist
					cout << "\n### input" + fileName + ".bin does not exist! ###\n";
					tobinary.writeBinaryFile(totalData, fileName);
					hash.ReadBinaryFile(fileName);
				}
				else {
					hash.ReadBinaryFile(fileName);
				}
				hash.bulidQHash(fileName);
				lastCommand = 1;
			}
			//lastCommand = 1;
		}
		else if (command == "2") {
			if (lastCommand == 1) {
				cout << "\nHash table has been successfully created by Double hashing\n";
				hash.buildDHash(fileName);
				lastCommand = 2;
			}
			else if (lastCommand == 2) {
				string searchKey;
				cout << "\nHash table has been successfully created by Double hashing\n";
				hash.printComparison();
				do {
					cout << "Input a student ID to search ([0] quit): ";
					cin >> searchKey;
					hash.searchDHash(searchKey);
				} while (searchKey != "0");
				lastCommand = 2;
			}
			else {
				cout << "### Choose 1 first. ###\n\n\n";
			}
			//lastCommand = 2;
		}
		else if (command > "2" || command < "0") {
			cout << "\nCommand does not exist!\n\n";
			//lastCommand = 0;
		}
	} while (command != "0");
	return 0;
}
