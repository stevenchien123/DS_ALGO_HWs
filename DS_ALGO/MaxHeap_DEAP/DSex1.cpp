// 學號: 10926125，姓名: 簡紹軒
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
using namespace std;

//****************************************************** start School ******************************************************
class School {                                                         // 學校資料
private:
    int id;                                                            // 唯一序號
    //string schoolId;                                                   // 學校代碼
    string schoolName;                                                 // 學校名稱
    //string depId;                                                      // 科系代碼
    string depName;                                                    // 科系名稱
    string type;                                                       // 日夜別
    string level;                                                      // 等級別
    int studentNum;                                                    // 此資料之學生數

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
    void insert(School node);               // 插入 max heap
    void ReHeapUp();                        // 調整 heap 的結構，使之成為 heap
    void showNODES();                       // 印出題目要求的三筆資料 (root, last node, leftmost node)
    bool isEmpty();                         // 判斷 max heap 是否為空
    void clearUp();                         // 清空 heap 的資料
    int treeHeight(int idx);                // 找 idx 的樹高，tree height of root = 0
    int parent(int idx);                    // 找 idx 的 parent index
};

void MaxHeap::clearUp() {
    maxHeap.clear();
}

int MaxHeap::treeHeight(int idx) {          // 找 idx 的樹高，tree height of root = 0
    return floor(log2(idx + 1));
}

int MaxHeap::parent(int idx) {              // 找 idx 的 parent index
    return ceil((double)idx / 2) - 1;
}

bool MaxHeap::isEmpty() {
    return maxHeap.size() == 0 ? true : false;
}

void MaxHeap::ReHeapUp() {
    if (isEmpty()) {                                                           // 若為空，則印出錯誤訊息
        cout << "ReHeapUp error: "
            << "Please insert node first." << endl;
        return;
    }

    int idx = maxHeap.size() - 1;                                              // index of the last node
    int p = parent(idx);                                                       // parent index of the last node

    while (idx > 0) {                                                          // 調整 heap 直到 idx 到 root 為止
        if (maxHeap[idx].getStudentNum() <= maxHeap[p].getStudentNum()) {      // 若沒有比 parent 的值還大
            return;                                                            // 則結束
        }
        swap(maxHeap[idx], maxHeap[p]);                                        // 反之，則交換位置
        idx = p;                                                               // 更新 index
        p = parent(idx);
    }
}

void MaxHeap::insert(School node) {
    maxHeap.push_back(node);
    ReHeapUp();                                      // insert 之後要調整 heap 的結構
}

void MaxHeap::showNODES() {
    cout << "\n<Max Heap>" << endl;

    if (isEmpty()) {                                                                           // 若 max heap 為空
        cout << "No element yet." << endl;                                                     // 印出錯誤信息後 return
        return;
    }

    cout << "root: " << "[" << maxHeap[0].getId() << "] "                                      // 印出 root
        << maxHeap[0].getStudentNum() << endl;

    cout << "bottom: " << "[" << maxHeap[maxHeap.size() - 1].getId() << "] "                   // 印出 heap 中最後一個 node
        << maxHeap[maxHeap.size() - 1].getStudentNum() << endl;

                                                                                               // 印出 leftmost node in max heap
    if (maxHeap.size() == 1) {                                                                 // 若 heap 中只有一個 node
        cout << "leftmost bottom: " << "[" << maxHeap[0].getId() << "] "                       // 印出 root
            << maxHeap[0].getStudentNum() << endl;
    }

    int idx = 0;                                                                               // 否則不斷往左子點走
    while (idx * 2 + 1 <= maxHeap.size() - 1) {                                                // 直到超出 max heap 範圍為止
        idx = idx * 2 + 1;                                                                     // idx 為 leftmost node 的索引
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
    int getDeapSize();                      // 取得 deap 的大小
    void insert(School node);               // 插入 deap
    void retrieveMax(vector<School>& maximums);                     // 取出 maximum
    void Restructure();                     // 調整 deap 的結構，使之成為 deap
    void showNODES();                       // 印出題目要求的兩筆資料 (last node, leftmost node)
    bool isEmpty();                         // 判斷 deap 是否為空
    void clearUp();                         // 清空 deap 的資料
    int inMinorMax(int idx);                // 判斷 idx 的位置在 min 還是 max heap
    void ReHeapUpMax(int idx);              // 從 idx 由下到上調整 max heap 的結構
    void ReHeapDownMax(int idx);            // 從 idx 由上到下調整 max heap 的結構
    void ReHeapUpMin(int idx);              // 從 idx 由下到上調整 min heap 的結構
    int treeHeight(int idx);                // 找 idx 的樹高，tree height of root = 0
    int parent(int idx);                    // 找 idx 的 parent index
    int maxChild(int idx);                  // 找 idx 較大的 child
    int minChild(int idx);                  // 找 idx 較小的 child
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

    if (idx2 > deap.size() - 1) {                                               // 若 idx2 超出 deap 的範圍
        if (idx1 > deap.size() - 1) {                                               // 且 idx1 也超出範圍
            return -1;                                                              // return -1 表示 idx 沒有 child
        }
        else {                                                                      // 但 idx1 沒有超出範圍
            return idx1;                                                            // 代表 idx 只有 idx1 一個 child，直接 return
        }
    }

    if (deap[idx1].getStudentNum() > deap[idx2].getStudentNum()) {              // idx1 和 idx2 都在範圍內
        return idx1;
    }
    return idx2;
}

int DEAP::treeHeight(int idx) {                                  // 找 idx 的樹高，tree height of root = 0
    return floor(log2(idx + 1));
}

int DEAP::parent(int idx) {                                      // 找 idx 的 parent index
    return ceil((double)idx / 2) - 1;
}

bool DEAP::isEmpty() {
    return deap.size() == 1 ? true : false;
}

int DEAP::inMinorMax(int idx) {
    while (idx != 1 && idx != 2) {                                              // 不斷往 parent 跑，直到 index 為 1 或 2
        idx = parent(idx);
    }

    if (idx == 1) return 1;                                                     // 若 idx 為 1: 在 min heap，反之則在 max heap
    else return 2;                                                              // 因為 min heap 在左子樹，而左子樹的 root index 為 1，同理 max heap
}

void DEAP::ReHeapDownMax(int idx) {
    if (idx > deap.size() - 1) {                                                // deap 中的 max heap 沒有資料了
        return;
    }

    int child = maxChild(idx);                                                  // 找出 idx 的最大 child

    while (child != -1) {                                                       // 當 idx 的 child 存在 (idx 不是 leaf)
        if (deap[child].getStudentNum() > deap[idx].getStudentNum()) {          // 若 child 比 idx 的值還大
            swap(deap[child], deap[idx]);                                       // 兩者位置交換
            idx = child;                                                        // idx 往下跑
            child = maxChild(idx);                                              // 更新 child idex
        }
        else {                                                                  // 若 child 比 idx 的值還小
            return;                                                             // 調整結束，直接 return
        }
    }

    int y = idx - pow(2, treeHeight(idx) - 1);                                  // y 為 idx 相對應的 node index
    int yChild = maxChild(y);                                                   // yChild 為 y 最大的 child

    if (yChild != -1) {                                                         // 當 yChild 存在 (考慮 special case)
        if (deap[yChild].getStudentNum() < deap[idx].getStudentNum()) {         // idx 比 yChild 的值還大
            return;                                                             // 調整結束，直接 return
        }
        else {                                                                  // idx 比 yChild 的值還小 (進入 special case)
            swap(deap[yChild], deap[idx]);                                      // 兩者交換位置
            ReHeapUpMin(yChild);                                                // 在 yChild 的地方調整 min heap
        }
    }
    else {                                                                      // 當 yChild 不存在
        if (deap[y].getStudentNum() < deap[idx].getStudentNum()) {              // idx 比 y 的值還大
            return;
        }
        else {                                                                  // idx 比 y 的值還小
            swap(deap[y], deap[idx]);                                           // 兩者交換位置
            ReHeapUpMin(y);                                                     // 在 y 的地方調整 min heap
        }
    }
}

void DEAP::ReHeapUpMax(int idx) {
    if (isEmpty()) {                                                           // 若為空，則印出錯誤訊息
        cout << "ReHeapUp error: "
            << "Please insert node first." << endl;
        return;
    }

    int p = parent(idx);                                                       // parent index of the last node

    while (idx > 0 && p != 0) {                                                // 調整 heap 直到 idx 到 root 為止
                                                                               // 或 parent index 為 0 為止 (index 0 不存值)
        if (deap[idx].getStudentNum() <= deap[p].getStudentNum()) {            // 若沒有比 parent 的值還大
            return;                                                            // 則結束
        }
        swap(deap[idx], deap[p]);                                              // 反之，則交換位置
        idx = p;                                                               // 更新 index
        p = parent(idx);
    }
}

void DEAP::ReHeapUpMin(int idx) {
    if (isEmpty()) {                                                           // 若為空，則印出錯誤訊息
        cout << "ReHeapUp error: "
            << "Please insert node first." << endl;
        return;
    }

    int p = parent(idx);                                                       // parent index of the last node

    while (idx > 0 && p != 0) {                                                // 調整 heap 直到 idx 到 root 為止
                                                                               // 或 parent index 為 0 為止 (index 0 不存值)
        if (deap[idx].getStudentNum() >= deap[p].getStudentNum()) {            // 若沒有比 parent 的值還小
            return;                                                            // 則結束
        }
        swap(deap[idx], deap[p]);                                              // 反之，則交換位置
        idx = p;                                                               // 更新 index
        p = parent(idx);
    }
}

void DEAP::Restructure() {
    int idx = deap.size() - 1;                                                  // index of the last node
    if (idx == 1) {                                                             // 插入第一個 node 不用調整結構
        return;                                                                 // 所以直接 return
    }

    int check = inMinorMax(idx);                                                // check if the last node in min or max heap, 1:min, 2:max
    if (check == 1) {                                                           // 若 idx 在 min heap
        int tmp = idx + pow(2, treeHeight(idx) - 1);
        int y = parent(tmp);                                                    // y 為 idx 相對應的 node index

        if (deap[idx].getStudentNum() > deap[y].getStudentNum()) {              // idx 比相對應的 node 還大
            swap(deap[idx], deap[y]);                                           // 交換位置
            ReHeapUpMax(y);                                                     // 調整 max heap 的結構
        }
        else {
            ReHeapUpMin(idx);                                                   // 調整 min heap 的結構
        }
    }
    else {                                                                      // 若 idx 在 max heap
        int y = idx - pow(2, treeHeight(idx) - 1);                              // y 為 idx 相對應的 node index

        if (deap[idx].getStudentNum() < deap[y].getStudentNum()) {              // idx 比相對應的 node 還小
            swap(deap[idx], deap[y]);                                           // 交換位置
            ReHeapUpMin(y);                                                     // 調整 min heap 的結構
        }
        else {
            ReHeapUpMax(idx);                                                   // 調整 max heap 的結構
        }
    }
}

void DEAP::insert(School node) {
    if (deap.size() == 0) {                 // 若 deap 中沒有 node
        School root;                        // 則新增一個空的 node 進去
        deap.push_back(root);
    }
    deap.push_back(node);
    Restructure();                          // insert 後要調整 deap 的結構
}

void DEAP::retrieveMax(vector<School>& maximums) {              // maximums 用來存 取出的最大節點
    int maximum = 0;

    if (isEmpty()) {                        // deap 為空
        cout << "No elements in DEAP yet." << endl;
        return;
    }
    if (deap.size() - 1 == 1) {                 // deap 不為空且 max heap 中沒有資料 (代表只有 min heap 有資料)
        maximums.push_back(deap[1]);            // 將資料插入 maximums
        deap.pop_back();                        // 移除資料
        return;                                 // 直接 return，此時 deap 為空
    }
                                                        // deap 不為空且 max heap 中有資料
    maximums.push_back(deap[2]);                        // 將最大值插入 maximums
    swap(deap[2], deap[deap.size() - 1]);               // maximum node 和 last node 的位置交換
    deap.pop_back();                                    // 刪除 deap 的 maximum

    ReHeapDownMax(2);                                   // 從原本 maximum 的位置開始由上到下調整 deap 的結構
}

void DEAP::showNODES() {
    cout << "\n<DEAP>" << endl;

    if (isEmpty()) {                                                                        // 若 deap 為空
        cout << "No element yet." << endl;                                                  // 印出錯誤信息後 return
        return;
    }

    cout << "bottom: " << "[" << deap[deap.size() - 1].getId() << "] "                      // 印出 deap 中最後一個 node
        << deap[deap.size() - 1].getStudentNum() << endl;

                                                                                            // 印出 leftmost node in deap
    int idx = 0;                                                                            // 不斷往左子點走
    while (idx * 2 + 1 <= deap.size() - 1) {                                                // 直到超出 max heap 範圍為止
        idx = idx * 2 + 1;                                                                  // idx 為 leftmost node 的索引
    }
    cout << "leftmost bottom: " << "[" << deap[idx].getId() << "] "
        << deap[idx].getStudentNum() << endl;
}
//****************************************************** end DEAP ******************************************************



bool readFile(vector<string>& lines) {                                 // 讀檔，存入lines中
    int fileNum = -1;
    cout << "Input the file number([0]: Quit): ";
    cin >> fileNum;

    if(fileNum == 0) exit(EXIT_SUCCESS);                               // 輸入 0 則結束程序

    ifstream ifs("input" + to_string(fileNum) + ".txt", ios::in);
    if (!ifs.is_open()) {                                              // 若 fileNum.txt 不在此資料夾
        cout << "Failed to open file." << endl;                        // print 錯誤訊息後 return
        cout << endl;
        return false;
    }

    string s;                                                          // 檔案開啟成功
    getline(ifs, s);
    getline(ifs, s);
    getline(ifs, s);
    getline(ifs, s);                                                   // 直接用 g++ 編譯的話第一行讀到的會是空字串
                                                                       // 第二行開始才是檔案的第一行資料

    while (getline(ifs, s)) {                                          // 從第四行開始處理資料，讀取每一行直到沒有下一行為止
        lines.push_back(s);                                            // 將每行資料存起來
    }
    return true;
}

int splitStdNum(string token) {
    if (token.empty()) return 0;                               // 若學生數欄位為空，視為 0 個學生

    /*
    for (int i = 0; i < token.length(); i++) {                 // 學生數若為個位數，ex: 2，
        if (token[i] == '0') continue;                         // 則資料會表示成 02
        token = token.substr(i, token.length());               // 所以將前面的 0 去掉，才能正常的轉 int
        break;
    }
    */
    
    if (token.find(",") != string::npos) {                     // 因為學生數採用千位數格式，ex: 1,000
        size_t found = token.find(",");                        // 而1,000不能直接轉int，會報錯
        string temp1 = token.substr(0, found);                 // 所以再切一次字串，只保留數字部分
        string temp2 = token.substr(found + 1, token.length());
        token = temp1 + temp2;
    }
    //cout << token << endl;
    return stoi(token);
}

vector<School> dataProcess(vector<string>& lines) {                    // 處理檔案資料
    vector<School> schoolTypes;                                        // 將處理完的資料存入 vector
    int i = 0;                                                         // i 代表唯一序號

    for (string s : lines) {                                           // 對每行資料做處理
        School school;                                                 // 每行都創建一個物件來存資料
        i++;
        std::istringstream iss(s);
        std::string token;
        int j = 0;                                                     // 以 j 當作 iterator
        while (std::getline(iss, token, '\t')) {                       // 提取出每行的學生數
            switch (j)
            {
            case 1:                                                    // 第一欄為學校名稱
                school.setSchoolName(token);
                //cout << token << " ";
                break;
            case 3:                                                    // 第三欄為科系名稱
                school.setDepName(token);
                //cout << token << " ";
                break;
            case 4:                                                     // 第四欄為日夜別
                school.setType(token);
                //cout << token << " ";
                break;
            case 5:                                                     // 第五欄為等級別
                school.setLevel(token);
                //cout << token << " ";
                break;
            case 6:
            {
                int stdNum = splitStdNum(token);                       // 第六欄為學生數
                school.setStudentNum(stdNum);                          // stdNum 為學生數，將 token 轉成整數後存入
                //cout << stdNum << " ";
                break;
            }
            default:
                break;
            }
            j++;
        }
        school.setId(i);                                           // 設定唯一序號
        //cout << i << endl;
        /*
        if (j != 6) {                                              // 學生數在第六個欄位，且每個欄位用\t隔開
            j++;                                                   // 所以把第六個欄位提取出來處理
            continue;                                              // token 為學生數的字串形式
        }
        */
        // cout << stdNum << endl;
        schoolTypes.push_back(school);                             // 將處理完的資料 push 進另外一個 vector 中
    }
    return schoolTypes;
}

int main()
{
    int choise = 0;
    vector<string> lines;                                       // lines 保存檔案各行資料
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

        cin >> choise;                                                  // 輸入要做哪個選項
        cout << endl;

        lines.clear();                                                  // 清空 lines
        if (choise == 0) {                                               // 0: QUIT
            return 0;
        }
        else if (choise == 1) {                                         // 1: Build a max heap
            
            maxHeap.clearUp();                                          // 清空 max heap

            if (readFile(lines)) {                                      // 若成功讀檔
                vector<School> schoolTypes;                             // schoolTypes 用來存處理好的資料，每筆資料為School成員
                schoolTypes = dataProcess(lines);                       // 處理檔案資料

                for (int i = 0; i < schoolTypes.size(); i++) {          // 依序插入至 max heap
                    maxHeap.insert(schoolTypes[i]);
                }

                maxHeap.showNODES();                                    // 印出題目要求的三筆資料
            }
            cout << endl;
        }
        else if (choise == 2) {                                         // 2: Build a DEAP
            
            deap.clearUp();                                             // 清空 deap

            if (readFile(lines)) {                                      // 若成功讀檔
                vector<School> schoolTypes;                             // schoolTypes 用來存處理好的資料，每筆資料為School成員
                schoolTypes = dataProcess(lines);                       // 處理檔案資料

                for (int i = 0; i < schoolTypes.size(); i++) {          // 依序插入至 DEAP
                    deap.insert(schoolTypes[i]);
                }

                deap.showNODES();                                       // 印出題目要求的三筆資料
            }
            cout << endl;
        }
        else if (choise == 3) {                                         // 取出 top-k maximums from DEAP
            int deapSize = deap.getDeapSize();
            if (deapSize == -1) {                                       // 若 deap 中沒有資料
                cout << "No elements in DEAP, Please build DEAP first..." << endl;
            }
            else if (deapSize == 0) {                                   // 當取出 deap 中所有的最大值之後，會剩下一個 root
                cout << "No elements in DEAP, Please build DEAP first..." << endl;

            }
            else {
                int K = 0;                                              // 輸入要取出幾個 maximums
                cout << "Enter the value of K in [1," << deap.getDeapSize() << "]: ";
                cin >> K;
                cout << endl;
                if (1 > K || K > deap.getDeapSize()) {
                    cout << "Input out of range!" << endl;
                }
                else {
                    vector<School> maximums;
                    for (int i = 0; i < K; i++) {                           // 利用 for 迴圈依序取出 maximum
                        deap.retrieveMax(maximums);
                    }

                    for (int i = 0; i < K; i++) {                           // 利用 for 迴圈依序印出 maximum
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
