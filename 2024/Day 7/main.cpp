#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
using namespace std;

struct treeNode {
    long long value;
    string operation;
    treeNode *left;
    treeNode *right;
    treeNode *down;

    treeNode() : left(nullptr), right(nullptr), down(nullptr) {}
};

void printInput(unordered_map<long long, vector<vector<long long>>> &finalInput) {
    for(auto const &pair: finalInput) {
        cout << "Key: " << pair.first << endl;
        for(auto const &row: pair.second) {
            for(auto const &element: row) {
                cout << element << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}


void readInput(string line, unordered_map<long long, vector<vector<long long>>> &finalInput) {
    stringstream ss(line);
    string token;
    long long result;

    // Extract the key before the colon
    getline(ss, token, ':');
    result = stoll(token);

    // Initialize the map entry for this key
    finalInput[result] = {};

    // Extract the remaining part of the line (values)
    vector<long long> tempResult;
    while (getline(ss, token, ' ')) {
        if (!token.empty()) {
            tempResult.push_back(stoll(token));
        }
    }

    // Add the parsed vector to the map
    finalInput[result].push_back(tempResult);
}


void printTree(treeNode* root) {
    if(root == nullptr) return;
    printTree(root->left);
    cout << root->value << " ";
    printTree(root->right);
}

long long concatenate(long long currentValue, long long newValue) {
    if (newValue == 0) {
        // Appending "0" means shifting by 1 decimal place if currentValue != 0
        // or leaving currentValue as 0 if both are 0.
        // Usually, you'd do something like:
        return currentValue * 10 + 0;
    }
    // Count digits of newValue
    int digits = static_cast<int>(floor(log10(llabs(newValue)))) + 1; 
    // Shift currentValue left by that many digits, then add newValue
    return currentValue * static_cast<long long>(pow(10, digits)) + newValue;
}


// @allowThirdOperator: "|"
bool isCorrectOperation(treeNode* root, long long currentValue, long long targetValue, bool allowThirdOperator) {
    if(root == nullptr) return false;
    
    // Updating currentValue depending on operation (+, *, |)
    if(root->operation == "+") currentValue += root->value;
    else if(root->operation == "*") currentValue *= root->value;
    else if(allowThirdOperator && root->operation == "|") currentValue = concatenate(currentValue, root->value);

    // Check if we have reached correct value
    if (currentValue == targetValue) return true;
    else if (currentValue > targetValue) return false;

    bool isCorrect = isCorrectOperation(root->left, currentValue, targetValue, allowThirdOperator);
    if (isCorrect) return isCorrect;
    isCorrect = isCorrectOperation(root->right, currentValue, targetValue, allowThirdOperator);
    if (isCorrect) return isCorrect;

    if(allowThirdOperator){
        isCorrect = isCorrectOperation(root->down, currentValue, targetValue, allowThirdOperator);
        if (isCorrect) return isCorrect;
    }

    return false;
}

treeNode* fillTree(treeNode* node, const vector<long long>& values, long long index, string operation) {
    if(index >= values.size()) return node;

    if(node  == nullptr) {
        node = new treeNode();
        node->value = values[index];
        node->operation = operation;
        
        node->right = fillTree(node->right, values, index+1, "*");
        node->left = fillTree(node->left, values, index+1, "+");
        node->down = fillTree(node->down, values, index+1, "|");
    }

    return node;
}



int main() {
    ifstream fileInput("input.txt");

    if(!fileInput) {
        cout << "File not found!" << endl;
    }

    unordered_map<long long, vector<vector<long long>>> finalInput;
    string line;
    while(getline(fileInput, line)) {
        readInput(line, finalInput);
    }

    //printInput(finalInput);

    long long calibrationResultP1 = 0;
    long long calibrationResultP2 = 0;
    for(const auto &pair: finalInput) {
        long long targetValue = pair.first;
        for(const auto &row: pair.second) {
            treeNode* root = nullptr;
            root = fillTree(root, row, 0, "+");
            if(isCorrectOperation(root, 0, targetValue, false)) calibrationResultP1 += targetValue;
            if(isCorrectOperation(root, 0, targetValue, true)) calibrationResultP2 += targetValue;
        }
    }

    cout << "Calibration result [P1]: " << calibrationResultP1 << endl;
    cout << "Calibration result [P2]: " << calibrationResultP2 << endl;

    return 0;
}