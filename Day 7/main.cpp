#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

void printInput(unordered_map<int, vector<vector<int>>> &finalInput) {
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

void readInput(string line, unordered_map<int, vector<vector<int>>> &finalInput) {
    string tempLine;
    int result;
    for(int i = 0; i < line.size(); i++) {
        if(line[i] == ':') {
            result = stoi(tempLine);
            finalInput[result] = {};
            line = line.substr(i+2);
            tempLine = "";
            break;
        }
        tempLine += line[i];
    }


    vector<int> tempResult;
    tempLine = "";
    for(int i = 0; i < line.size(); i++) {
        if(line[i] == ' ' || i == line.size()-1){
            if(i == line.size()-1) tempLine += line.substr(i, 1);
            tempResult.push_back(stoi(tempLine));
            tempLine = "";
            
        }
        else {
            tempLine += line.substr(i, 1);
        }
    }
    finalInput[result].push_back(tempResult);

}

void partOne(unordered_map<int, vector<vector<int>>> finalInput) {
    for (const auto &pair: finalInput) {
        int key = pair.first;
        for (const auto &row: pair.second) {
            // LOGIC FOR + AND *
        }
    }
}

int main() {
    ifstream fileInput("input.txt");

    if(!fileInput) {
        cout << "File not found!" << endl;
    }

    unordered_map<int, vector<vector<int>>> finalInput;
    string line;
    while(getline(fileInput, line)) {
        readInput(line, finalInput);
    }

    printInput(finalInput);

    return 0;
}