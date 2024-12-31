#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct spaceInfo {
    int upper, lower;
    int size;
    spaceInfo(int u, int l, int s) : upper(u), lower(l), size(s) {}
};

void printVector(vector<string> input) {
    for(const auto &element: input) {
        cout << element;
    }
    cout << endl;
}

u_int64_t calculateChecksum(vector<string> diskmap) {
    u_int64_t sum = 0;
    int id = 0;
    for(int i = 0; i < diskmap.size(); i++) {
        if(diskmap[i] != ".") {
            sum += (u_int64_t)stoi(diskmap[i])*id;
            id++;
        }
    }
    return sum;
}

vector<string> moveIndividualFileLeft(vector<string> diskmap, bool debug) {
    int i = 0;
    int j = diskmap.size()-1;
    while(i < j) {
        while(diskmap[j] == "." && i < j) j--;
        while(diskmap[i] != "." && i < j) i++;
        if(i >= j) break;
        swap(diskmap[i], diskmap[j]);
        if(debug) printVector(diskmap);
    }
    return diskmap;
}

stack<spaceInfo> locateFreeSpace(const vector<string> &diskmap, bool debug = false) {
    stack<spaceInfo> result;
    int size = 0;
    int lower = 0, upper = 0;
    string prev = diskmap[0];
    for(int i = 0; i < diskmap.size(); i++) {
        if(diskmap[i] != prev && size != 0) {
            upper = i-1;
            result.push(spaceInfo(upper, lower, size));
            if(diskmap[i] == ".") {
                size = 1;
                lower = i;
            }
            else {
                size = 0;
                lower = 0;
            }
            upper = 0;
        }
        else if(diskmap[i] == "." && size == 0) {
            lower = i;
            size++;
        }
        else if(diskmap[i] == "."){
            size++;
        }
        prev = diskmap[i];
    }

    // Quick reverse because i put them in the wrong way and i cant bother rewriting...
    stack<spaceInfo> reversed;
    while(!result.empty()) {
        reversed.push(result.top());
        result.pop();
    }

    if(debug) {
        stack<spaceInfo> temp = reversed;
        cout << "[FREE SPACE DEBUG]" << endl;
        while(!temp.empty()) {
            spaceInfo node = temp.top();
            temp.pop();
            cout << "Size: " <<  node.size << ", Lower: " << node.lower << ", Upper: " << node.upper << endl;
        }
        cout << endl;
    }


    return reversed;
}

stack<spaceInfo> locateDiskSpace(const vector<string> &diskmap, bool debug = false) {
    stack<spaceInfo> result;
    int size = 0;
    int lower = 0, upper = 0;
    string prev = diskmap[diskmap.size()-1];
    for(int i = diskmap.size()-1; i >= 0; i--) {
        if(diskmap[i] != prev && size != 0) {
            lower = i+1;
            result.push(spaceInfo(upper, lower, size));

            if(diskmap[i] != ".") {
                size = 1;
                upper = i;
            }
            else {
                size = 0;
                upper = 0;
            }
            lower = 0;
            
        }
        else if(diskmap[i] != "." && size == 0) {
            upper = i;
            size++;
        }
        else if(diskmap[i] != "."){
            size++;
        }

        prev = diskmap[i];
    }

    // If we ended in a file run
    if(size > 0 && prev != ".") {
        lower = upper - (size - 1);
        result.push(spaceInfo(upper, lower, size));
    }


    // Quick reverse because i put them in the wrong way and i cant bother rewriting...
    stack<spaceInfo> reversed;
    while(!result.empty()) {
        reversed.push(result.top());
        result.pop();
    }

    if(debug) {
        stack<spaceInfo> temp = reversed;
        cout << "[DISK SPACE DEBUG]" << endl;
        while(!temp.empty()) {
            spaceInfo node = temp.top();
            temp.pop();
            cout << "Size: " <<  node.size << ", Lower: " << node.lower << ", Upper: " << node.upper << endl;
        }
        cout << endl;
    }

    return reversed;
}

void swapSpace(vector<string> &diskmap, spaceInfo &free, spaceInfo &disk) {
    for(int i = 0; i < disk.size; i++) {
        swap(diskmap[free.lower + i], diskmap[disk.upper - i]);
    }
}


vector<string> moveClumpedFileLeft(vector<string> diskmap,  bool debug) {
    stack<spaceInfo> freeInfo = locateFreeSpace(diskmap);
    stack<spaceInfo> diskInfo = locateDiskSpace(diskmap);
    stack<spaceInfo> tempDiskInfo;


    while(!freeInfo.empty()) {
        spaceInfo free = freeInfo.top();
        
        freeInfo.pop();
        while(!diskInfo.empty()) {
            spaceInfo disk = diskInfo.top();
            diskInfo.pop();

            if(free.size == disk.size) {
                //cout << "[SWAPPING FREE]" << "Size: " <<  free.size << ", Lower: " << free.lower << ", Upper: " << free.upper << endl;
                //cout << "[SWAPPING DISK]" << "Size: " <<  disk.size << ", Lower: " << disk.lower << ", Upper: " << disk.upper << endl;
                //cout << endl;
                swapSpace(diskmap, free, disk);
                break;
            }
            else if(free.size > disk.size) {
                //cout << "[SWAPPING FREE]" << "Size: " <<  free.size << ", Lower: " << free.lower << ", Upper: " << free.upper << endl;
                //cout << "[SWAPPING DISK]" << "Size: " <<  disk.size << ", Lower: " << disk.lower << ", Upper: " << disk.upper << endl;
                
                int diff = free.size - disk.size;
                int lower = free.lower + disk.size;
                int upper = free.upper;
                swapSpace(diskmap, free, disk);
                spaceInfo newFree = spaceInfo(upper, lower, diff);
                freeInfo.push(newFree);
                //cout << "[NEWFREE]" << "Size: " <<  newFree.size << ", Lower: " << newFree.lower << ", Upper: " << newFree.upper << endl;
                //cout << endl;
                break;
            }
            else {
                tempDiskInfo.push(disk);
            }

        } // 00...111...2...333.44.5555.6666.777.888899

        while(!tempDiskInfo.empty()) {
            diskInfo.push(tempDiskInfo.top());
            tempDiskInfo.pop();
        }

    }

    return diskmap;
}

int main() {
    ifstream fileInput("input.txt");

    if(!fileInput) {
        cout << "File not found!" << endl;
    }

    string line;
    getline(fileInput, line); // One long line as input
    cout << "Line length: " << line.size() << endl;

    vector<int> regularInput;
    vector<string> preprocessedInput;
    u_int64_t idNumber = 0;
    for(int i = 0; i < line.size(); i++) {
        regularInput.push_back(stoi(line.substr(i,1)));
        if((i+1)%2 == 0) { // If free space
            for(int j = 0; j < stoi(line.substr(i,1)); j++) {
                preprocessedInput.push_back(".");
            }
        }
        else { // If disk space
            for(int j = 0; j < stoi(line.substr(i,1)); j++) {
                preprocessedInput.push_back(to_string(idNumber)); // convert: idNumber to 'idNumber'
            }
            idNumber++;
        }
    }   
    vector<string> movedIndividualFiles = moveIndividualFileLeft(preprocessedInput, false);
    vector<string> movedClumpedFiles = moveClumpedFileLeft(preprocessedInput, false);
    // DEBUG
    //cout << "PREPROCESSED INPUT: " << endl;
    //printVector(preprocessedInput);
    //cout << "CLUMPED MOVE: " << endl;
    //printVector(movedClumpedFiles);

    u_int64_t checksumIndividual = calculateChecksum(movedIndividualFiles);
    u_int64_t checksumClumped = calculateChecksum(movedClumpedFiles);
    cout << "Checksum [P1]: " << checksumIndividual << endl;
    cout << "Checksum [P2]: " << checksumClumped << endl;
    return 0;
}