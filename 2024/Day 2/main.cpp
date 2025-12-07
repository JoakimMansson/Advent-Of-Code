#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

// Forward declarations:
bool isSafe(const vector<int>& levels);
bool isSafeWithOneRemoval(const vector<int>& levels);

int main() {
    ifstream input("input.txt");
    if (!input) {
        cerr << "File not found" << endl;
        return 1;
    }

    int safeReportCount = 0;
    string line;

    // For each report (line)
    while (getline(input, line)) {
        // Parse the line into integers
        vector<int> levels;
        {
            istringstream iss(line);
            int number;
            while (iss >> number) {
                levels.push_back(number);
            }
        }

        // Check safety with single removal allowed
        if (isSafeWithOneRemoval(levels)) {
            safeReportCount++;
        }
    }

    cout << "Number of safe reports: " << safeReportCount << endl;
    return 0;
}

// Definition of helper functions:
bool isSafe(const vector<int>& levels) {
    if (levels.size() < 2) return true;

    int idx = 1;
    while (idx < (int)levels.size() && levels[idx] == levels[idx - 1]) {
        // If two adjacent levels are equal, it's unsafe
        return false;
    }
    if (idx >= (int)levels.size()) {
        // All levels were the same
        return false;
    }

    bool increasing = (levels[idx] > levels[idx - 1]);

    for (int i = idx; i < (int)levels.size(); i++) {
        int diff = levels[i] - levels[i - 1];

        if (increasing && diff <= 0) return false;
        if (!increasing && diff >= 0) return false;
        if (std::abs(diff) < 1 || std::abs(diff) > 3) return false;
    }

    return true;
}

bool isSafeWithOneRemoval(const vector<int>& levels) {
    // If already safe, no removal needed
    if (isSafe(levels)) return true;

    // Try removing each level in turn
    for (int i = 0; i < (int)levels.size(); i++) {
        vector<int> modified;
        modified.reserve(levels.size() - 1);
        for (int j = 0; j < (int)levels.size(); j++) {
            if (j == i) continue;  // Skip this index
            modified.push_back(levels[j]);
        }

        if (isSafe(modified)) return true;
    }

    return false;
}
