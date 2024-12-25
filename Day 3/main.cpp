#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
using namespace std;

int main() {
    ifstream inputFile("input.txt");

    if (!inputFile) {
        cout << "File not found" << endl;
    }

    regex pattern(R"(mul\((\d+),(\d+)\)|(do\(\))|(don't\(\)))"); // Regex matching patterns: "mul(digit, digit)" or "do()" or "don't()"

    string line;
    int mulResult = 0;
    bool doMul = true;
    while(getline(inputFile, line)) {

        
        for(sregex_iterator it(line.begin(), line.end(), pattern), end; it != end; ++it) { // Regex iterator
            smatch match = *it;

            if (match[1].matched && match[2].matched && doMul) {
                int firstInteger = stoi(match[1]);
                int secondInteger = stoi(match[2]);
                mulResult += firstInteger*secondInteger;
            }
            // "do()" found
            else if (match[3].matched) {
                doMul = true;
            }
            // "don't()" found
            else if (match[4].matched) {
                doMul = false;
            }
            
        }
    }
    inputFile.close();

    cout << "Multiplication Result: " << mulResult << endl;


    return 0;
}