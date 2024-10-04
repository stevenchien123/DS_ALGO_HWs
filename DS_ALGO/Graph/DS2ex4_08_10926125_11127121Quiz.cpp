//資訊二甲 第8組 11127121鄧俊揚 10926125簡紹軒
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <unordered_map>
#include <random>
#include <time.h>


using namespace std;

// *************************************** Start Node ***************************************
class Node {
public:
    string sid;
    float weight;
    float randomWeight;
    unordered_map<string, float> unOrderADJ;      // mission1: adjacency list
    unordered_map<string, float> unOrderCNT;      // mission2: connection counts list
    unordered_map<string, float> unOrderINF;      // mission3
    unordered_map<string, float> unOrderPRO;      // mission4
    // unordered_map is a hash table have two element, fist element is key , second is value
    // let search from O(logn) to O(1) better than binary search.
    // insert use => []operator , for example : unOrderADJ[this->sid] = this->weight, then we can use this->sid to search this->weight
    // or check this->sid whether exist in unOrderADJ or not.
    Node() {}

    Node(const string& sid) : sid(sid), weight(0) {}

    Node(const string& sid, float weight) : sid(sid), weight(weight) {}

    bool operator==(const Node& rhs) const noexcept {
        return this->sid == rhs.sid;
    }
};
// *************************************** End Node ***************************************

bool compareSid(const Node& n1, const Node& n2) { // custom compare function for std::sort()
    return (n1.sid < n2.sid); // ascending order of sid
}

bool compareInf(const Node& n1, const Node& n2) {
    return (n1.unOrderINF.size() > n2.unOrderINF.size());
}

bool compareCnt(const Node& n1, const Node& n2) { // custom compare function for std::sort()
    return (n1.unOrderCNT.size() > n2.unOrderCNT.size()); // descending order of cntList.size()
}

bool comparePro(const Node& n1, const Node& n2) { // custom compare function for std::sort()
    return (n1.unOrderPRO.size() > n2.unOrderPRO.size()); // descending order of cntList.size()
}

int binarySearch(const vector<Node>& vec, const string& target) {
    // not be used, because we use unordered_map to search, not use vector
    int left = 0;
    int right = vec.size() - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (vec[mid].sid == target) {
            return mid;
        }
        else if (vec[mid].sid < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return right + 1;
}

bool contains(const vector<Node>& vec, const string& sid) {
    // not be used, because we use unordered_map to search, not use vector
    if (vec.empty()) {
        return false;
    }
    int temp = binarySearch(vec, sid);
    if (temp > vec.size() - 1 || temp < 0) {
        return false;
    }
    return vec[temp].sid == sid;
}

// *************************************** Start Graph ***************************************
class Graph {
private:
    struct data {
        char putID[12];
        char getID[12];
        float weight;
    };
    vector<data> initdata; // store each put/get info.
    unordered_map<string, Node> searchList; // replace vector<Node> totaldata


public:
    bool exist() {
        return !initdata.empty();
    }

    void clear() {
        initdata.clear();
        searchList.clear();
    }

    void buildGraph() {
        for (const data& d : initdata) {
            if (searchList.find(d.putID) == searchList.end()) {
                searchList[d.putID] = Node(d.putID);
            }
            if (searchList.find(d.getID) == searchList.end()) {
                searchList[d.getID] = Node(d.getID);
            }
            searchList[d.putID].unOrderADJ[d.getID] = d.weight;
        }
    }

    void BFS() {
        queue<Node> q;											// Create a stack of BFS
        for (const auto& pair : searchList) {
            q.push(pair.second);
            // Iterate over the queue
            while (!q.empty()) {
                Node currentNode = q.front();						// Dequeue a vertex from queue
                q.pop();
                // find index of currentNode in totaldata vector
                // Get all adjacent vertices of the dequeued vertex
                // currentNode If an adjacent has not been visited,
                // then mark it visited and enqueue it
                // Node n : totaldata[index].adjList
                for (const auto& pair2 : searchList[currentNode.sid].unOrderADJ) {
                    // if n not in cntList (not visited), then push into queue
                    if (pair.second.unOrderCNT.find(pair2.first) == pair.second.unOrderCNT.end() && pair2.first != pair.first) {
                        searchList[pair.first].unOrderCNT[pair2.first] = pair2.second;
                        q.push(Node(pair2.first, pair2.second));
                    }
                }
            }
        }
    }

    void DFS(double threshold) {
        for (const auto& pair : searchList) {
            stack<Node> s;
            s.push(pair.first);

            unordered_map<string, bool> visited;
            visited[pair.first] = true;
            while (!s.empty()) {
                Node currentNode = s.top();                         // priority node to search next step.
                if (currentNode.weight >= threshold) {
                    // if currentNode can connect to next step(bigger than threshold), record currentNode to unOrderINF.
                    searchList[pair.first].unOrderINF[currentNode.sid] = currentNode.weight;
                }

                bool end = true; // initial the path is to the end, if not will upadte to false.

                for (const auto& pair2 : searchList[currentNode.sid].unOrderADJ) {
                    if (visited.find(pair2.first) == visited.end() && pair2.second >= threshold && pair2.first != pair.first) {
                        // only check and push that next step weight exceed threshold and isn't visited.
                        visited[pair2.first] = true;                // record visited
                        s.push(Node(pair2.first, pair2.second));    // record this fork all possible path.
                        end = false;                                // find the path to next step, proof it no end.
                    }
                }

                if (end) // if to the end, pop to last step ( backtrack )
                    s.pop();
            }
        }
    }

    double Probability_based() {
        std::random_device rd;
        std::mt19937 gen(rd());

        // Set range from 0.8 ~ 1.0
        std::uniform_real_distribution<> dis(0.8, 1.0);

        // Generate a threshold
        return dis(gen);
    }

    void ProbailityINF() {
        for (auto& pair : searchList) {
            pair.second.unOrderPRO.clear();
        }
        double start = double(clock()) / CLOCKS_PER_SEC;
        queue<Node> s;
        for (const auto& pair : searchList) {
            s.push(pair.second);
            // Iterate over the queue
            while (!s.empty()) {
                Node currentNode = s.front();						// Dequeue a vertex from queue
                s.pop();
                // find index of currentNode in totaldata vector
                // Get all adjacent vertices of the dequeued vertex
                // currentNode If an adjacent has not been visited,
                // then mark it visited and enqueue it
                // Node n : totaldata[index].adjList
                for (const auto& pair2 : searchList[currentNode.sid].unOrderADJ) {
                    //long ran = long(&pair2.second);
                    double ranThreshold = 0.8 + (long(&pair2) / (int(clock()))) % 100 / 500.0;
                    // cout << ranThreshold << endl;
                    //float ranThreshold = Probability_based();
                    // if n not in cntList (not visited), then push into queue
                    if (pair.second.unOrderPRO.find(pair2.first) == pair.second.unOrderPRO.end() && pair2.first != pair.first && pair2.second >= ranThreshold) {
                        searchList[pair.first].unOrderPRO[pair2.first] = pair2.second;
                        s.push(Node(pair2.first, pair2.second));
                    }
                }
            }

        }
        double end = double(clock()) / CLOCKS_PER_SEC;
        cout << "\n[Elapsed time]: " << (end - start) * 1000 << " ms" << endl;

        return;
    }

    bool ReadBinaryFile(const string& fileName) {
        ifstream file(("pairs" + fileName + ".bin"), ios::binary);
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

    void WriteAdjData(const string& fileName) {
        ofstream file(("pairs" + fileName + ".adj"));
        if (!file.is_open()) {
            cout << "WriteAdjData: Failed to open file.\n";
            return;
        }
        cout << "\n<<< There are " << searchList.size() << " IDs in total. >>>\n";
        file << "<<< There are " << searchList.size() << " IDs in total. >>>";
        int nodeNum = 0;
        vector<Node> sortedData;
        vector<Node> sortedADJ;
        for (const auto& pair : searchList) {
            sortedData.push_back(pair.second);
        }
        sort(sortedData.begin(), sortedData.end(), compareSid);
        int i = 0;
        for (size_t i = 0; i < sortedData.size(); ++i) {
            const Node& node = sortedData[i];
            file << "\n[";
            for (int k = i + 1; k < 100; k *= 10) file << " ";
            file << i + 1 << "] " << node.sid << ":";
            sortedADJ.clear();
            for (const auto& pair : node.unOrderADJ) {
                sortedADJ.push_back(Node(pair.first, pair.second));
            }
            sort(sortedADJ.begin(), sortedADJ.end(), compareSid);
            for (int j = 0; j < sortedADJ.size(); j++) {
                if (j % 12 == 0) file << "\n\t";
                file << "(";
                if (j < 9) file << " ";
                file << j + 1 << ") ";
                file << sortedADJ[j].sid << ",\t " << sortedADJ[j].weight << "\t";
                nodeNum++;
            }
        }
        cout << "\n<<< There are " << nodeNum << " nodes in total. >>> \n\n";
        file << "\n<<< There are " << nodeNum << " nodes in total. >>> \n";
        file.close();
        /*string oldName = "pairs" + fileName + ".txt";
        string newName = "pairs" + fileName + ".adj";
        if (rename(oldName.c_str(), newName.c_str()) == -1) {
            cout << "file rename failed." << endl;
        }*/
    }

    void WriteCntData(const string& fileName, int lastCommand) {
        if (!exist()) {
            cout << "### There is no graph and choose 1 first. ###\n" << endl;
            return;
        }
        if (lastCommand == 2) {
            cout << "\n<<< There are " << searchList.size() << " IDs in total. >>>\n\n";
            return;
        }
        ofstream file(("pairs" + fileName + ".cnt"));
        if (!file.is_open()) {
            cout << "WriteCntData: Failed to open file.\n";
            return;
        }
        cout << "\n<<< There are " << searchList.size() << " IDs in total. >>>\n\n";
        file << "<<< There are " << searchList.size() << " IDs in total. >>>";
        vector<Node> sortedData;
        vector<string> sortedCNT;
        for (const auto& pair : searchList) {
            sortedData.push_back(pair.second);
        }
        sort(sortedData.begin(), sortedData.end(), compareSid);
        stable_sort(sortedData.begin(), sortedData.end(), compareCnt);
        int i = 0;
        for (size_t i = 0; i < sortedData.size(); ++i) {
            const Node& node = sortedData[i];
            file << "\n[";
            for (int k = i + 1; k < 100; k *= 10) file << " ";
            file << i + 1 << "] " << node.sid << "(" << node.unOrderCNT.size() << "): ";
            sortedCNT.clear();
            for (const auto& pair : node.unOrderCNT) {
                sortedCNT.push_back(pair.first);
            }
            sort(sortedCNT.begin(), sortedCNT.end(), compareSid);
            for (int j = 0; j < sortedCNT.size(); j++) {
                if (j % 12 == 0) file << "\n\t";
                file << "(";
                if (j < 9) file << " ";
                file << j + 1 << ") ";
                file << sortedCNT[j] << "\t";
            }
        }
        file.close();
        /*string oldName = "pairs" + fileName + ".txt";
        string newName = "pairs" + fileName + ".cnt";
        if (rename(oldName.c_str(), newName.c_str()) == -1) {
            cout << "file rename failed." << endl;
        }*/
    }

    void WriteINFData(const string& fileName, int lastCommand) {
        if (!exist()) {
            cout << "### There is no graph and choose 1 first. ###\n" << endl;
            return;
        }
        ofstream file(("pairs" + fileName + ".inf"));
        if (!file.is_open()) {
            cout << "WriteCntData: Failed to open file.\n";
            return;
        }
        vector<Node> sortedData;
        vector<string> sortedINF;
        for (const auto& pair : searchList) {
            if (pair.second.unOrderINF.size() > 0)
                sortedData.push_back(pair.second);
        }
        cout << "\n<<< There are " << sortedData.size() << " IDs in total. >>>\n\n";
        file << "<<< There are " << sortedData.size() << " IDs in total. >>>";
        sort(sortedData.begin(), sortedData.end(), compareSid);
        stable_sort(sortedData.begin(), sortedData.end(), compareInf);
        int i = 0;
        for (size_t i = 0; i < sortedData.size(); ++i) {
            const Node& node = sortedData[i];
            file << "\n[";
            for (int k = i + 1; k < 100; k *= 10) file << " ";
            file << i + 1 << "] " << node.sid << "(" << node.unOrderINF.size() << "): ";
            sortedINF.clear();
            for (const auto& pair : node.unOrderINF) {
                sortedINF.push_back(pair.first);
            }
            sort(sortedINF.begin(), sortedINF.end(), compareSid);
            for (int j = 0; j < sortedINF.size(); j++) {
                if (j % 12 == 0) file << "\n\t";
                file << "(";
                if (j < 9) file << " ";
                file << j + 1 << ") ";
                file << sortedINF[j] << "\t";
            }
        }
        file.close();
        /*string oldName = "pairs" + fileName + ".txt";
        string newName = "pairs" + fileName + ".inf";
        if (rename(oldName.c_str(), newName.c_str()) == -1) {
            cout << "file rename failed." << endl;
        }*/
    }

    void ShowTopKdata() {
        vector<Node> sortedData;
        for (const auto& pair : searchList) {
            if (pair.second.unOrderPRO.size() > 0)
                sortedData.push_back(pair.second);
        }
        int k = 0;
        int proNum = sortedData.size();
        if (proNum > 0) {
            do {
                cout << endl << "Input an integer to show top-K in [1," << proNum << "]: ";
                cin >> k;
                if (1 > k || k > proNum) {
                    cout << "### It is NOT in [1," << proNum << "] ###\n\n";
                }
            } while (k < 1 || k > proNum);
        }
        else {
            cout << "No Data." << endl;
            return;
        }
        sort(sortedData.begin(), sortedData.end(), compareSid);
        stable_sort(sortedData.begin(), sortedData.end(), comparePro);
        cout << endl;
        for (int i = 0; i < k; i++) {
            cout << "<" << i + 1 << "> " << sortedData[i].sid << ": " << sortedData[i].unOrderPRO.size() << endl;
        }
        cout << endl;
    }
};
// *************************************** End Graph ***************************************

int main()
{
    string command;
    string fileName;
    int lastCommand = 0;
    Graph graph;

    do {
        cout << "**** Graph data manipulation *****\n";
        cout << "* 0. QUIT                        *\n";
        cout << "* 1. Build adjacency lists       *\n";
        cout << "* 2. Compute connection counts   *\n";
        cout << "* 3. Estimate influence values   *\n";
        cout << "* 4. Probability-based influence *\n";
        cout << "**********************************\n";
        cout << "Input a choice(0, 1, 2, 3, 4): ";
        cin >> command;

        if (command == "1") {
            graph.clear();

            cout << endl << "Input a file number ([0] Quit): ";
            cin >> fileName;

            if (graph.ReadBinaryFile(fileName)) {
                if (fileName != "0") {
                    graph.buildGraph();
                    graph.WriteAdjData(fileName);
                    lastCommand = 1;
                }
            }
            //lastCommand = 1;
        }
        else if (command == "2") {
            if (lastCommand == 1) {
                if (graph.exist()) {
                    graph.BFS();
                    graph.WriteCntData(fileName, lastCommand);
                    lastCommand = 2;
                }
            }
            else if (lastCommand == 2) {
                graph.WriteCntData(fileName, lastCommand);
                lastCommand = 2;
            }
            else {
                cout << "### There is no graph and choose 1 first. ###\n" << endl;
            }
            //lastCommand = 2;
        }
        else if (command == "3") {
            if (!graph.exist()) {
                cout << "### There is no graph and choose 1 first. ###\n" << endl;
            }
            else {
                double threshold = 0;
                do {
                    cout << endl << "Input a real number in [0.5,1]: ";
                    cin >> threshold;
                    if (0.5 > threshold || threshold > 1) {
                        cout << "### " << threshold << " is NOT in [0.5,1] ###\n\n";
                    }
                } while (threshold < 0.5 || threshold > 1);
                graph.DFS(threshold);
                graph.WriteINFData(fileName, lastCommand);
            }
        }
        else if (command == "4") {
            if (!graph.exist()) {
                cout << "### There is no graph and choose 1 first. ###\n" << endl;
            }
            else {
                graph.ProbailityINF();
                graph.ShowTopKdata();
            }
        }
        else if (stoi(command) > 4 || stoi(command) < 0) {
            cout << "\nCommand does not exist!\n\n";
            //lastCommand = 0;
        }
    } while (command != "0");

    return 0;
}