#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int ROWS = 20;
const int COLS = 20;

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    
    ifstream file(filename);
    
    if (!file) {
        cout << "Error: Cannot open " << filename << endl;
        
        // Create sample file
        ofstream create("sample.txt");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                create << "*";
            }
            create << endl;
        }
        create.close();
        
        cout << "Created sample.txt. Use that file next time." << endl;
        return 1;
    }
    
    // Create dynamic array
    char** grid = new char*[ROWS];
    for (int i = 0; i < ROWS; i++) {
        grid[i] = new char[COLS];
        for (int j = 0; j < COLS; j++) {
            file >> grid[i][j];
        }
    }
    
    file.close();
    
    // Display
    cout << "\n20x20 Environment:" << endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
    
    // Clean up
    for (int i = 0; i < ROWS; i++) {
        delete[] grid[i];
    }
    delete[] grid;
    
    return 0;
}