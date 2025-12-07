#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

struct point{
    int x, y;
};

struct offsets{
    int offsetX1, offsetY1;
    int offsetX2, offsetY2;
};

struct indexPosition {
    int index;
    bool isValid;
};

void printInput(const vector<string> &input, int lineLength) {
    for(int i = 0; i < input.size(); i++) {
        cout << input[i];

        if((i+1)%lineLength==0 && i != 0) cout << endl;
    }
}

void printMap(const unordered_map<string, vector<int>> &inputMap) {
    for(const auto &pair: inputMap) {
        cout << "Key: " << pair.first << endl;
        for(const auto &element: pair.second) {
            cout << element << " ";
        }
        cout << endl;
    }
}

point indexToPoint(int index, int lineLength) {
    int x = index%lineLength;
    int y = (index - index%lineLength)/lineLength;

    return {x, y};
}

int pointToIndex(point p, int lineLength) {
    return p.y*lineLength + p.x;
}

offsets calculateOffset(point p1, point p2) {
    offsets result;
    result.offsetX1 = p1.x - p2.x;
    result.offsetY1 = p1.y - p2.y;
    result.offsetX2 = p2.x - p1.x;
    result.offsetY2 = p2.y - p1.y;
    return result;
}

indexPosition calculateAntinodeIndexPosition(int offsetX, int offsetY, point p, int lineLength, int inputSize) {
    indexPosition result;
    if(p.x + offsetX >= 0 && p.x + offsetX < lineLength && p.y + offsetY >= 0 && p.y + offsetY < inputSize/lineLength) {
        p.x += offsetX;
        p.y += offsetY;
        result.isValid = true;
        result.index = pointToIndex(p, lineLength);
    }
    else {
        result.isValid = false;
    }

    return result;
}



int main() {
    ifstream fileInput("input.txt");

    if(!fileInput) {
        cout << "File not found" << endl;
        return 1;
    }

    string line;
    vector<string> input;
    unordered_map<string, vector<int>> antennaPositions;
    int lineLength;
    int currentLine = 0;
    while(getline(fileInput, line)) {
        lineLength = line.size();
        for(int i = 0; i < line.size(); i++) {
            string substr = line.substr(i, 1);
            input.push_back(substr);
            if(substr != ".") antennaPositions[substr].push_back(i + line.size()*currentLine);
        }
        currentLine++;
    }

    //printMap(antennaPositions);
    //unordered_map<string, unordered_map<int, bool>> uniqueAntinode;
    unordered_map<int, bool> uniqueAntinodeP1;
    unordered_set<int> uniqueAntinodeP2;
    for(const auto &pair: antennaPositions) {
        string antennaType = pair.first;
        for(int i = 0; i < pair.second.size(); i++) {
            for(int j = i+1; j < pair.second.size(); j++) {
                point p1 = indexToPoint(antennaPositions[antennaType][i], lineLength);
                point p2 = indexToPoint(antennaPositions[antennaType][j], lineLength);
                //cout << "point1: " << "(" << p1.x << ", " << p1.y << ")" << endl;
                //cout << "point2: " << "(" << p2.x << ", " << p2.y << ")" << endl;
                offsets off = calculateOffset(p1, p2);
                //cout << "offset x1: " << off.offsetX1 << ", offset y1: " << off.offsetY1 << endl;
                //cout << "offset x2: " << off.offsetX2 << ", offset y2: " << off.offsetY2 << endl;

                /* ------ PART 1 ------*/
                indexPosition antinode1 = calculateAntinodeIndexPosition(off.offsetX1, off.offsetY1, p1, lineLength, input.size());
                indexPosition antinode2 = calculateAntinodeIndexPosition(off.offsetX2, off.offsetY2, p2, lineLength, input.size());

                if(antinode1.isValid) {
                    uniqueAntinodeP1[antinode1.index] = true;
                    if(input[antinode1.index] == ".") input[antinode1.index] = "#";
                }

                if(antinode2.isValid) {
                    uniqueAntinodeP1[antinode2.index] = true;
                    if(input[antinode2.index] == ".") input[antinode2.index] = "#";
                }
              
                /* ------ PART 2 ------*/
                bool antinodeValid1 = true;
                bool antinodeValid2 = true;
                int startOffsetX1 = off.offsetX1, startOffsetY1 = off.offsetY1;
                int startOffsetX2 = off.offsetX2, startOffsetY2 = off.offsetY2;
                while(antinodeValid1 || antinodeValid2) {
                    if(antinodeValid1) {
                        antinode1 = calculateAntinodeIndexPosition(off.offsetX1, off.offsetY1, p1, lineLength, input.size());
                        antinodeValid1 = antinode1.isValid;
                        if(antinodeValid1) {
                            uniqueAntinodeP2.insert(antinode1.index);
                            input[antinode1.index] = "#";
                        }
                        off.offsetX1 += startOffsetX1;
                        off.offsetY1 += startOffsetY1;
                        
                    }
                    if(antinodeValid2) {
                        antinode2 = calculateAntinodeIndexPosition(off.offsetX2, off.offsetY2, p2, lineLength, input.size());
                        antinodeValid2 = antinode2.isValid;
                        if(antinodeValid2) {
                            uniqueAntinodeP2.insert(antinode2.index);
                            input[antinode2.index] = "#";
                        }
                        off.offsetX2 += startOffsetX2;
                        off.offsetY2 += startOffsetY2;
                    }
                }
                


            }
        }

    }
    
    printInput(input, lineLength);
    cout << "Unique antinode locations [P1]: " << uniqueAntinodeP1.size() << endl;

    // Add antennas which are not yet antinodes if nr.antennas of same freq > 1!
    int count2 = 0;
    for(const auto &pair: antennaPositions) {
        if(antennaPositions[pair.first].size() > 1){
            for(const auto &index: pair.second) {
                if(uniqueAntinodeP2.find(index) == uniqueAntinodeP2.end()) count2++;
            }
        } 
    } 
    cout << "Unique antinode locations [P2]: " << uniqueAntinodeP2.size() + count2 << endl; 
    return 0;
}