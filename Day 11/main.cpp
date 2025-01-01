#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <unordered_map>
#include <array>
using namespace std;
using namespace std::chrono;

void printVector(vector<u_int64_t> vec) {
    for(const auto &element: vec) {
        cout << element << " ";
    }
    cout << endl;
}

bool hasEvenDigits(u_int64_t digit) {
    u_int64_t nrDigits = 0;
    while (digit > 0) {
        digit /= 10;
        nrDigits++;
    }
    return (nrDigits % 2 == 0);
}

u_int64_t countStones(unordered_map<u_int64_t, u_int64_t> stoneCount) {
    u_int64_t count = 0;
    for(const auto &pair: stoneCount) {
        count += pair.second;
    }
    return count;
}

void printStones(unordered_map<u_int64_t, u_int64_t> stoneCount) {
    for(const auto &pair: stoneCount) {
        if(pair.second > 0) {
            for(int i = 0; i < pair.second; i++) cout << pair.first << " ";
        }
    }
    cout << endl;
}

vector<u_int64_t> nBlinks(vector<u_int64_t> input, int blinks) {
    unordered_map<u_int64_t, array<u_int64_t, 2>> cache;

    for (int i = 0; i < blinks; i++) {
        for (int j = 0; j < input.size(); j++) {
            if (input[j] == 0) {
                input[j] = 1;
            } else if (hasEvenDigits(input[j])) {
                u_int64_t firstPart, secondPart;
                
                if (cache.find(input[j]) != cache.end()) {
                    // Retrieve from cache
                    firstPart = cache[input[j]][0];
                    secondPart = cache[input[j]][1];
                } else {
                    // Split the number manually
                    string tempInput = to_string(input[j]);
                    int middle = tempInput.size() / 2;
                    firstPart = stoll(tempInput.substr(0, middle));
                    secondPart = stoll(tempInput.substr(middle, middle));

                    // Add to cache
                    cache[input[j]] = {firstPart, secondPart};
                }

                // Update input vector
                input[j] = firstPart;
                input.insert(input.begin() + j + 1, secondPart);
                j++; // Skip the newly inserted element
            } else {
                input[j] *= 2024;
            }
        }
    }

    return input;
}

unordered_map<u_int64_t, u_int64_t> nBlinks2(unordered_map<u_int64_t, u_int64_t> stoneCount, int blinks) {
    unordered_map<u_int64_t, array<u_int64_t, 2>> cache;
    //printStones(stoneCount);
    for(int i = 0; i < blinks; i++) {
        unordered_map<u_int64_t, u_int64_t> copyStoneCount = stoneCount;
        for(const auto &pair: copyStoneCount) {
            u_int64_t stone = pair.first;
            u_int64_t count = pair.second;
            if(count == 0) continue;

            if (stone == 0) {
                stoneCount[stone] -= count;
                stoneCount[1] += count;
            } else if (hasEvenDigits(stone)) {
                u_int64_t firstPart, secondPart;
                
                if (cache.find(stone) != cache.end()) {
                    // Retrieve from cache
                    firstPart = cache[stone][0];
                    secondPart = cache[stone][1];
                } else {
                    // Split the number manually
                    string tempInput = to_string(stone);
                    int middle = tempInput.size() / 2;
                    //cout << "middle:" << middle << endl;
                    firstPart = stoll(tempInput.substr(0, middle));
                    secondPart = stoll(tempInput.substr(middle));

                    // Add to cache
                    cache[stone] = {firstPart, secondPart};
                }

                stoneCount[stone] -= count;
                stoneCount[firstPart] += count;
                stoneCount[secondPart] += count;
            } else {
                stoneCount[stone] -= count;
                stoneCount[stone*2024] += count;
            }
        }
        //printStones(stoneCount);
    }

    return stoneCount;
}


int main() {
    ifstream fileInput("input.txt");

    if(!fileInput) {
        cout << "File not found!" << endl;
        return 1;
    }

    string line;
    getline(fileInput, line);

    stringstream ss(line);
    string token;
    vector<u_int64_t> input;
    unordered_map<u_int64_t, u_int64_t> stoneCount;
    while(getline(ss, token, ' ')) {
        if(!token.empty()) {
            u_int64_t stone = stoll(token);
            input.push_back(stone);
            stoneCount[stone] = 1;
        }
        
    }
    int nrBlinks = 75;

    auto start = high_resolution_clock::now(); // Start time
    unordered_map<u_int64_t, u_int64_t> stones2 = nBlinks2(stoneCount, nrBlinks);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);// Calculate the duration
    cout << "Execution time2: " << duration.count() << " milliseconds" << endl;

    /*
    start = high_resolution_clock::now(); // Start time
    vector<u_int64_t> stones1 = nBlinks(input, nrBlinks);
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);// Calculate the duration
    cout << "Execution time1: " << duration.count() << " milliseconds" << endl;
    */
    

    //cout << "Nr stones [P1]: " << stones.size() << endl;
    cout << "Nr stones2 [P1]: " << countStones(stones2) << endl;
    //cout << "Nr stones1 [P1]: " << stones1.size() << endl;
    

    return 0;
}