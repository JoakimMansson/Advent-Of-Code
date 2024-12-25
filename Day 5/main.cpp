#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <vector>
#include <cstdlib> // For std::stoi
using namespace std;

void print2DVector(vector<vector<string>> &vec) {
    for (const auto &row: vec) {
        for (const auto &element: row) {
            cout << element << endl;
        }
        cout << endl; // print new row
    }
}

void printMap(map<string, vector<string>> &map) {
    for (const auto &vec: map) {
        cout << "Key: " << vec.first << endl;
        for (const auto &element: vec.second) {
            cout << element << endl;
        }
        cout << endl;
    }
}

bool elementisInVector(vector<string> vec, const string &element) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == element) return true;
    }
    return false;
}

void addRule(map<string, vector<string>> &rulesMap, const string &rule) {
    regex rulePattern(R"((\d+)\|(\d+))");

    smatch match;
    if(regex_match(rule, match, rulePattern)) {
        string X = match[1];
        string Y = match[2];

        rulesMap[X].push_back(Y);
    }

}

vector<string> addUpdate(const string &update) {
    stringstream ss(update);
    string token;
    vector<string> result;
    
    while(getline(ss, token, ',')) {
        result.push_back(token);
    }

    return result;
}

void partOne(map<string, vector<string>> &rulesMap, vector<vector<string>> &updatesVector) {
    int MiddlePageSum = 0;
    for (const auto &row: updatesVector) {
        if (row.empty()) continue; // Skip if row is empty

        vector<string> previousElements;
        bool correctUpdate = true;
        for (const auto &element: row) {
            if (previousElements.size() == 0) previousElements.push_back(element);
            else {

                for (const auto &prevElement: previousElements) {
                    if (elementisInVector(rulesMap[element], prevElement)) correctUpdate = false;
                }
                

                previousElements.push_back(element);
            }
        }

        if (correctUpdate) {
            MiddlePageSum += stoi(row[(int)row.size() / 2]);
        }

    }

    cout << "Middle Page Sum: " << MiddlePageSum << endl;
}

void partTwo(map<string, vector<string>> &rulesMap, vector<vector<string>> &updatesVector) {
    int MiddlePageSum = 0;
    for (int row = 0; row < updatesVector.size(); row++) {
        if (updatesVector[row].empty()) continue; // Skip if row is empty

        vector<string> previousElements;
        bool correctUpdate = true;
        for (int col = 0; col < updatesVector[row].size(); col++) {
            string element = updatesVector[row][col];
            if(previousElements.empty()) previousElements.push_back(element);
            else {

                for (const auto &prevElement: previousElements) {
                    if (elementisInVector(rulesMap[element], prevElement)) {
                        // SWITCH ELEMENTS HERE
                    }
                }
            }
        }

        if (!correctUpdate) {
            MiddlePageSum += stoi(row[(int)row.size() / 2]);
        }
    }
    
}

int main() {
    ifstream fileInput("input.txt");

    if(!fileInput) {
        cout << "File not found!" << endl;
        return 0;
    }

    map<string, vector<string>> rulesMap;
    vector<vector<string>> updatesVector;
    string line;
    bool readingRules = true;
    int lineNumber = 0;
    while(getline(fileInput, line)) {
        if (line == "") readingRules = false;

        if (readingRules) {
            addRule(rulesMap, line);
        }
        else {
            updatesVector.push_back(addUpdate(line));
        }
    }


    partOne(rulesMap, updatesVector);
    partTwo(rulesMap, updatesVector)

    /* DEBUGGING
    printMap(rulesMap);
    print2DVector(updatesVector);
    */


    return 1;
}