#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Node class taken directly from Aman's lecture/Stepik 5.2.2 and edited to include a string
//Also edited naming to be more applicable to the project 
class Node {
public:
	int id, height;
	Node* left, * right;
	string studentName;
	Node(string x, int y) : studentName(x), id(y), height(0), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
	Node* node;
	int numNodes;
public:
	AVLTree();
	int height(Node* node);
	void insert(string studentName, int id);
	Node* insertRecursive(Node* node, string studentName, int id);
	Node* balance(Node* node);
	int calculateBalanceFactor(Node* node);
	void remove(int id);
	Node* removeRecursive(int id, Node* node, bool& isSuccessful);
	void search(int id);
	void searchRecursive(int id, Node* node, bool& isFound);
	void search(string studentName);
	void searchRecursive(string name, Node* node, vector<string>& matches);
	void printOrder(string order);
	string printInorderRecursive(Node* node);
	string printPreorderRecursive(Node* node);
	string printPostorderRecursive(Node* node);
	void printHeight();
	void removeInorder(int index);
	Node* removeInorderRecursive(Node* node, int index, int& counter);
	Node* rotateLeft(Node* node);
	Node* rotateRight(Node* node);
	Node* rotateLeftRight(Node* node);
	Node* rotateRightLeft(Node* node);
};

//Default constructor, setting node to null as default and setting the number of nodes to 0
AVLTree::AVLTree() {
	node = nullptr;
	numNodes = 0;
}

//Finds height of a node
//Taken from Stepik 5.2.1 (which implemented Professor Aman's tree lecture, slide #16)
int AVLTree::height(Node* node) {
	if (node == nullptr)
		return 0;
	return 1 + max(height(node->left), height(node->right));
}

//Calls the recursive insert function and adds an object with the given name and id
void AVLTree::insert(string studentName, int id) {
	node = insertRecursive(node, studentName, id);
	//node = balance(node);
	numNodes++;
}

//Inserts a new node recursively 
//Taken from Stepik 5.2.2 but adjusted to include the balancing function and the cout statements...
//...as well as differentiate the output for unsuccessful inserts
//(Stepik 5.2.2 taken from 4.3.2, see submission)
Node* AVLTree::insertRecursive(Node* node, string studentName, int id) {
	if (node == nullptr) {
		cout << "successful" << endl;
		return new Node(studentName, id);
	}

	if (id < node->id) {
		node->left = insertRecursive(node->left, studentName, id);
		node->left->height = height(node->left);
	}
	else if (id > node->id) {
		node->right = insertRecursive(node->right, studentName, id);
		node->right->height = height(node->right);
	}
	else {
		cout << "unsuccessful" << endl;
		return node;
	}

	//balance(node->left);
	//balance(node->right);

	return balance(node);
}

//Balances nodes following insertion
Node* AVLTree::balance(Node* node) {
	//Null nodes and leaf nodes do not need to be balanced
	if (node == nullptr || (node->left == nullptr && node->right == nullptr)) {
		return node;
	}

	if (node->left != nullptr) {
		//Left-left case; rotate right
		if (calculateBalanceFactor(node->left) >= 1 && calculateBalanceFactor(node) >= 2) {
			return rotateRight(node);
		}
		//Left-right case; rotate left right
		if (calculateBalanceFactor(node->left) <= -1 && calculateBalanceFactor(node) >= 2) {
			return rotateLeftRight(node);
		}
	}

	if (node->right != nullptr) {
		//Right-right case; rotate left
		if (calculateBalanceFactor(node->right) <= -1 && calculateBalanceFactor(node) <= -2) {
			return rotateLeft(node);
		}
		//Right-left case; rotate right left
		if (calculateBalanceFactor(node->right) >= 1 && calculateBalanceFactor(node) <= -2) {
			return rotateRightLeft(node);
		}
	}
	return node;
}

//Calculates the balance factor using the height function
//Implemented in the format used in Professor Aman's lectures, which is left-right
int AVLTree::calculateBalanceFactor(Node* node) {
	if (node == nullptr)
		return 0;
	else
		return height(node->left) - height(node->right);
}

//Removes a node using the recursive remove function
void AVLTree::remove(int id) {
	bool isSuccessful = false;

	//No node to remove
	if (numNodes == 0) {
		cout << "unsuccessful" << endl;
		return;
	}

	node = removeRecursive(id, node, isSuccessful);

	if (!isSuccessful)
		cout << "unsuccessful" << endl;
	else {
		numNodes--;
		cout << "successful" << endl;
	}
}

//Removes node recursively
Node* AVLTree::removeRecursive(int id, Node* node, bool& isSuccessful) {
	//Search for node
	if (node == nullptr)
		return node;
	else if (node->id < id)
		node->right = removeRecursive(id, node->right, isSuccessful);
	else if (node->id > id)
		node->left = removeRecursive(id, node->left, isSuccessful);
	else {
		//Delete node functionality
		isSuccessful = true;
		Node* temp;

		//Leaf node
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			return nullptr;
		}

		//Left node
		else if (node->right == nullptr) {
			temp = node->left;
			delete node;
			return temp;
		}

		//Right node
		else if (node->left == nullptr) {
			temp = node->right;
			delete node;
			return temp;
		}

		//Left and right node; replace with inorder successor
		else {
			Node* inOrderSuccessor = node->right;

			while (inOrderSuccessor->left != nullptr) {
				inOrderSuccessor = inOrderSuccessor->left;
			}

			node->id = inOrderSuccessor->id;
			node->studentName = inOrderSuccessor->studentName;
			bool temp = true;
			node->right = removeRecursive(node->id, node->right, temp);
		}
		return node;
	}
	return node;
}

//Searches for an inputted id
void AVLTree::search(int id) {
	//Tree is empty
	if (numNodes == 0) {
		cout << "unsuccessful" << endl;
		return;
	}

	bool isfound = false;
	searchRecursive(id, node, isfound);

	if (!isfound)
		cout << "unsuccessful" << endl;
}

//Recursively searches for an id and prints if found
void AVLTree::searchRecursive(int id, Node* node, bool& isFound) {
	if (node == nullptr)
		return;

	if (isFound)
		return;

	if (node->id == id) {
		cout << node->studentName << endl;
		isFound = !isFound;
		return;
	}

	if (id < node->id)
		searchRecursive(id, node->left, isFound);
	else if (id > node->id)
		searchRecursive(id, node->right, isFound);

	return;
}

//Searches for the id associated with an inputted string
void AVLTree::search(string studentName) {
	//Empty tree
	if (numNodes == 0) {
		cout << "unsuccessful" << endl;
		return;
	}

	//Puts matches into a vector
	vector<string> matches;
	searchRecursive(studentName, node, matches);

	if (matches.empty())
		cout << "unsuccessful" << endl;
	else {
		for (unsigned int i = 0; i < matches.size(); i++) {
			cout << matches[i] << endl;
		}
	}
}

//Recursively searches for matching student names
void AVLTree::searchRecursive(string name, Node* node, vector<string>& matches) {
	if (node == nullptr) {
		return;
	}

	if (node->studentName == name) {
		//Convert id to string
		string match = to_string(node->id);

		while (match.length() < 8) {
			match = "0" + match;
		}

		matches.push_back(match);
	}

	searchRecursive(name, node->left, matches);
	searchRecursive(name, node->right, matches);
}

//Calls the recursive print function for the respective order traversal
void AVLTree::printOrder(string order) {
	if (numNodes != 0) {
		string output;

		if (order == "inorder")
			output = printInorderRecursive(node);
		else if (order == "preorder")
			output = printPreorderRecursive(node);
		else if (order == "postorder")
			output = printPostorderRecursive(node);

		cout << output.substr(2) << endl;
	}
}

//Recursively traverses the tree inorder
//Taken from Stepik 4.2.1, adding the comma 
string AVLTree::printInorderRecursive(Node* node) {
	string str = "";

	if (node != nullptr) {
		str += printInorderRecursive(node->left);

		//str += to_string(node->height);

		str += ", ";
		str += node->studentName;
		str += printInorderRecursive(node->right);

	}

	return str;
}

//Recursively traverses the tree preorder
//Based off of the inorder function
string AVLTree::printPreorderRecursive(Node* node) {
	string str = "";

	if (node != nullptr) {
		str += ", ";
		str += node->studentName;
		str += printPreorderRecursive(node->left);
		str += printPreorderRecursive(node->right);
	}

	return str;
}

//Recursively traverses the tree postorder
//Based off of the inorder function
string AVLTree::printPostorderRecursive(Node* node) {
	string str = "";

	if (node != nullptr) {
		str += printPostorderRecursive(node->left);
		str += printPostorderRecursive(node->right);
		str += ", ";
		str += node->studentName;
	}

	return str;
}

//Print height
void AVLTree::printHeight() {
	cout << height(node) << endl;
}

//Remove the node at the given index
void AVLTree::removeInorder(int index) {
	//Check if it's a valid index
	if (index >= numNodes || index < 0) {
		cout << "unsuccessful" << endl;
		return;
	}

	numNodes--;
	int counter = 0;
	node = removeInorderRecursive(node, index + 1, counter);
	cout << "successful" << endl;
}

Node* AVLTree::removeInorderRecursive(Node* node, int index, int& counter) {
	if (node == nullptr)
		return node;

	node->left = removeInorderRecursive(node->left, index, counter);
	counter++;

	if (counter == index) {
		//Delete node functionality
		Node* temp;

		//Leaf node
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			return nullptr;
		}

		//Left node
		else if (node->right == nullptr) {
			temp = node->left;
			delete node;
			return temp;
		}

		//Right node
		else if (node->left == nullptr) {
			temp = node->right;
			delete node;
			return temp;
		}

		//Left and right node; replace with inorder successor
		else {
			Node* inOrderSuccessor = node->right;

			while (inOrderSuccessor->left != nullptr) {
				inOrderSuccessor = inOrderSuccessor->left;
			}

			node->id = inOrderSuccessor->id;
			node->studentName = inOrderSuccessor->studentName;
			bool temp = true;
			node->right = removeRecursive(node->id, node->right, temp);
		}
		return node;
	}

	//Index could not be found/reached
	else if (counter > index) {
		return node;
	}

	node->right = removeInorderRecursive(node->right, index, counter);
	return node;
}

//Taken from Stepik 5.1.1, edited to include height
Node* AVLTree::rotateLeft(Node* node) {
	Node* grandchild = node->right->left;
	Node* newParent = node->right;
	newParent->left = node;
	node->right = grandchild;
	node->height = height(node);
	return newParent;
}

//Taken from Stepik 5.1.1, edited to include height
Node* AVLTree::rotateRight(Node* node) {
	Node* grandchild = node->left->right;
	Node* newParent = node->left;
	newParent->right = node;
	node->left = grandchild;
	node->height = height(node);
	return newParent;
}

//Taken from Stepik 5.1.1, edited to include height
Node* AVLTree::rotateLeftRight(Node* node) {
	Node* newParent;
	node->left = rotateLeft(node->left);
	newParent = rotateRight(node);
	newParent->height = height(node);
	return newParent;
}

//Taken from Stepik 5.1.3, edited to include height
Node* AVLTree::rotateRightLeft(Node* node) {
	Node* newParent;
	node->right = rotateRight(node->right);
	newParent = rotateLeft(node);
	newParent->height = height(node);
	return newParent;
}

//========================================================================

int main() {
	AVLTree tree;
	string input;
	int count;

	//Number of commands
	getline(cin, input);

	//If the first line of a list of commands isn't a number, crash the program
	if (stoi(input))
		count = stoi(input);
	else {
		cout << "An error has occurred." << endl;
		return 0;
	}

	for (int i = 0; i < count; i++) {
		getline(cin, input);
		int firstBreak = input.find(" ");
		string command = input.substr(0, firstBreak);

		//If the command is an insert command
		if (command == "insert") {
			int firstQuote = input.find("\"");
			firstQuote++;
			int secondQuote = input.find("\"", firstQuote);

			//String to insert
			string studentName = input.substr(firstQuote, secondQuote - firstQuote);

			//Ensures the name consists of letters only
			bool isValid = true;

			for (unsigned int i = 0; i < studentName.size(); i++) {
				if (!(studentName[i] == ' ') && !isalpha(studentName[i])) {
					isValid = false;
					break;
				}
			}

			//Get the student's id as a string
			secondQuote += 2;
			string idString = input.substr(secondQuote);

			//Ensure that the id has a length of 8
			if (idString.size() != 8)
				isValid = false;

			//Ensure that all characters of the id are numeric
			for (unsigned int i = 0; i < idString.length(); i++) {
				if (!isdigit(idString[i])) {
					isValid = false;
					break;
				}
			}

			//If the id isn't a number, output "unsuccessful"
			int idNum;
			if (stoi(idString))
				idNum = stoi(idString);
			else
				isValid = false;

			//Insert the node if everything works
			if (isValid)
				tree.insert(studentName, idNum);
			else
				cout << "unsuccessful" << endl;
		}

		//If the command is a remove command
		else if (command == "remove") {
			firstBreak++;
			string idString = input.substr(firstBreak);
			bool isValid = true;

			//Ensure that all characters of the id are numeric
			for (unsigned int i = 0; i < idString.length(); i++) {
				if (!isdigit(idString[i])) {
					isValid = false;
					break;
				}
			}

			//Remove the node if everything works
			if (!isValid || idString.size() != 8)
				cout << "unsuccessful" << endl;
			else {
				int idNum = stoi(idString);
				tree.remove(idNum);
			}
		}

		//If the command is a search command
		else if (command == "search") {
			//If a quote is found, it is a search for a name
			if (input.find("\"") != string::npos) {
				int firstQuote = input.find("\"");
				firstQuote++;
				int secondQuote = input.find("\"", firstQuote);
				string studentName = input.substr(firstQuote, secondQuote - firstQuote);
				bool isValidName = true;

				//Ensure that all characters of the name are letters
				for (unsigned int i = 0; i < studentName.size(); i++) {
					if (!(studentName[i] == ' ') && !isalpha(studentName[i])) {
						isValidName = false;
					}
				}

				//Search for node if successful
				if (isValidName)
					tree.search(studentName);
				else
					cout << "unsuccessful" << endl;
			}

			//Search for an id
			else {
				firstBreak++;
				string idString = input.substr(firstBreak);
				int idNum;
				bool isValid = true;

				//Ensure that all characters of the id are numeric
				for (unsigned int i = 0; i < idString.length(); i++) {
					if (!isdigit(idString[i])) {
						isValid = false;
						break;
					}
				}

				//Ensure that the id has a length of 8
				if (idString.size() != 8 || !isValid) {
					cout << "unsuccessful" << endl;
				}
				else {
					//Search for node if successful
					if (stoi(idString)) {
						idNum = stoi(idString);
						tree.search(idNum);
					}
					else {
						cout << "unsuccessful" << endl;
					}

				}
			}
		}

		//If the command is to print the inorder traversal
		else if (command == "printInorder") {
			tree.printOrder("inorder");
		}

		//If the command is to print the preorder traversal
		else if (command == "printPreorder") {
			tree.printOrder("preorder");
		}

		//If the command is to print the postorder traversal
		else if (command == "printPostorder") {
			tree.printOrder("postorder");
		}

		//If the command is to print the level count (aka the height)
		else if (command == "printLevelCount") {
			tree.printHeight();
		}

		//If the command is to remove the node at a given index of the inorder traversal
		else if (command == "removeInorder") {
			firstBreak++;
			string idString = input.substr(firstBreak);
			int idIndex;
			bool isValid = true;

			//Ensure that all characters of the id are numeric
			for (unsigned int i = 0; i < idString.length(); i++) {
				if (!isdigit(idString[i])) {
					isValid = false;
					break;
				}
			}

			if (stoi(idString)) {
				idIndex = stoi(idString);

				//Search for node if successful
				if (isValid)
					tree.removeInorder(idIndex);
				else
					cout << "unsuccessful" << endl;
			}

			else {
				cout << "unsuccessful" << endl;
			}
		}

		//If it's none of the covered commands, print out "unsuccessful"
		else {
			cout << "unsuccessful" << endl;
		}
	}
	return 0;
}