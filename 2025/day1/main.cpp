#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    std::ifstream file("input.txt");

    vector<int> value = {};
    vector<string> direction = {};
    string line;
    int current_dial_pos = 50;
    int first_password = 0;
    int second_password = 0;
    int count = 0;
    while ( file >> line) {
        string dir = line.substr(0, 1);
        int rotation_value = stoi(line.substr(1));

        if(dir == "L") {
            int diff = (current_dial_pos - (rotation_value % 100)); // If we are over/above 0
            int new_dial_pos = diff >= 0 ? diff : diff + 100;
            
            // Second password increments for all extra turns but we miss increment for those which only flip negative once
            second_password += rotation_value / 100;
            second_password += diff >= 0 || current_dial_pos == 0 ? 0 : 1;

            cout << "\nL: " << current_dial_pos << " - (" << rotation_value << " % 100)" << " = " << diff << " -> " << new_dial_pos << endl;
            //cout << "   -> second password: " << int((abs(rotation_value) + current_dial_pos) / 100) << endl;

            current_dial_pos = new_dial_pos % 100;
        } else {
            
            int new_dial_pos = current_dial_pos + (rotation_value % 100);
            cout << "\nR: " << current_dial_pos << " + (" << rotation_value << " % 100) -> " << new_dial_pos << endl;
            //cout << "   -> second password: " << int((current_dial_pos + rotation_value) / 100) << endl;
            
            second_password += rotation_value / 100;
            second_password += new_dial_pos > 100 ? 1 : 0;
            
            current_dial_pos = new_dial_pos % 100;
        }

        if(current_dial_pos == 0) {
            first_password++;
            second_password++;
        }
        count++;
        cout << "   -> second password: " << second_password << endl;

        //if(count == 10) return 1;
    }

    cout << "First Password: " << first_password << endl;
    cout << "Second Password: " << second_password << endl;

    file.close();
    return 0;
}