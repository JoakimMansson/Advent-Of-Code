#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

string getString(vector<char> &inputVector, int startIndex, int inc, int length, bool debug) {
    
    string result;
    for (int step = 0; step <= length; step++) {
        int i = startIndex + step * inc;
        result += inputVector[i];
    }

    if (debug) {cout << "getString: " << result << ", startIndex: " << startIndex << endl;}
    return result;
}


bool containsForward(vector<char> &inputVector, int index, int lineLength, string word) {
    int boundary = static_cast<int>(word.size()) - 1;
    if (index+boundary >= inputVector.size() || (index%lineLength)+boundary >= lineLength) return false; // maybe just index+3 (CHECK THIS AGAIN IF DOES NOT WORK)

    bool debug = false;
    string result = getString(inputVector, index, 1, boundary, false);

    if (result == word) {
        //cout << "Found FORWARD" << endl; 
        return true;
    }
    return false;
}

bool containsBackward(vector<char> &inputVector, int index, int lineLength, string word) {
    int boundary = static_cast<int>(word.size()) - 1;
    if (index-boundary < 0 || (index%lineLength)-boundary < 0) return false;

    bool debug = false;
    string result = getString(inputVector, index, -1, boundary, debug);

    if (result == word) {
        //cout << "Found BACKWARD" << endl; 
        return true;
    }
    return false;
}

bool containsUp(vector<char> &inputVector, int index, int lineLength, string word) {
    int boundary = static_cast<int>(word.size()) - 1;
    if(index - boundary*lineLength < 0) return false;
    
    bool debug = false;
    string result = getString(inputVector, index, -lineLength, boundary, debug);

    if (result == word) {
        //cout << "Found UP" << endl; 
        return true;
    }
    return false;
}

bool containsDown(vector<char> &inputVector, int index, int lineLength, string word) {
    int boundary = static_cast<int>(word.size()) - 1;
    // Ensure we don't exceed the bounds of the vector
    if (index + boundary * lineLength >= inputVector.size()) return false;

    bool debug = false;
    string result = getString(inputVector, index, lineLength, boundary, debug);

    if (result == word) {
        //cout << "Found DOWN" << endl;
        return true;
    }

    return false;
}

bool containsUpBackward(vector<char> &inputVector, int index, int lineLength, string word) {
    int boundary = static_cast<int>(word.size()) - 1;
    /* DIAGONALLY UP & BACKWARD CASE*/
    if (index - boundary*lineLength - boundary >= 0 && (index%lineLength) - boundary >= 0) { // If checking pos > 0 && we dont wrap around the rows
        
        bool debug = false;
        string result = getString(inputVector, index, -lineLength-1, boundary, debug);
        
        if (result == word) {
            //cout << "Found UP & BACKWARD" << endl; 
            return true;
        }
    }
    return false;
}

bool containsUpForward(vector<char> &inputVector, int index, int lineLength, string word) {
    int boundary = static_cast<int>(word.size()) - 1;

    /* DIAGONALLY UP & FORWARD CASE*/
    if (index - boundary*lineLength + boundary >= 0 && (index%lineLength) + boundary < lineLength) { // If checking pos > 0 && we dont wrap around the rows
        
        bool debug = false;
        string result = getString(inputVector, index, -(lineLength)+1, boundary, debug);
        
        if (result == word) {
            //cout << "Found UP & FORWARD" << endl; 
            return true;
        }
    }

    return false;
}

bool containsDownBackward(vector<char> &inputVector, int index, int lineLength, string word) {
    int boundary = static_cast<int>(word.size()) - 1;

    /* DIAGONALLY DOWN & BACKWARD CASE*/
    if (index + boundary*lineLength - boundary < inputVector.size() && (index%lineLength) - boundary >= 0) { // If checking pos > 0 && we dont wrap around the rows

        bool debug = false;
        string result = getString(inputVector, index, lineLength-1, boundary, debug);
        
        if (result == word) {
            //cout << "Found DOWN & BACKWARD" << endl; 
            return true;
        }
    }

    return false;
}

bool containsDownForward(vector<char> &inputVector, int index, int lineLength, string word) {
    int boundary = static_cast<int>(word.size()) - 1;
    /* DIAGONALLY DOWN & FORWARD CASE*/
    if (index + boundary*lineLength + boundary < inputVector.size() && (index%lineLength) + boundary < lineLength) { // If checking pos > 0 && we dont wrap around the rows

        bool debug = false;
        string result = getString(inputVector, index, lineLength+1, boundary, debug);
        
        if (result == word) {
            //cout << "Found DOWN & FORWARD" << endl; 
            return true;
        }
    }

    return false;
}


void partOne(vector<char> inputVector, int lineLength) {
    int XmasCounter = 0;
    for(int i = 0; i < inputVector.size(); i++) {
        if(inputVector[i] == 'X') {
            if(containsForward(inputVector, i, lineLength, "XMAS")) XmasCounter++;
            if(containsBackward(inputVector, i, lineLength, "XMAS")) XmasCounter++;
            if(containsUp(inputVector, i, lineLength, "XMAS")) XmasCounter++;
            if(containsDown(inputVector, i, lineLength, "XMAS")) XmasCounter++;
            if(containsUpBackward(inputVector, i, lineLength, "XMAS")) XmasCounter++;
            if(containsUpForward(inputVector, i, lineLength, "XMAS")) XmasCounter++;
            if(containsDownBackward(inputVector, i, lineLength, "XMAS")) XmasCounter++;
            if(containsDownForward(inputVector, i, lineLength, "XMAS")) XmasCounter++;
        }
    }

    cout << "XMAS Counter: " << XmasCounter << endl;
}

void partTwo(vector<char> inputVector, int lineLength) {

    int X_masCounter = 0;
    for (int i = 0; i< inputVector.size(); i++) {
        if (i + 2*lineLength + 2 >= inputVector.size()) break;

        if(inputVector[i] == 'M') {
            bool one = containsDownForward(inputVector, i, lineLength, "MAS");
            bool two = containsDownBackward(inputVector, i+2, lineLength, "MAS") || containsDownBackward(inputVector, i+2, lineLength, "SAM");
            if (one && two) X_masCounter++;
        }
        else if(inputVector[i] == 'S') {
            bool one = containsDownForward(inputVector, i, lineLength, "SAM");
            bool two = containsDownBackward(inputVector, i+2, lineLength, "MAS") || containsDownBackward(inputVector, i+2, lineLength, "SAM");
            if (one && two) X_masCounter++;
        }
    }

    cout << "X-MAS Count: " << X_masCounter << endl;
}

int main() {
    ifstream fileInput("input.txt");

    if (!fileInput) {
        cout << "File not found" << endl;
        return 1;
    }

    vector<char> inputVector; // Containing the input
    int lineLength;
    string line;
    while(getline(fileInput, line)) {
        lineLength = line.length();
        for(char c: line) inputVector.push_back(c);
    }
    fileInput.close();
    cout << "Line length: " << lineLength << endl;

    partOne(inputVector, lineLength);
    partTwo(inputVector, lineLength);




    return 0;
}