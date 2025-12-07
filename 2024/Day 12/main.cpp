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
    bool isVisitedDirection;
    unordered_map<string, node*> neighboursDirection;
    vector<node*> neighbours;

    node(string val) : value(val) {
        neighboursDirection["up"] = nullptr;
        neighboursDirection["down"] = nullptr;
        neighboursDirection["left"] = nullptr;
        neighboursDirection["right"] = nullptr;
    }
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
            allNodes[i]->neighboursDirection["up"] = allNodes[i-lineLength];
        }
        else { // out of bound 
            node* newNode = new node("EMPTY");
            allNodes[i]->neighbours.push_back(newNode);
        }

        if(i + lineLength < allNodes.size()) { // down
            allNodes[i]->neighbours.push_back(allNodes[i+lineLength]);
            allNodes[i]->neighboursDirection["down"] = allNodes[i+lineLength];
        }
        else { // out of bound 
            node* newNode = new node("EMPTY");
            allNodes[i]->neighbours.push_back(newNode);
        }

        if((i%lineLength) - 1 >= 0) { // left
            allNodes[i]->neighbours.push_back(allNodes[i-1]);
            allNodes[i]->neighboursDirection["left"] = allNodes[i-1];
        }
        else { // out of bound 
            node* newNode = new node("EMPTY");
            allNodes[i]->neighbours.push_back(newNode);
        }


        if((i%lineLength) + 1 < lineLength) { //right
            allNodes[i]->neighbours.push_back(allNodes[i+1]);
            allNodes[i]->neighboursDirection["right"] = allNodes[i+1];
        }
        else { // out of bound 
            node* newNode = new node("EMPTY");
            allNodes[i]->neighbours.push_back(newNode);
        }

    }

    return allNodes;
}

int P1(node* land) {
    queue<node*> Q;

    int area = 0;
    int perimeter = 0;
    Q.push(land);
    land->isVisited = true;

    while(!Q.empty()) {
        node* v = Q.front();
        Q.pop();
        area++;

        for(const auto &neighbour: v->neighbours) {
            if(!neighbour->isVisited && v->value == neighbour->value) {
                neighbour->isVisited = true;
                Q.push(neighbour);
            }
            else if(neighbour->value == "EMPTY" || v->value != neighbour->value) {
                perimeter++;
            }
        }
    }

    return area*perimeter;
}

bool isCorner(node* v) {
    int count = 0;

    for(const auto &neighbour: v->neighbours) {
        if(v->value != neighbour->value) {
            count++;
        }
    }
    return count >= 2;
}

int getSideLength(node* v, string direction, int length = 0) {
    int count = 0;
    for(const auto &neighbour: v->neighbours) {
        if(neighbour->value == v->value) count++;
    }
    if(count == v->neighbours.size()) return length;


    getSideLength(v->neighboursDirection[direction], direction, length+1);

    return length;
}

int P2(node* land) {
    queue<node*> Q;

    int area = 0;
    int perimeter = 0;
    Q.push(land);
    land->isVisited = true;

    while(!Q.empty()) {
        node* v = Q.front();
        Q.pop();
        area++;

        for(const auto &neighbour: v->neighbours) {
            if(!neighbour->isVisited && v->value == neighbour->value) {
                neighbour->isVisited = true;
                Q.push(neighbour);
            }
            else if(neighbour->value == "EMPTY" || v->value != neighbour->value) {
                perimeter++;
            }
        }
    }

    return area*perimeter;
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
    int cost = 0;
    for(const auto &l: land) {
        if(!l->isVisited) {
            int landCost = P1(l);
            cost += landCost;
            cout << l->value << ": " << landCost << endl;
        }
    }
    cout << "Land cost: " << cost << endl;
     
    return 0;
}