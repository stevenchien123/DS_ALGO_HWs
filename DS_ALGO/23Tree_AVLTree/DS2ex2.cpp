// 第八組 11127121 鄧俊揚、10926125 簡紹軒
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

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

class tree23 {
private:
	int nodeNum = 0;
	struct nodeData {
		int k;   // the series
		int num; // students number
		nodeData(int ser, int data) {
			k = ser;
			num = data;
		}
	};
	struct node {
		vector<nodeData> smallData;
		vector<nodeData> midData; // temp data in order to split
		vector<nodeData> bigData;
		node* parent;
		node* left;
		node* mid;
		node* mid2;               // temp child to distribute node child, the key is between mid and right,the third biggest in four child
		node* right;

		node() {
			left = NULL;
			mid = NULL;
			mid2 = NULL;
			right = NULL;
			parent = NULL;
		}

		node(int ser, int data) {
			nodeData tempSmall(ser, data);
			smallData.push_back(tempSmall);
			parent = NULL;
			left = NULL;
			mid = NULL;
			mid2 = NULL;
			right = NULL;
		}

		node(vector<nodeData> newData) {
			smallData = newData;
			left = NULL;
			mid = NULL;
			mid2 = NULL;
			right = NULL;
			parent = NULL;
		}
		~node() {
			delete left;
			delete mid;
			delete mid2;
			delete right;
			left = NULL;
			right = NULL;
			mid = NULL;
			mid2 = NULL;
			parent = NULL;
			smallData.clear();
			bigData.clear();
		}

		bool isLeaf() {
			if (left == NULL && right == NULL && mid == NULL && mid2 == NULL) 	return true;
			else	return false;
		}

		bool isFull() {
			if (bigData.empty()) return false;
			else				return true;
		}
	};
	node* root;

public:
	~tree23() {
		delete root;
		nodeNum = 0;
		root = NULL;
	}
	tree23() {
		root = NULL;
	}


	vector<int> getRootSer() {
		vector<int> temp;
		for (int i = 0; i < root->smallData.size(); i++) {
			temp.push_back(root->smallData[i].k);
		}
		if (!root->bigData.empty()) {
			for (int i = 0; i < root->bigData.size(); i++) {
				temp.push_back(root->bigData[i].k);
			}
		}
		return temp;
	}

	void split(node* cur) {
		nodeNum++;
		// let this node midData push to his parent, andthis node split to two nodes
		if (cur == root) {
			nodeNum++;
			root = new node(cur->midData);            // create new root = midData
			root->left = new node(cur->smallData);    // distribute new root child
			root->left->parent = root;
			root->right = new node(cur->bigData);
			root->right->parent = root;
			cur->midData.clear();
			// distribute new root 
			root->left->left = cur->left;
			if (root->left->left) root->left->left->parent = root->left;
			root->left->right = cur->mid;
			if (root->left->right) root->left->right->parent = root->left;
			root->right->left = cur->mid2;
			if (root->right->left) root->right->left->parent = root->right;
			root->right->right = cur->right;
			if (root->right->right) root->right->right->parent = root->right;
			cur = NULL; // delete this node
			return;
		}

		if (!cur->parent->isFull())/*check parent is full or not, when it is not full*/ {
			insertData(cur->midData, cur->parent);			  // insert midData to parent ,let parent have two data.
			cur->midData.clear(); 							  // clear this data
			if (cur->parent->left == cur) /*cur is parent left child*/ {
				cur->parent->mid = new node(cur->bigData);    // split to mid
				cur->parent->mid->parent = cur->parent;       // update parent
				cur->bigData.clear();
				// distribute left and mid child
				cur->parent->mid->left = cur->mid2;
				if (cur->parent->mid->left) cur->parent->mid->left->parent = cur->parent->mid;
				cur->mid2 = NULL;
				cur->parent->mid->right = cur->right;
				if (cur->parent->mid->right) cur->parent->mid->right->parent = cur->parent->mid;
				cur->right = cur->mid;
				cur->mid = NULL;
				if (cur->left) cur->left->parent = cur;
				if (cur->right) cur->right->parent = cur;
			}
			else if (cur->parent->right == cur)/*cur is parent right child*/ {
				cur->parent->mid = new node(cur->smallData);    // split to mid
				cur->parent->mid->parent = cur->parent;         // update parent
				cur->smallData = cur->bigData;
				cur->bigData.clear();
				// distribute left and mid child
				cur->parent->mid->left = cur->left;
				if (cur->parent->mid->left) cur->parent->mid->left->parent = cur->parent->mid;
				cur->parent->mid->right = cur->mid;
				if (cur->parent->mid->right) cur->parent->mid->right->parent = cur->parent->mid;
				cur->left = cur->mid2;
				cur->mid2 = NULL;
				cur->mid = NULL;
				if (cur->left) cur->left->parent = cur;
				if (cur->right) cur->right->parent = cur;
			}
		}
		else/*if parent is FULL*/ {
			insertData(cur->midData, cur->parent);   //insert cur->midData and sort data in parent
			cur->midData.clear();                    //clear this data
			node* temp;
			if (cur->parent->left == cur) /* cur is its parent left child */ {
				// distribute left to left and mid, and move mid to mid2
				temp = new node(cur->bigData);
				cur->parent->mid2 = cur->parent->mid;
				if (cur->parent->mid2) cur->parent->mid2->parent = cur->parent;
				cur->parent->mid = temp;
				if (cur->parent->mid) cur->parent->mid->parent = cur->parent;
				cur->bigData.clear();
				// distribute mid2 and right child
				cur->parent->mid->left = cur->mid2;
				if (cur->parent->mid->left) cur->parent->mid->left->parent = cur->parent->mid;
				cur->parent->mid->right = cur->right;
				if (cur->parent->mid->right) cur->parent->mid->right->parent = cur->parent->mid;
				cur->right = cur->mid;
				cur->mid = NULL;
				cur->mid2 = NULL;
			}
			else if (cur->parent->right == cur) /* cur is its parent right child */ {
				// distribute right to mid2 and right
				temp = new node(cur->smallData);
				cur->parent->mid2 = temp;
				if (cur->parent->mid2) cur->parent->mid2->parent = cur->parent;
				cur->smallData = cur->bigData;
				cur->bigData.clear();
				// distribute mid2 and right child
				cur->parent->mid2->left = cur->left;
				if (cur->parent->mid2->left) cur->parent->mid2->left->parent = cur->parent->mid2;
				cur->parent->mid2->right = cur->mid;
				if (cur->parent->mid2->right) cur->parent->mid2->right->parent = cur->parent->mid2;
				cur->left = cur->mid2;
				cur->mid = NULL;
				cur->mid2 = NULL;
			}
			else /* cur is its parent mid child */ {
				// distribute mid to mid and mid2
				temp = new node(cur->bigData);
				cur->parent->mid2 = temp;
				if (cur->parent->mid2) cur->parent->mid2->parent = cur->parent;
				cur->bigData.clear();
				// distribute mid and mid2 child
				cur->parent->mid2->left = cur->mid2;
				if (cur->parent->mid2->left) cur->parent->mid2->left->parent = cur->parent->mid2;
				cur->parent->mid2->right = cur->right;
				if (cur->parent->mid2->right) cur->parent->mid2->right->parent = cur->parent->mid2;
				cur->right = cur->mid;
				cur->mid = NULL;
				cur->mid2 = NULL;
			}
			split(cur->parent);   // recusion when it parent is full
		}
	}

	void insertData(nodeData newData, node* cur) {
		// just like void insertData( vector<nodeData> newData, node* cur), but it sort by nodeData, not only one value.
		if (cur->bigData.empty()) {
			cur->bigData.push_back(newData);
			if (cur->smallData[0].num > cur->bigData[0].num) {
				swap(cur->smallData, cur->bigData);
			}
		}
		else {
			cur->midData.push_back(newData);
			if (cur->smallData[0].num > cur->bigData[0].num) {
				swap(cur->smallData, cur->bigData);
			}
			if (cur->midData[0].num > cur->bigData[0].num) {
				swap(cur->midData, cur->bigData);
			}
			if (cur->smallData[0].num > cur->midData[0].num) {
				swap(cur->smallData, cur->midData);
			}
		}
	}

	void insertData(vector<nodeData> newData, node* cur) {		// insert data in the node and sort
		if (cur->bigData.empty()) {		// only one data in this node
			cur->bigData = newData;      // insert to bigData
			//sort smallData and bigData
			if (cur->smallData[0].num > cur->bigData[0].num) {
				swap(cur->smallData, cur->bigData);
			}
		}
		else {
			cur->midData = newData;     // insert data to midData
			// sort smallData and midData and bigData
			if (cur->smallData[0].num > cur->bigData[0].num) {
				swap(cur->smallData, cur->bigData);
			}
			if (cur->midData[0].num > cur->bigData[0].num) {
				swap(cur->midData, cur->bigData);
			}
			if (cur->smallData[0].num > cur->midData[0].num) {
				swap(cur->smallData, cur->midData);
			}
		}
	}

	void insertNode(int ser, int data, node* cur) {	// to insert a new node in the tree
		if (root == NULL) {
			root = new node(ser, data);
			nodeNum++;
			return;
		}
		else {
			if (!cur->smallData.empty() && cur->smallData[0].num == data) {
				nodeData temp(ser, data);
				cur->smallData.push_back(temp);
				return;
			}
			if (!cur->bigData.empty() && cur->bigData[0].num == data) {
				nodeData temp(ser, data);
				cur->bigData.push_back(temp);
				return;
			}

			if (cur->isLeaf()) {
				// only one key, could add key in this node
				nodeData temp(ser, data);
				insertData(temp, cur);
				if (!cur->midData.empty())split(cur);
				return;
			}
			else {
				if (cur->smallData[0].num > data)
					insertNode(ser, data, cur->left);
				else if ((cur->bigData.empty() && cur->smallData[0].num < data) || (!cur->bigData.empty() && cur->bigData[0].num < data))
					insertNode(ser, data, cur->right);
				else if (cur->smallData[0].num < data && (!cur->bigData.empty() && cur->bigData[0].num > data))
					insertNode(ser, data, cur->mid);
			}
		}
	}

	void buildTree(vector<string> initdata) {
		for (int i = 0; i < initdata.size(); i++) {
			insertNode(i + 1, strToi(initdata[i]), root);    // one by one insert to 2-3tree
		}
		node* heightPath = root;
		int height = 0;
		while (heightPath != NULL)/* to count the tree height */ {
			heightPath = heightPath->left; // because the 2-3 tree every length from root to leet is same, so only search one.
			height++;
		}
		cout << "Tree height = " << height << endl;
		cout << "Number of nodes = " << nodeNum << endl;
	}

	bool isNull() {
		return root == NULL;
	}

	vector<int> printsmallNode(node* cur, int& k) {
		vector<int> result;
		for (int i = 0; i < k && i < cur->smallData.size(); i++) {
			result.push_back(cur->smallData[i].k);
		}
		k = k - result.size();
		return result;
	}

	vector<int> printBigNode(node* cur, int& k) {
		vector<int> result;
		for (int i = 0; i < k && i < cur->bigData.size(); i++) {
			result.push_back(cur->bigData[i].k);
		}
		k = k - result.size();
		return result;
	}

	vector<int> searchKBig(node* cur, int& k, vector<int>& result) {
		// print complete AVLtree by prefix (for debug)
		if (cur != NULL && k > 0) {
			searchKBig(cur->right, k, result);
			if (!cur->bigData.empty()) {
				vector<int> curBig = printBigNode(cur, k);
				result.insert(result.end(), curBig.begin(), curBig.end());
				searchKBig(cur->mid, k, result);
			}
			vector<int> curSmall;
			curSmall = printsmallNode(cur, k);
			result.insert(result.end(), curSmall.begin(), curSmall.end());
			searchKBig(cur->left, k, result);

		}
		return result;
	}

	node* getRoot() {
		return root;
	}
};



class AVLtree {
private:
	struct AVLnode
	{
		vector<int> index;      // the series of data
		int key;      			// the data ()
		AVLnode* left;
		AVLnode* right;
		AVLnode(int ser, int number) {
			index.push_back(ser);
			key = number;
			left = NULL;
			right = NULL;
		}
		~AVLnode() {
			index.clear();
			key = 0;
			delete left;
			delete right;
		}
	};
	AVLnode* root;
	int nodeNum;

public:

	~AVLtree() {
		delete root;
		nodeNum = 0;
		root = NULL;
	}

	AVLtree() {
		nodeNum = 0;
		root = NULL;
	}

	bool isNull() {
		return root == NULL;
	}

	int getHeight(AVLnode* node) {
		if (node == NULL) {
			return 0; // if NULL , Height =0
		}
		int leftHeight = getHeight(node->left);
		int rightHeight = getHeight(node->right);
		if (leftHeight > rightHeight) return 1 + leftHeight;  // compare leftsubtree is bigger or not. If true, the height is leftsubtree Height+1
		else return 1 + rightHeight;                      // If false, the Height is rightsubtree Height + 1;
	}

	AVLnode* rotateLeft(AVLnode* x) {
		// from class slide, it can left node left rotate
		AVLnode* y = x->right;       // y will replace x to this node,the y will be x->right first(to left rotate).
		x->right = y->left;          // Second,break x and y connect, and connect y->left to x->right(it will be y->left->right later)
		y->left = x;               // connect y->left to new x(have y->left)
		if (x == root)	root = y;    // if cur is root, refresh root
		return y;
	}

	AVLnode* rotateRight(AVLnode* x) {
		// from class slide, it can left node left rotate
		AVLnode* y = x->left;        // y will replace x to this node,the y will be x->left first(to right rotate).
		x->left = y->right;          // Second,break x and y connect, and connect y->right to x->left(it will be y->right->left later)
		y->right = x;                // connect y->right to new x(have y->right)
		if (x == root) root = y;    // if cur is root, refresh root
		return y;
	}

	int getBalanceFactor(AVLnode* node) {
		// to calculate the two subtree is balance or not
		// but I don't use this function ,because it broken easily
		if (node == NULL) {
			return 0;
		}
		return getHeight(node->left) - getHeight(node->right);
	}

	AVLnode* insertData(int ser, int data, AVLnode* cur) {
		if (cur == NULL) {
			// when cur go to NULL node ,it can insert new node on here.
			nodeNum++;
			return new AVLnode(ser, data);
		}
		else {
			if (cur->key > data)/* move to left node*/ {
				cur->left = insertData(ser, data, cur->left);
			}
			else if (cur->key < data)/* move to right node*/ {
				cur->right = insertData(ser, data, cur->right);
			}
			else /* data is as same as key,push ser to this node index.*/ {
				cur->index.push_back(ser);
				return cur;
			}

			if (getHeight(cur->left) - getHeight(cur->right) > 1 && getHeight(cur->left->left) - getHeight(cur->left->right) > 0) {
				// cur's left subtree is bigger than right. and cur->left's left subtree also bigger than right.
				return rotateRight(cur);    // need to rotate right
			}
			if (getHeight(cur->left) - getHeight(cur->right) < -1 && getHeight(cur->right->left) - getHeight(cur->right->right) < 0) {
				// cur's left subtree is smaller than right. and cur->right's left subtree also smaller than right.
				return rotateLeft(cur);    // need to rotate left
			}
			if (getHeight(cur->left) - getHeight(cur->right) > 1 && getHeight(cur->left->left) - getHeight(cur->left->right) < 0) {
				// cur's left subtree is bigger than right. but cur->left's left subtree smaller than right.
				cur->left = rotateLeft(cur->left);   // first let cur->left rotate left
				return rotateRight(cur);             // and make the  new cur rotate right
			}
			if (getHeight(cur->left) - getHeight(cur->right) < -1 && getHeight(cur->right->left) - getHeight(cur->right->right) > 0) {
				// cur's left subtree is smaller than right. but cur->right's left subtree bigger than right.
				cur->right = rotateRight(cur->right);// first let cur->right rotate right
				return rotateLeft(cur);              // and make the new cur rotate left
			}
			return cur;  // return cur to previous recusion, to update the new node.
		}
	}

	void build(vector<vector<string>> initData) {
		for (int i = 0; i < initData.size(); i++) {
			root = insertData(i + 1, strToi(initData[i][4]), root); // one by one insert to AVLtree.
		}
		cout << "Tree height = " << getHeight(root) << endl;
		cout << "Number of nodes = " << nodeNum << endl;


	}

	vector<int> getRootSer() {
		return root->index;   // the root of (vector<int> index), store the node have data
	}

	vector<int> printNode(AVLnode* cur, int& k) {
		vector<int> result;
		for (int i = 0; i < k && i < cur->index.size(); i++) {
			result.push_back(cur->index[i]);
		}
		k = k - result.size();
		return result;
	}

	vector<int> searchKBig(AVLnode* cur, int& k, vector<int>& result) {
		// print complete AVLtree by prefix (for debug)
		if (cur != NULL && k > 0) {
			searchKBig(cur->right, k, result);
			vector<int> curNode = printNode(cur, k);
			result.insert(result.end(), curNode.begin(), curNode.end());
			searchKBig(cur->left, k, result);

		}
		return result;
	}

	vector<string> storeAVLData() {
		vector<string> temp;
		int height = getHeight(root);
		temp.push_back("Tree height = " + std::to_string(height));
		temp.push_back("Number of nodes = " + std::to_string(nodeNum));
		return temp;
	}


	AVLnode* getRoot() {
		return root;
	}
	int getNodeNum() {
		return nodeNum;
	}


};

bool inputData(string fileNum, vector<string>& Data, vector< vector<string>>& detail) {
	for (int i = 0; i < detail.size(); i++) {
		detail[i].clear();
	}
	detail.clear();
	Data.clear();
	fstream file;
	string fileName = "input" + fileNum + ".txt";
	file.open(fileName.c_str());
	if (!file.is_open()) /* Check whether the file is exists */ {
		cout << "\n### input" << fileNum << ".txt does not exist! ###" << endl;
		return false;
	}
	bool noData = true;
	string label;
	for (int i = 0; i < 3; i++)
		getline(file, label); // Get the labels, there have three lines labels to get first
	string info;                // the string from each input
	string infoData;
	vector<string> detailLine;
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
			if (dataNum == 8) {
				Data.push_back(infoData);
			}
			if (dataNum == 1 || dataNum == 3 || dataNum == 4 || dataNum == 5 || dataNum == 6 || dataNum == 8) {
				detailLine.push_back(infoData);
			}


			dataNum += 1;
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

void printRoot(vector<int> rootSer, vector<vector< string >> detail) {
	// two mission need print root content, and I store data in tree is not complete, so I use index to search from all Data.
	for (int i = 0; i < rootSer.size(); i++) {
		cout << i + 1 << ": [" << rootSer[i] << "] ";
		for (int j = 0; j < detail[rootSer[i] - 1].size() - 1; j++) {
			cout << detail[rootSer[i] - 1][j] << " ,";
		}
		cout << detail[rootSer[i] - 1][detail[rootSer[i] - 1].size() - 1] << endl;
	}
	cout << endl;
}

int main() {
	string command;
	vector<vector< string >> detail;
	vector<string> data;
	vector<string> stroreAVL;
	vector<int> storeAVLRoot;
	AVLtree tree_AVL;
	tree23 tree_23;


	do {
		cout << "\n**** Balanced Search Trees ****";
		cout << "\n* 0. QUIT                     *";
		cout << "\n* 1. Build 23 tree            *";
		cout << "\n* 2. Build AVL tree           *";
		cout << "\n* 3. Top-K search on 23 tree  *";
		cout << "\n* 4. Top-K search on AVL tree *";
		cout << "\n*******************************";
		cout << "\nInput a choice(0, 1, 2, 3, 4): ";
		cin >> command;

		if (command == "1") {
			string filename;
			do {
				cout << endl << "Input a file number ([0] Quit): ";
				cin >> filename;

			} while (filename != "0" && !inputData(filename, data, detail));

			if (filename != "0") {
				tree_23.~tree23();
				tree_AVL.~AVLtree();
				tree_23.buildTree(data);
				printRoot(tree_23.getRootSer(), detail);
			}
			else {
				tree_23.~tree23();
			}
		}
		else if (command == "2") {
			if (!tree_23.isNull() && tree_AVL.isNull()) {
				tree_AVL.build(detail);
				printRoot(tree_AVL.getRootSer(), detail);
				stroreAVL = tree_AVL.storeAVLData();
				storeAVLRoot = tree_AVL.getRootSer();
			}
			else if (!tree_AVL.isNull()) {
				cout << "### AVL tree has been built. ###\n";
				for (int i = 0; i < stroreAVL.size(); i++) {
					cout << stroreAVL[i] << endl;
				}
				printRoot(storeAVLRoot, detail);
			}
			else {
				cout << "### Choose 1 first. ###\n";
			}
		}
		else if (command == "3") {
			if (tree_23.isNull()) {
				cout << "### Choose 1 first. ###\n";
			}
			else {
				int k = 0;
				cout << "\nEnter K in [1," << detail.size() << "]:";
				cin >> k;
				if (1 <= k && k <= detail.size()) {
					vector<int> temp;
					printRoot(tree_23.searchKBig(tree_23.getRoot(), k, temp), detail);
				}
			}
		}
		else if (command == "4") {
			if (tree_AVL.isNull()) {
				if (tree_23.isNull()) cout << "### Choose 1 first. ###\n";
				else cout << "### Choose 2 first. ###\n";
			}
			else {
				int k = 0;
				cout << "\nEnter K in [1," << detail.size() << "]:";
				cin >> k;
				if (1 <= k && k <= detail.size()) {
					vector<int> temp;
					printRoot(tree_AVL.searchKBig(tree_AVL.getRoot(), k, temp), detail);
				}
			}
		}
		else if (command > "4" || command < "0") {
			cout << "\nCommand does not exist!\n";
		}
	} while (command != "0");

	cout << "程式已退出" << endl;
	return 0;
}