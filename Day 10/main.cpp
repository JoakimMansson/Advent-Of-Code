#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

struct node {
    int value;
    node* up; 
    node* down; 
    node* left; 
    node* right;

    node(int val = -1) : value(val), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};

vector<node> getTrailheads(vector<int> input, int lineLength) {
    vector<node> trailHeads;
    vector<node*> allNodes;
    for(int i = 0; i < input.size(); i++) {
        node* newNode = new node(input[i]);
    }

    for(int i = 0; i < allNodes.size(); i++) {
        if(i - lineLength >= 0) { // up
            allNodes[i]->up = allNodes[i-lineLength];
        }
        if(i + lineLength < allNodes.size()) { // down
            allNodes[i]->down = allNodes[i+lineLength];
        }
        if() { // left

        }
        if() { //down

        }
    }
}



int main() {

    ifstream fileInput("input.txt");

    if(!fileInput) {
        cout << "File not found!" << endl;
        return 1;
    }

    vector<int> input;
    int lineLength;
    string line;
    while(getline(fileInput, line)) {
        for(int i = 0; i < line.size(); i++) {
            input.push_back(stoi(line.substr(i, 1)));
        }
        lineLength = line.size();
    }

    node 



    return 0;
}