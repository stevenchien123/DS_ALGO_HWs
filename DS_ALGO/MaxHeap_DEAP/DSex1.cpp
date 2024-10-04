// �Ǹ�: 10926125�A�m�W: ²�аa
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
using namespace std;

//****************************************************** start School ******************************************************
class School {                                                         // �Ǯո��
private:
    int id;                                                            // �ߤ@�Ǹ�
    //string schoolId;                                                   // �ǮեN�X
    string schoolName;                                                 // �ǮզW��
    //string depId;                                                      // ��t�N�X
    string depName;                                                    // ��t�W��
    string type;                                                       // ��]�O
    string level;                                                      // ���ŧO
    int studentNum;                                                    // ����Ƥ��ǥͼ�

public:
    School() {                                                         // Constructor without parameters
        this->id = -1;
        this->schoolName = "";
        this->depName = "";
        this->level = "";
        this->type = "";
        this->studentNum = -1;
    }
    School(int id, int stdn) {                                         // Constructor with parameters
        this->id = id;
        this->studentNum = stdn;
    }
    ~School() {}                                                       // Destructor
    
    int getId() {                                                      // Getter & Setter
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    int getStudentNum() {
        return studentNum;
    }
    void setStudentNum(int stdn) {
        this->studentNum = stdn;
    }
    string getSchoolName() {
        return schoolName;
    }
    void setSchoolName(string sn) {
        this->schoolName = sn;
    }
    string getDepName() {
        return depName;
    }
    void setDepName(string dn) {
        this->depName = dn;
    }
    string getType() {
        return type;
    }
    void setType(string type) {
        this->type = type;
    }
    string getLevel() {
        return level;
    }
    void setLevel(string level) {
        this->level = level;
    }
};
//****************************************************** end School ******************************************************

//****************************************************** start MaxHeap ******************************************************
class MaxHeap {
private:
    vector<School> maxHeap;

public:
    void insert(School node);               // ���J max heap
    void ReHeapUp();                        // �վ� heap �����c�A�Ϥ����� heap
    void showNODES();                       // �L�X�D�حn�D���T����� (root, last node, leftmost node)
    bool isEmpty();                         // �P�_ max heap �O�_����
    void clearUp();                         // �M�� heap �����
    int treeHeight(int idx);                // �� idx ���𰪡Atree height of root = 0
    int parent(int idx);                    // �� idx �� parent index
};

void MaxHeap::clearUp() {
    maxHeap.clear();
}

int MaxHeap::treeHeight(int idx) {          // �� idx ���𰪡Atree height of root = 0
    return floor(log2(idx + 1));
}

int MaxHeap::parent(int idx) {              // �� idx �� parent index
    return ceil((double)idx / 2) - 1;
}

bool MaxHeap::isEmpty() {
    return maxHeap.size() == 0 ? true : false;
}

void MaxHeap::ReHeapUp() {
    if (isEmpty()) {                                                           // �Y���šA�h�L�X���~�T��
        cout << "ReHeapUp error: "
            << "Please insert node first." << endl;
        return;
    }

    int idx = maxHeap.size() - 1;                                              // index of the last node
    int p = parent(idx);                                                       // parent index of the last node

    while (idx > 0) {                                                          // �վ� heap ���� idx �� root ����
        if (maxHeap[idx].getStudentNum() <= maxHeap[p].getStudentNum()) {      // �Y�S���� parent �����٤j
            return;                                                            // �h����
        }
        swap(maxHeap[idx], maxHeap[p]);                                        // �Ϥ��A�h�洫��m
        idx = p;                                                               // ��s index
        p = parent(idx);
    }
}

void MaxHeap::insert(School node) {
    maxHeap.push_back(node);
    ReHeapUp();                                      // insert ����n�վ� heap �����c
}

void MaxHeap::showNODES() {
    cout << "\n<Max Heap>" << endl;

    if (isEmpty()) {                                                                           // �Y max heap ����
        cout << "No element yet." << endl;                                                     // �L�X���~�H���� return
        return;
    }

    cout << "root: " << "[" << maxHeap[0].getId() << "] "                                      // �L�X root
        << maxHeap[0].getStudentNum() << endl;

    cout << "bottom: " << "[" << maxHeap[maxHeap.size() - 1].getId() << "] "                   // �L�X heap ���̫�@�� node
        << maxHeap[maxHeap.size() - 1].getStudentNum() << endl;

                                                                                               // �L�X leftmost node in max heap
    if (maxHeap.size() == 1) {                                                                 // �Y heap ���u���@�� node
        cout << "leftmost bottom: " << "[" << maxHeap[0].getId() << "] "                       // �L�X root
            << maxHeap[0].getStudentNum() << endl;
    }

    int idx = 0;                                                                               // �_�h���_�����l�I��
    while (idx * 2 + 1 <= maxHeap.size() - 1) {                                                // ����W�X max heap �d�򬰤�
        idx = idx * 2 + 1;                                                                     // idx �� leftmost node ������
    }
    cout << "leftmost bottom: " << "[" << maxHeap[idx].getId() << "] "
        << maxHeap[idx].getStudentNum() << endl;
}
//****************************************************** end MaxHeap ******************************************************

//****************************************************** start DEAP ******************************************************
class DEAP {
private:
    vector<School> deap;

public:
    int getDeapSize();                      // ���o deap ���j�p
    void insert(School node);               // ���J deap
    void retrieveMax(vector<School>& maximums);                     // ���X maximum
    void Restructure();                     // �վ� deap �����c�A�Ϥ����� deap
    void showNODES();                       // �L�X�D�حn�D���ⵧ��� (last node, leftmost node)
    bool isEmpty();                         // �P�_ deap �O�_����
    void clearUp();                         // �M�� deap �����
    int inMinorMax(int idx);                // �P�_ idx ����m�b min �٬O max heap
    void ReHeapUpMax(int idx);              // �q idx �ѤU��W�վ� max heap �����c
    void ReHeapDownMax(int idx);            // �q idx �ѤW��U�վ� max heap �����c
    void ReHeapUpMin(int idx);              // �q idx �ѤU��W�վ� min heap �����c
    int treeHeight(int idx);                // �� idx ���𰪡Atree height of root = 0
    int parent(int idx);                    // �� idx �� parent index
    int maxChild(int idx);                  // �� idx ���j�� child
    int minChild(int idx);                  // �� idx ���p�� child
};

int DEAP::getDeapSize() {
    return deap.size() - 1;
}

void DEAP::clearUp() {
    deap.clear();
}

int DEAP::maxChild(int idx) {
    int idx1 = idx * 2 + 1;
    int idx2 = idx * 2 + 2;

    if (idx2 > deap.size() - 1) {                                               // �Y idx2 �W�X deap ���d��
        if (idx1 > deap.size() - 1) {                                               // �B idx1 �]�W�X�d��
            return -1;                                                              // return -1 ��� idx �S�� child
        }
        else {                                                                      // �� idx1 �S���W�X�d��
            return idx1;                                                            // �N�� idx �u�� idx1 �@�� child�A���� return
        }
    }

    if (deap[idx1].getStudentNum() > deap[idx2].getStudentNum()) {              // idx1 �M idx2 ���b�d��
        return idx1;
    }
    return idx2;
}

int DEAP::treeHeight(int idx) {                                  // �� idx ���𰪡Atree height of root = 0
    return floor(log2(idx + 1));
}

int DEAP::parent(int idx) {                                      // �� idx �� parent index
    return ceil((double)idx / 2) - 1;
}

bool DEAP::isEmpty() {
    return deap.size() == 1 ? true : false;
}

int DEAP::inMinorMax(int idx) {
    while (idx != 1 && idx != 2) {                                              // ���_�� parent �]�A���� index �� 1 �� 2
        idx = parent(idx);
    }

    if (idx == 1) return 1;                                                     // �Y idx �� 1: �b min heap�A�Ϥ��h�b max heap
    else return 2;                                                              // �]�� min heap �b���l��A�ӥ��l�� root index �� 1�A�P�z max heap
}

void DEAP::ReHeapDownMax(int idx) {
    if (idx > deap.size() - 1) {                                                // deap ���� max heap �S����ƤF
        return;
    }

    int child = maxChild(idx);                                                  // ��X idx ���̤j child

    while (child != -1) {                                                       // �� idx �� child �s�b (idx ���O leaf)
        if (deap[child].getStudentNum() > deap[idx].getStudentNum()) {          // �Y child �� idx �����٤j
            swap(deap[child], deap[idx]);                                       // ��̦�m�洫
            idx = child;                                                        // idx ���U�]
            child = maxChild(idx);                                              // ��s child idex
        }
        else {                                                                  // �Y child �� idx �����٤p
            return;                                                             // �վ㵲���A���� return
        }
    }

    int y = idx - pow(2, treeHeight(idx) - 1);                                  // y �� idx �۹����� node index
    int yChild = maxChild(y);                                                   // yChild �� y �̤j�� child

    if (yChild != -1) {                                                         // �� yChild �s�b (�Ҽ{ special case)
        if (deap[yChild].getStudentNum() < deap[idx].getStudentNum()) {         // idx �� yChild �����٤j
            return;                                                             // �վ㵲���A���� return
        }
        else {                                                                  // idx �� yChild �����٤p (�i�J special case)
            swap(deap[yChild], deap[idx]);                                      // ��̥洫��m
            ReHeapUpMin(yChild);                                                // �b yChild ���a��վ� min heap
        }
    }
    else {                                                                      // �� yChild ���s�b
        if (deap[y].getStudentNum() < deap[idx].getStudentNum()) {              // idx �� y �����٤j
            return;
        }
        else {                                                                  // idx �� y �����٤p
            swap(deap[y], deap[idx]);                                           // ��̥洫��m
            ReHeapUpMin(y);                                                     // �b y ���a��վ� min heap
        }
    }
}

void DEAP::ReHeapUpMax(int idx) {
    if (isEmpty()) {                                                           // �Y���šA�h�L�X���~�T��
        cout << "ReHeapUp error: "
            << "Please insert node first." << endl;
        return;
    }

    int p = parent(idx);                                                       // parent index of the last node

    while (idx > 0 && p != 0) {                                                // �վ� heap ���� idx �� root ����
                                                                               // �� parent index �� 0 ���� (index 0 ���s��)
        if (deap[idx].getStudentNum() <= deap[p].getStudentNum()) {            // �Y�S���� parent �����٤j
            return;                                                            // �h����
        }
        swap(deap[idx], deap[p]);                                              // �Ϥ��A�h�洫��m
        idx = p;                                                               // ��s index
        p = parent(idx);
    }
}

void DEAP::ReHeapUpMin(int idx) {
    if (isEmpty()) {                                                           // �Y���šA�h�L�X���~�T��
        cout << "ReHeapUp error: "
            << "Please insert node first." << endl;
        return;
    }

    int p = parent(idx);                                                       // parent index of the last node

    while (idx > 0 && p != 0) {                                                // �վ� heap ���� idx �� root ����
                                                                               // �� parent index �� 0 ���� (index 0 ���s��)
        if (deap[idx].getStudentNum() >= deap[p].getStudentNum()) {            // �Y�S���� parent �����٤p
            return;                                                            // �h����
        }
        swap(deap[idx], deap[p]);                                              // �Ϥ��A�h�洫��m
        idx = p;                                                               // ��s index
        p = parent(idx);
    }
}

void DEAP::Restructure() {
    int idx = deap.size() - 1;                                                  // index of the last node
    if (idx == 1) {                                                             // ���J�Ĥ@�� node ���νվ㵲�c
        return;                                                                 // �ҥH���� return
    }

    int check = inMinorMax(idx);                                                // check if the last node in min or max heap, 1:min, 2:max
    if (check == 1) {                                                           // �Y idx �b min heap
        int tmp = idx + pow(2, treeHeight(idx) - 1);
        int y = parent(tmp);                                                    // y �� idx �۹����� node index

        if (deap[idx].getStudentNum() > deap[y].getStudentNum()) {              // idx ��۹����� node �٤j
            swap(deap[idx], deap[y]);                                           // �洫��m
            ReHeapUpMax(y);                                                     // �վ� max heap �����c
        }
        else {
            ReHeapUpMin(idx);                                                   // �վ� min heap �����c
        }
    }
    else {                                                                      // �Y idx �b max heap
        int y = idx - pow(2, treeHeight(idx) - 1);                              // y �� idx �۹����� node index

        if (deap[idx].getStudentNum() < deap[y].getStudentNum()) {              // idx ��۹����� node �٤p
            swap(deap[idx], deap[y]);                                           // �洫��m
            ReHeapUpMin(y);                                                     // �վ� min heap �����c
        }
        else {
            ReHeapUpMax(idx);                                                   // �վ� max heap �����c
        }
    }
}

void DEAP::insert(School node) {
    if (deap.size() == 0) {                 // �Y deap ���S�� node
        School root;                        // �h�s�W�@�ӪŪ� node �i�h
        deap.push_back(root);
    }
    deap.push_back(node);
    Restructure();                          // insert ��n�վ� deap �����c
}

void DEAP::retrieveMax(vector<School>& maximums) {              // maximums �ΨӦs ���X���̤j�`�I
    int maximum = 0;

    if (isEmpty()) {                        // deap ����
        cout << "No elements in DEAP yet." << endl;
        return;
    }
    if (deap.size() - 1 == 1) {                 // deap �����ťB max heap ���S����� (�N��u�� min heap �����)
        maximums.push_back(deap[1]);            // �N��ƴ��J maximums
        deap.pop_back();                        // �������
        return;                                 // ���� return�A���� deap ����
    }
                                                        // deap �����ťB max heap �������
    maximums.push_back(deap[2]);                        // �N�̤j�ȴ��J maximums
    swap(deap[2], deap[deap.size() - 1]);               // maximum node �M last node ����m�洫
    deap.pop_back();                                    // �R�� deap �� maximum

    ReHeapDownMax(2);                                   // �q�쥻 maximum ����m�}�l�ѤW��U�վ� deap �����c
}

void DEAP::showNODES() {
    cout << "\n<DEAP>" << endl;

    if (isEmpty()) {                                                                        // �Y deap ����
        cout << "No element yet." << endl;                                                  // �L�X���~�H���� return
        return;
    }

    cout << "bottom: " << "[" << deap[deap.size() - 1].getId() << "] "                      // �L�X deap ���̫�@�� node
        << deap[deap.size() - 1].getStudentNum() << endl;

                                                                                            // �L�X leftmost node in deap
    int idx = 0;                                                                            // ���_�����l�I��
    while (idx * 2 + 1 <= deap.size() - 1) {                                                // ����W�X max heap �d�򬰤�
        idx = idx * 2 + 1;                                                                  // idx �� leftmost node ������
    }
    cout << "leftmost bottom: " << "[" << deap[idx].getId() << "] "
        << deap[idx].getStudentNum() << endl;
}
//****************************************************** end DEAP ******************************************************



bool readFile(vector<string>& lines) {                                 // Ū�ɡA�s�Jlines��
    int fileNum = -1;
    cout << "Input the file number([0]: Quit): ";
    cin >> fileNum;

    if(fileNum == 0) exit(EXIT_SUCCESS);                               // ��J 0 �h�����{��

    ifstream ifs("input" + to_string(fileNum) + ".txt", ios::in);
    if (!ifs.is_open()) {                                              // �Y fileNum.txt ���b����Ƨ�
        cout << "Failed to open file." << endl;                        // print ���~�T���� return
        cout << endl;
        return false;
    }

    string s;                                                          // �ɮ׶}�Ҧ��\
    getline(ifs, s);
    getline(ifs, s);
    getline(ifs, s);
    getline(ifs, s);                                                   // ������ g++ �sĶ���ܲĤ@��Ū�쪺�|�O�Ŧr��
                                                                       // �ĤG��}�l�~�O�ɮת��Ĥ@����

    while (getline(ifs, s)) {                                          // �q�ĥ|��}�l�B�z��ơAŪ���C�@�檽��S���U�@�欰��
        lines.push_back(s);                                            // �N�C���Ʀs�_��
    }
    return true;
}

int splitStdNum(string token) {
    if (token.empty()) return 0;                               // �Y�ǥͼ���쬰�šA���� 0 �Ӿǥ�

    /*
    for (int i = 0; i < token.length(); i++) {                 // �ǥͼƭY���Ӧ�ơAex: 2�A
        if (token[i] == '0') continue;                         // �h��Ʒ|��ܦ� 02
        token = token.substr(i, token.length());               // �ҥH�N�e���� 0 �h���A�~�ॿ�`���� int
        break;
    }
    */
    
    if (token.find(",") != string::npos) {                     // �]���ǥͼƱĥΤd��Ʈ榡�Aex: 1,000
        size_t found = token.find(",");                        // ��1,000���ઽ����int�A�|����
        string temp1 = token.substr(0, found);                 // �ҥH�A���@���r��A�u�O�d�Ʀr����
        string temp2 = token.substr(found + 1, token.length());
        token = temp1 + temp2;
    }
    //cout << token << endl;
    return stoi(token);
}

vector<School> dataProcess(vector<string>& lines) {                    // �B�z�ɮ׸��
    vector<School> schoolTypes;                                        // �N�B�z������Ʀs�J vector
    int i = 0;                                                         // i �N��ߤ@�Ǹ�

    for (string s : lines) {                                           // ��C���ư��B�z
        School school;                                                 // �C�泣�Ыؤ@�Ӫ���Ӧs���
        i++;
        std::istringstream iss(s);
        std::string token;
        int j = 0;                                                     // �H j ��@ iterator
        while (std::getline(iss, token, '\t')) {                       // �����X�C�檺�ǥͼ�
            switch (j)
            {
            case 1:                                                    // �Ĥ@�欰�ǮզW��
                school.setSchoolName(token);
                //cout << token << " ";
                break;
            case 3:                                                    // �ĤT�欰��t�W��
                school.setDepName(token);
                //cout << token << " ";
                break;
            case 4:                                                     // �ĥ|�欰��]�O
                school.setType(token);
                //cout << token << " ";
                break;
            case 5:                                                     // �Ĥ��欰���ŧO
                school.setLevel(token);
                //cout << token << " ";
                break;
            case 6:
            {
                int stdNum = splitStdNum(token);                       // �Ĥ��欰�ǥͼ�
                school.setStudentNum(stdNum);                          // stdNum ���ǥͼơA�N token �ন��ƫ�s�J
                //cout << stdNum << " ";
                break;
            }
            default:
                break;
            }
            j++;
        }
        school.setId(i);                                           // �]�w�ߤ@�Ǹ�
        //cout << i << endl;
        /*
        if (j != 6) {                                              // �ǥͼƦb�Ĥ������A�B�C������\t�j�}
            j++;                                                   // �ҥH��Ĥ�����촣���X�ӳB�z
            continue;                                              // token ���ǥͼƪ��r��Φ�
        }
        */
        // cout << stdNum << endl;
        schoolTypes.push_back(school);                             // �N�B�z������� push �i�t�~�@�� vector ��
    }
    return schoolTypes;
}

int main()
{
    int choise = 0;
    vector<string> lines;                                       // lines �O�s�ɮצU����
    MaxHeap maxHeap;
    DEAP deap;

    do {
        cout << "****** Heap Construction ******" << endl;
        cout << "* 0. QUIT                     *" << endl;
        cout << "* 1. Build a max heap         *" << endl;
        cout << "* 2. Build a DEAP             *" << endl;
        cout << "* 3. Top-K maximums from DEAP *" << endl;
        cout << "*******************************" << endl;
        cout << "Input a chiose(0, 1, 2, 3): ";

        cin >> choise;                                                  // ��J�n�����ӿﶵ
        cout << endl;

        lines.clear();                                                  // �M�� lines
        if (choise == 0) {                                               // 0: QUIT
            return 0;
        }
        else if (choise == 1) {                                         // 1: Build a max heap
            
            maxHeap.clearUp();                                          // �M�� max heap

            if (readFile(lines)) {                                      // �Y���\Ū��
                vector<School> schoolTypes;                             // schoolTypes �ΨӦs�B�z�n����ơA�C����Ƭ�School����
                schoolTypes = dataProcess(lines);                       // �B�z�ɮ׸��

                for (int i = 0; i < schoolTypes.size(); i++) {          // �̧Ǵ��J�� max heap
                    maxHeap.insert(schoolTypes[i]);
                }

                maxHeap.showNODES();                                    // �L�X�D�حn�D���T�����
            }
            cout << endl;
        }
        else if (choise == 2) {                                         // 2: Build a DEAP
            
            deap.clearUp();                                             // �M�� deap

            if (readFile(lines)) {                                      // �Y���\Ū��
                vector<School> schoolTypes;                             // schoolTypes �ΨӦs�B�z�n����ơA�C����Ƭ�School����
                schoolTypes = dataProcess(lines);                       // �B�z�ɮ׸��

                for (int i = 0; i < schoolTypes.size(); i++) {          // �̧Ǵ��J�� DEAP
                    deap.insert(schoolTypes[i]);
                }

                deap.showNODES();                                       // �L�X�D�حn�D���T�����
            }
            cout << endl;
        }
        else if (choise == 3) {                                         // ���X top-k maximums from DEAP
            int deapSize = deap.getDeapSize();
            if (deapSize == -1) {                                       // �Y deap ���S�����
                cout << "No elements in DEAP, Please build DEAP first..." << endl;
            }
            else if (deapSize == 0) {                                   // ����X deap ���Ҧ����̤j�Ȥ���A�|�ѤU�@�� root
                cout << "No elements in DEAP, Please build DEAP first..." << endl;

            }
            else {
                int K = 0;                                              // ��J�n���X�X�� maximums
                cout << "Enter the value of K in [1," << deap.getDeapSize() << "]: ";
                cin >> K;
                cout << endl;
                if (1 > K || K > deap.getDeapSize()) {
                    cout << "Input out of range!" << endl;
                }
                else {
                    vector<School> maximums;
                    for (int i = 0; i < K; i++) {                           // �Q�� for �j��̧Ǩ��X maximum
                        deap.retrieveMax(maximums);
                    }

                    for (int i = 0; i < K; i++) {                           // �Q�� for �j��̧ǦL�X maximum
                        cout << "Top    " << i + 1 << ": [" << maximums[i].getId() << "] "
                            << maximums[i].getSchoolName() << maximums[i].getDepName() << ", "
                            << maximums[i].getType() << ", " << maximums[i].getLevel() << ", "
                            << maximums[i].getStudentNum() << endl;
                    }
                }
            }
            cout << endl;
        }
        else {
            cout << "Invalid input!" << endl;
        }
    } while (choise != 0);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
