#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

struct node {
    string value;
    int index;
    bool isVisited;
    vector<node*> neighbours;

    node(string val) : value(val) {}
};

vector<node*> getLand(vector<string> input, int lineLength) {
    vector<node*> allNodes;
    for(int i = 0; i < input.size(); i++) {
        node* newNode = new node(input[i]);
        newNode->index = i;
        newNode->isVisited = false;
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

    }

    return allNodes;
}

int P1(node* trailHead) {
    queue<node*> Q;
    unordered_map<int, bool> visited;

    int area = 0;
    Q.push(trailHead);
    visited[trailHead->index] = true;

    while(!Q.empty()) {
        node* v = Q.front();
        Q.pop();
        area++;

        int nrVisitableNeighbours = 0;
        for(const auto &neighbour: v->neighbours) {
            if(!visited[neighbour->index] && v->value == neighbour->value) {
                visited[neighbour->index] = true;
                Q.push(neighbour);
                nrVisitableNeighbours++;
            } 
        }
    }

    return area;
}

int main() {

    ifstream fileInput("input.txt");

    if(!fileInput) {
        cout << "File not found!" << endl;
        return 1;
    }

    vector<string> input;
    int lineLength;
    string line;
    while(getline(fileInput, line)) {
        for(int i = 0; i < line.size(); i++) {
            input.push_back(line.substr(i, 1));
        }
        lineLength = line.size();
    }

    vector<node*> land = getLand(input, lineLength);
    
     
    return 0;
}