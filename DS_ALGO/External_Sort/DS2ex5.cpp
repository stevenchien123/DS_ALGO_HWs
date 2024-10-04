// 資訊二甲 第8組 10926125簡紹軒 11127121鄧俊揚

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <cstdio>
#include <math.h>

using namespace std;

// *************************************** Start Sort ***************************************
class Sort {
private:
	struct data {
		char putID[10];
		char getID[10];
		float weight;
	};
	vector<data> initdata;

	struct Index {
		int offset;
		float weight;
	};
	vector<Index> primIndex;

public:
	int getRunsNum(string fileName, int bufSize) {
		ifstream file(("pairs" + fileName + ".bin"), ios::binary);
		if (fileName == "0") {
			cout << "\n" << endl;
			return -1;
		}
		if (!file.is_open()) {
			cout << "\n### pairs" << fileName << ".bin does not exist! ###\n" << endl;
			return -1;
		}
		file.seekg(0, ios::end);
		streampos fileSize = file.tellg();
		return fileSize / (bufSize * 24) + 1;
	}

	/*
	 * Each data set is 10+10+4 = 24 bytes.
	 * Read 24*300 = 7200 bytes each time. (buffer size is 300 data sets)
	 * @param fileName: the input filename
	 * @param i: how many times reading data (count from 0)
	 */
	bool ReadBinaryFile(const string& fileName, int bufSize, int i) {
		ifstream file(("pairs" + fileName + ".bin"), ios::binary);
		if (!file.is_open()) {
			cout << "\n### pairs" << fileName << ".bin does not exist! ###\n" << endl;
			return false;
		}
		int dataNum = 0;
		file.seekg(0, ios::end);
		streampos fileSize = file.tellg();
		file.seekg(i * bufSize * 24, ios::beg);                                                   // first time start from 0, second time start from bufSize*24 ..., and so on.
		bool noData = true;
		data temp;
		while (file.tellg() != fileSize && dataNum < bufSize) {
			file.read(reinterpret_cast<char*>(&temp), sizeof(data));
			initdata.push_back(temp);
			dataNum++;
			noData = false;
		}
		if (!file.eof() && file.fail()) {
			cerr << "Error occurred at reading time!" << endl;
			return false;
		}
		if (noData) {
			cout << "\n### Get nothing from the file input" << fileName << ".txt ! ###" << endl;
			cout << "\nThere is no data!" << endl;
			return false;
		}
		file.close();
		return true;
	}

	bool ReadOrderBinaryFile(const string& fileName) {
		ifstream file(("order" + fileName + ".bin"), ios::binary);
		if (fileName == "0") {
			cout << "\n" << endl;
			return false;
		}
		if (!file.is_open()) {
			cout << "\n### pairs" << fileName << ".bin does not exist! ###\n" << endl;
			return false;
		}
		file.seekg(0, ios::end);
		streampos fileSize = file.tellg();
		file.seekg(0, ios::beg);
		bool noData = true;
		data temp;
		cout << "ReadOrderBinaryFile:" << endl;
		cout << "fileSize: " << fileSize << endl;
		cout << "initdata size: " << initdata.size() << endl;
		while (file.tellg() != fileSize) {
			file.read(reinterpret_cast<char*>(&temp), sizeof(data));
			initdata.push_back(temp);
			noData = false;
		}
		if (!file.eof() && file.fail()) {
			cerr << "Error occurred at reading time!" << endl;
			return false;
		}
		if (noData) {
			cout << "\n### Get nothing from the file input" << fileName << ".txt ! ###" << endl;
			cout << "\nThere is no data!" << endl;
			return false;
		}
		file.close();
		return true;
	}

	vector<data> mergeVector(vector<data>& data1, vector<data>& data2) {
		vector<data> result1;
		int i = 0;
		int j = 0;
		// 比較兩個陣列的元素並合併
		while (i < data1.size() && j < data2.size()) {
			if (data1[i].weight > data2[j].weight) {
				result1.push_back(data1[i]);
				++i;
			}
			else {
				result1.push_back(data2[j]);
				++j;
			}
		}
		// 如果 arr1 還有剩餘元素，添加到結果陣列
		while (i < data1.size()) {
			result1.push_back(data1[i]);
			++i;
		}
		// 如果 arr2 還有剩餘元素，添加到結果陣列
		while (j < data2.size()) {
			result1.push_back(data2[j]);
			++j;
		}
		return result1;
	}

	void merge(string fileName, string result, int bufSize, int level) {
		std::ifstream file1(fileName + ".bin", ios::binary);
		std::ofstream outFile(result + ".bin", ios::binary);
		int dataNum = 0;
		bool isLast1 = true;
		streampos data1;
		streampos data2;
		streampos data1End;
		streampos data2End;
		data temp1;
		data temp2;
		file1.seekg(0, ios::end);
		streampos fileSize = file1.tellg();
		file1.seekg(0, ios::beg);
		streampos curAddr = file1.tellg();
		while (curAddr < fileSize) {
			data1 = curAddr;
			data2 = curAddr + streampos(level * 24 * bufSize);
			if (data2 < fileSize)data1End = data2;
			else data1End = fileSize;
			if (data2 + streampos(level * 24 * bufSize) < fileSize)data2End = data2 + streampos(level * 24 * bufSize);
			else data2End = fileSize;
			file1.seekg(data1);
			isLast1 = true;
			file1.read(reinterpret_cast<char*>(&temp1), sizeof(data));
			isLast1 = true;
			if (data2 < fileSize) {
				file1.seekg(data2);
				isLast1 = false;
				file1.read(reinterpret_cast<char*>(&temp2), sizeof(data));
				isLast1 = false;
			}

			while (data2 < data2End && data1 < data1End) {
				if (temp1.weight >= temp2.weight) {
					data1 = data1 + streampos(24);
					outFile.write(temp1.getID, 10);
					outFile.write(temp1.putID, 10);
					outFile.write(reinterpret_cast<const char*>(&temp1.weight), sizeof(float));
					if (!isLast1)file1.seekg(data1);
					isLast1 = true;
					file1.read(reinterpret_cast<char*>(&temp1), sizeof(data));
				}
				else {
					data2 = data2 + streampos(24);
					outFile.write(temp2.getID, 10);
					outFile.write(temp2.putID, 10);
					outFile.write(reinterpret_cast<const char*>(&temp2.weight), sizeof(float));
					if (data2 < fileSize) {
						if (isLast1)file1.seekg(data2);
						isLast1 = false;
						file1.read(reinterpret_cast<char*>(&temp2), sizeof(data));
					}
				}
			}
			file1.seekg(data1);
			while (data1 < data1End) {
				file1.read(reinterpret_cast<char*>(&temp1), sizeof(data));
				//if(level == 1)cout << temp1.weight << "\t";
				outFile.write(temp1.getID, 10);
				outFile.write(temp1.putID, 10);
				outFile.write(reinterpret_cast<const char*>(&temp1.weight), sizeof(float));
				data1 = data1 + streampos(24);
			}
			file1.seekg(data2);
			while (data2 < data2End) {
				file1.read(reinterpret_cast<char*>(&temp2), sizeof(data));
				//if(level == 1)cout << temp2.weight << "\t";
				outFile.write(temp2.getID, 10);
				outFile.write(temp2.putID, 10);
				outFile.write(reinterpret_cast<const char*>(&temp2.weight), sizeof(float));
				data2 = data2 + streampos(24);
			}
			curAddr = curAddr + streampos(2 * level * 24 * bufSize);
			file1.seekg(curAddr);
		}

		file1.close();
		fileName += ".bin";
		const char* delFile = fileName.c_str();
		remove(delFile);
	}

	/*
	 * Sort data, then write the sorted data into a file.
	 * @param fileName: the input filename
	 * @param i: how many times writing data
	 */
	void WriteBinaryFile(const string& fileName, int i) {
		std::ofstream outFile;
		if (i == 0) {															// first time writing data, do not use append mode
			outFile.open("order0" + fileName + ".bin", std::ios::binary);
		}
		else {
			outFile.open("order0" + fileName + ".bin", std::ios::binary | std::ios::app);
		}

		if (!outFile) {															// if the file failed to open
			std::cerr << "Cannot open input" + fileName + ".bin" << std::endl;
			return;
		}

		stable_sort(initdata.begin(), initdata.end(),									// sort each data in initdata vector by weight in descending order
			[](const data& d1, const data& d2)
			{ return d1.weight > d2.weight; });

		for (int i = 0; i < initdata.size(); i++) {
			outFile.write(initdata[i].getID, 10);
			outFile.write(initdata[i].putID, 10);
			outFile.write(reinterpret_cast<const char*>(&initdata[i].weight), sizeof(float));
		}
		outFile.close();
	}

	void ClearInitdata() {
		initdata.clear();
		return;
	}

	void printInitdata() {
		for (int i = 0; i < initdata.size(); i++) {
			cout << "[" << i << "] " << initdata[i].putID << " -> " << initdata[i].getID << " : " << initdata[i].weight << endl;
		}
	}


	void buildIndex(string fileName) {
		data temp;
		Index indexTemp;
		int offsetTemp;
		ifstream file(fileName + ".bin", ios::binary);
		streampos curWeightPtr = file.tellg();
		float curWeight = -1.0;
		file.seekg(0, ios::end);
		streampos fileSize = file.tellg();
		file.seekg(ios::beg);
		// It need get first data to check latter change
		file.read(reinterpret_cast<char*>(&temp), sizeof(data));
		curWeight = temp.weight;
		indexTemp.offset = (int(file.tellg()) - long(24) - file.beg) / 24;
		indexTemp.weight = curWeight;
		primIndex.push_back(indexTemp);
		while (file.tellg() < fileSize) {
			if (temp.weight != curWeight) {      // when get different weight need add new index to store.
				curWeight = temp.weight;
				indexTemp.offset = (int(file.tellg()) - long(24) - file.beg) / 24; // the memory address gap
				indexTemp.weight = curWeight;
				primIndex.push_back(indexTemp);
			}
			file.read(reinterpret_cast<char*>(&temp), sizeof(data));
			//if (temp.weight >= 0.9)cout << temp.getID << " " << temp.putID << " " << temp.weight << endl;
		}
		cout << "<Primary index>: (key, offset)" << endl;
		for (int i = 0; i < primIndex.size(); i++) {
			cout << "[" << i + 1 << "] (" << primIndex[i].weight << ", " << primIndex[i].offset << ")" << endl;
		}
	}

	string ftos(float f) {
		string s = to_string(f);
		int num = 0;
		for (int i = s.length() - 1; i >= 0; i--) {
			if (s[i] == '0') num++;
			else {
				if (s[i] == '.') num++;
				break;
			}
		}
		s = s.substr(0, s.length() - num);

		return s;
	}

	// Not sure is it finished or not.
	void RangeSearch(string fileName, float threshold) {
		std::ifstream inFile("order" + fileName + ".bin");
		std::ofstream outFile("order" + fileName + ".txt");
		data temp;
		/*float curWeight = -1.0;
		string curputID = "";
		string curgetID = "";*/
		bool isEnd = false;
		string line = "";
		int dataNum = 0;
		streampos filePos = 0;
		inFile.seekg(0, ios::beg);
		double start = double(clock()) / CLOCKS_PER_SEC;
		for (int i = 0; i < primIndex.size(); i++) {
			dataNum = primIndex[i].offset;
			filePos = streampos(primIndex[i].offset * 24);
			inFile.seekg(filePos);
			inFile.read(reinterpret_cast<char*>(&temp), sizeof(data));
			if (temp.weight >= threshold) {
				line += "[" + to_string(primIndex[i].offset + 1) + "]" + " " + temp.putID + " " + temp.getID + " " + ftos(temp.weight) + "\n";
				/*if (temp.weight != curWeight) {
					if (curWeight != -1.0) {
						line = line + " " + to_string(curWeight) + "\n";
						outFile << line;
					}
					line = "[" + to_string(index.offset + 1) + "]";
				}
				else {
					line = line + " " + temp.getID + " " + temp.putID;
				}
				curWeight = temp.weight;
				curputID = temp.putID;
				curgetID = temp.getID;*/
			}
			else {
				break;
			}
		}
		if (primIndex[primIndex.size() - 1].weight >= threshold) {
			inFile.seekg(0, ios::end);
			dataNum = (int)inFile.tellg() / 24;
		}
		double end = double(clock()) / CLOCKS_PER_SEC;
		//outFile.clear();
		//outFile.seekp(0, ios::beg);
		outFile << "Search " + ftos(threshold) + " : " + to_string(dataNum) + " records, " + ftos((end - start) * 1000) + " ms\n";
		outFile << line;
		outFile.close();
		inFile.close();
		return;
	}
};
// *************************************** end Sort ***************************************

bool FileExist(const string& fileName) {
	ifstream file(("pairs" + fileName + ".bin"), ios::binary);
	if (!file.is_open()) {
		cout << "\npairs" + fileName + ".bin does not exist!!!\n" << endl;
		return false;
	}
	return true;
}

int main()
{
	string fileName = "";
	int bufSize = 0;

	do {
		do
		{
			cout << "Input a number in [1, 60000]: " << endl;
			cin >> bufSize;
			if (bufSize < 1 || bufSize > 60000) {
				cout << "\n### It is NOT in [1,60000] ###\n" << endl;
			}
		} while (bufSize < 1 || bufSize > 60000);
		cout << "\n*** buffer size is " << bufSize << endl;
		cout << "************************************************" << endl;
		cout << " On - machine Exercise 05                      *" << endl;
		cout << " Mission 1 : External Merge Sort on a Big File *" << endl;
		cout << " Mission 2 : Construction of Primary Index     *" << endl;
		cout << " Mission 3 : Threshold search on primary index *" << endl;
		cout << "************************************************" << endl;

		/********************************************************************* start Mission 1 *********************************************************************/
		cout << "########################################################" << endl;
		cout << "Mission 1 : External merge sort" << endl;
		cout << "########################################################\n" << endl;

		do
		{
			cout << "Input the file name: [0]Quit" << endl;
			cin >> fileName;
		} while (fileName != "0" && !FileExist(fileName));

		if (fileName == "0") break;
		Sort s;
		double start1 = double(clock()) / CLOCKS_PER_SEC;
		int runsNum = s.getRunsNum(fileName, bufSize);
		for (int i = 0; i < runsNum; i++) {
			s.ReadBinaryFile(fileName, bufSize, i);
			s.WriteBinaryFile(fileName, i);
			s.ClearInitdata();
		}
		double end1 = double(clock()) / CLOCKS_PER_SEC;
		cout << "\nThe internal sort is completed. Check the initial sorted runs!" << endl;
		cout << "\nNow there are " + to_string(runsNum) + " runs." << endl;
		float runsTmp = runsNum;
		double start2 = double(clock()) / CLOCKS_PER_SEC;
		int i = 1;
		for (i = 1; i < runsNum; i *= 2) {
			s.merge("order" + to_string(i / 2) + fileName, "order" + to_string(i) + fileName, bufSize, i);
			runsTmp /= 2;
			cout << "\nNow there are " + to_string(int(ceil(runsTmp))) + " runs." << endl;
		}
		string oldName = "order" + to_string(i / 2) + fileName + ".bin";
		string newName = "order" + fileName + ".bin";
		int result = rename(oldName.c_str(), newName.c_str());
		if (result != 0) {
			const char* delFile = oldName.c_str();
			remove(delFile);
		}
		double end2 = double(clock()) / CLOCKS_PER_SEC;
		/********************************************************************* end Mission 1 *********************************************************************/

		cout << "\nThe execution time ..." << endl;
		cout << "Internal Sort = " << (end1 - start1) * 1000 << " ms" << endl;
		cout << "External Sort = " << (end2 - start2) * 1000 << " ms" << endl;
		cout << "Total Execution Time = " << ((end2 - start2) + (end1 - start1)) * 1000 << " ms\n" << endl;

		/********************************************************************* start Mission 2 *********************************************************************/
		cout << "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
		cout << "Mission 2: Build the primary index" << endl;
		cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" << endl;

		s.buildIndex("order" + fileName);
		cout << endl;
		/********************************************************************* end Mission 2 *********************************************************************/

		/********************************************************************* start Mission 3 *********************************************************************/
		cout << "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
		cout << "Mission 3: Threshold search on primary index" << endl;
		cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" << endl;
		float threshold = 0;
		string conti = "0";
		do
		{
			cout << "Please input a threshold in the range [0,1]:" << endl;
			cin >> threshold;
			if (threshold <= 0 || threshold > 1) {
				cout << "\nThe threshold is not in the range [0,1]!" << endl;
			}
			else {
				s.RangeSearch(fileName, threshold);
			}
			cout << "\n[0]Quit or [Any other key]continue?" << endl;
			cin >> conti;
			cout << endl;
		} while (conti != "0");
		/********************************************************************* end Mission 3 *********************************************************************/

		cout << "[0]Quit or [Any other key]continue?" << endl;
		cin >> fileName;
		cout << endl;
	} while (fileName != "0");

	/*s.ReadOrderBinaryFile("501");
	s.printInitdata();*/

	return 0;
}
