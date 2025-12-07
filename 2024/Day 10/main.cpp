#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

struct node {
    int value;
    int index;
    vector<node*> neighbours;

    node(int val = -1) : value(val) {}
};

vector<node*> getTrailheads(vector<int> input, int lineLength) {
    vector<node*> trailHeads;
    vector<node*> allNodes;
    for(int i = 0; i < input.size(); i++) {
        node* newNode = new node(input[i]);
        newNode->index = i;
        allNodes.push_back(newNode);
    }

    for(int i = 0; i < allNodes.size(); i++) {
        if(i - lineLength >= 0) { // up
            allNodes[i]->neighbours.push_back(allNodes[i-lineLength]);
        }
        if(i + lineLength < allNodes.size()) { // down
            allNodes[i]->neighbours.push_back(allNodes[i+lineLength]);
        }
        if((i%lineLength) - 1 >= 0) { // left
            allNodes[i]->neighbours.push_back(allNodes[i-1]);
        }
        if((i%lineLength) + 1 < lineLength) { //right
            allNodes[i]->neighbours.push_back(allNodes[i+1]);
        }

        if(allNodes[i]->value == 0) trailHeads.push_back(allNodes[i]);
    }

    return trailHeads;
}

int P1(node* trailHead) {
    queue<node*> Q;
    unordered_map<int, bool> visitedEndIndexes;
    unordered_map<int, bool> visited;

    int trailHeadScore = 0;
    Q.push(trailHead);
    visited[trailHead->index] = true;

    while(!Q.empty()) {
        node* v = Q.front();
        Q.pop();

        if(v->value == 9 && visitedEndIndexes[v->index] == false) {
            trailHeadScore++;
        }
        else {
            for(const auto &neighbour: v->neighbours) {
                if(!visited[neighbour->index] && neighbour->value - v->value == 1) {
                    visited[neighbour->index] = true;
                    Q.push(neighbour);
                } 
            }
        }
    }

    return trailHeadScore;
}

int P2(node* trailHead) {
    queue<node*> Q;
    unordered_map<int, bool> visitedEndIndexes;
    unordered_map<int, bool> visited;

    int trailHeadScore = 0;
    Q.push(trailHead);

    while(!Q.empty()) {
        node* v = Q.front();
        Q.pop();

        if(v->value == 9) {
            trailHeadScore++;
        }
        else {
            for(const auto &neighbour: v->neighbours) {
                if(neighbour->value - v->value == 1) {
                    Q.push(neighbour);
                } 
            }
        }
    }

    return trailHeadScore;
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

    vector<node*> trailHeads = getTrailheads(input, lineLength);
    int trailHeadScore = 0;
    int multiTrailHeadScore = 0;
    for(const auto &trailHead: trailHeads) { // BFS for part 1 and 2!
        trailHeadScore += P1(trailHead);
        multiTrailHeadScore += P2(trailHead);
    }

    cout << "Trailhead score [P1]: " << trailHeadScore << endl;
    cout << "Multi-Trailhead score [P2]: " << multiTrailHeadScore << endl;
     
    return 0;
}