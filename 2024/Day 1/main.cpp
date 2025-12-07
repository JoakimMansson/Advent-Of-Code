#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm> // For std::sort
using namespace std;

void part_one(vector<int> &firstList, vector<int> &secondList) {

    
    sort(firstList.begin(), firstList.end());
    sort(secondList.begin(), secondList.end());

    int resulting_distance = 0;
    for(int i = 0; i < firstList.size(); i++) {
        //cout << "firstList[i]: " << firstList[i] << endl;
        resulting_distance += abs(firstList[i] - secondList[i]);
    }

    cout << "Distance: " << resulting_distance << endl;
}

void part_two(vector<int> &firstList, vector<int> &secondList) {

    unordered_map<int, int> secondListFrequencies;
    for(int i = 0; i < secondList.size(); i++) {
        secondListFrequencies[secondList[i]] += secondList[i];
    }

    int result = 0;
    for(int i = 0; i < firstList.size(); i++) {
        if (secondListFrequencies.find(firstList[i]) != secondListFrequencies.end()) {// Check so key from firstList exists'
            //cout << "Found matching keys: " << firstList[i] << endl;
            result += secondListFrequencies[firstList[i]];
        }
    }

    cout << "Frequency sum: " << result << endl;
}

int main() {

    ifstream inputFile("input.txt");

    if (!inputFile) {
        cerr << "Could not read in text file" << endl;
    }

    vector<int> firstList, secondList;
    int firstNumber, secondNumber;
    while (inputFile >> firstNumber >> secondNumber) {
        firstList.push_back(firstNumber);
        secondList.push_back(secondNumber);
    }

    part_one(firstList, secondList);
    part_two(firstList, secondList);
    inputFile.close();

    
    return 1;
}