#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdexcept> // For standard exceptions like std::runtime_error
using namespace std;

template <typename T>
void printVector(const vector<T> &text, int lineLength) {
    for(int i = 0; i < text.size(); i++) {
        cout << text[i] << " ";

        if((i+1)%lineLength == 0) cout << endl;
    }
}

int getCurrentPosition(const vector<string> &text) {
    for(int i = 0; i < text.size(); i++) {
        if(text[i] == "^") return i;
    }

    throw std::runtime_error("DID NOT FIND '^' IN @getCurrentPosition");
}

int getNewStepSize(int currentStepSize, int lineLength) {
    if (currentStepSize == lineLength) {
        return -1;
    }
    else if (currentStepSize == -lineLength) {
        return 1;
    }
    else if (currentStepSize == 1) {
        return lineLength;
    }
    else if (currentStepSize == -1) {
        return -lineLength;
    }
    
    throw std::runtime_error("NOT A VALID CURRENT STEP SIZE @getNewStepSize");
}

bool isOutOfBound(const vector<string> &text, int currentPosition, int lineLength, int stepSize) {
    // Out of bounds
    int newPosition = currentPosition + stepSize;
    
    // Wrapped around row
    int oldRow = currentPosition/lineLength;
    int newRow = newPosition/lineLength;

    return (newPosition < 0 || newPosition >= text.size()) || (oldRow != newRow && abs(stepSize) == 1);
}

bool hasReachedStop(const vector<string> &text, int currentPosition, int lineLength, int stepSize) {
    int newPosition = currentPosition + stepSize;
    
    if(text[newPosition] == "#") return true;

    return false;
}

int countVisitedPositions(vector<bool> &visitedPositions) {
    int nrVisitedPositions = 0;
    for(int i = 0; i < visitedPositions.size(); i++) {
        if(visitedPositions[i] == true) nrVisitedPositions++;
    }
    return nrVisitedPositions;
}


void partOne(const vector<string> &text, vector<bool> visitedPositions, int currentPosition, int lineLength, int stepSize) {
    while(true) {
        visitedPositions[currentPosition] = true;

        if(isOutOfBound(text, currentPosition, lineLength, stepSize)) break; 
        
        // Debug
        //printVector(visitedPositions, lineLength);

        bool isStuck = hasReachedStop(text, currentPosition, lineLength, stepSize);
        while(isStuck) {
            stepSize = getNewStepSize(stepSize, lineLength);
            isStuck = hasReachedStop(text, currentPosition, lineLength, stepSize);
        }

        currentPosition += stepSize;
    }


    cout << "Number of visited positions [P1]: " << countVisitedPositions(visitedPositions) << endl;
}


// Function for part two
bool hasLoop(const vector<string> &tempText, unordered_map<int,int> &positionCount, int currentPosition, int lineLength, int stepSize) {
    while(true) {
        if(positionCount.find(currentPosition) == positionCount.end()) { // Key does not exist
            positionCount[currentPosition] = stepSize;
        }
        else {
            if(positionCount[currentPosition] == stepSize) return true; // We reach the same position with the same direction meaning we loop!
            positionCount[currentPosition] = stepSize;
        }

        if(isOutOfBound(tempText, currentPosition, lineLength, stepSize)) break; 
        
        // Debug
        //printVector(visitedPositions, lineLength);

        // If we are stuck turn right until we can move forward!
        bool isStuck = hasReachedStop(tempText, currentPosition, lineLength, stepSize);
        while(isStuck) {
            stepSize = getNewStepSize(stepSize, lineLength);
            isStuck = hasReachedStop(tempText, currentPosition, lineLength, stepSize);
        }

        currentPosition += stepSize;
    }

    return false;
}

void partTwo(const vector<string> &text, int startPosition, int lineLength, int stepSize) {
    int nrDistinctLoops = 0;
    for(int i = 0; i < text.size(); i++) {
        if(i == startPosition || text[i] == "#") continue;
        vector<string> tempText = text;
        unordered_map<int,int> positionCount;
        tempText[i] = "#";
        if(hasLoop(tempText, positionCount, startPosition, lineLength, stepSize)) nrDistinctLoops++;
    }

    cout << "Number of distinct loops [P2]: " << nrDistinctLoops << endl;
}

int main() {

    ifstream fileInput("input.txt");

    if (!fileInput) {
        cout << "File not found!" << endl;
        return 1;
    }

    vector<string> text;
    vector<bool> visitedPositions;
    string line;
    int lineLength = 0;
    while(getline(fileInput, line)) {
        for(int i = 0; i < line.size(); i++) {
            text.push_back(line.substr(i, 1));
            visitedPositions.push_back(false);

            if (lineLength != 0 && lineLength != line.size()) {
                throw std::runtime_error("Inconsistent line lengths in input file!");
            }
            lineLength = line.size();
        }
        
    }
    

    cout << "lineLength: " << lineLength << endl;

    // Debug
    //printVector(text, lineLength);

    int startPosition = getCurrentPosition(text);
    int startStep = -lineLength;
    partOne(text, visitedPositions, startPosition, lineLength, startStep);
    partTwo(text, startPosition, lineLength, startStep);
    
    // Debug
    //printVector(visitedPositions, lineLength);

    return 0;
}