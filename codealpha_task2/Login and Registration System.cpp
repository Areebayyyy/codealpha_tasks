#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
using namespace std;

const string FILE_NAME = "users.txt";

// Simple demonstration hash.
// Not suitable for production password security.
uint64_t passwordHash(const string &password) {
    uint64_t hash = 14695981039346656037ULL;

    for (unsigned char ch : password) {
        hash ^= ch;
        hash *= 1099511628211ULL;
    }

    return hash;
}

bool usernameExists(string username) {
    ifstream file(FILE_NAME);

    string savedUser;
    uint64_t savedHash;

    while (file >> savedUser >> savedHash) {
        if (savedUser == username)
            return true;
    }

    return false;
}

void registerUser() {
    string username, password;

    cout << "\nEnter new username: ";
    cin >> username;

    if (usernameExists(username)) {
        cout << "Username already exists.\n";
        return;
    }

    cout << "Enter password (minimum 6 characters): ";
    cin >> password;

    if (password.length() < 6) {
        cout << "Password is too short.\n";
        return;
    }

    ofstream file(FILE_NAME, ios::app);

    if (!file) {
        cout << "File opening error.\n";
        return;
    }

    file << username << " "
         << passwordHash(password) << endl;

    cout << "Registration successful!\n";
}

void loginUser() {
    string username, password, savedUser;
    uint64_t savedHash;

    cout << "\nEnter username: ";
    cin >> username;

    cout << "Enter password: ";
    cin >> password;

    ifstream file(FILE_NAME);

    while (file >> savedUser >> savedHash) {

        if (savedUser == username) {

            if (savedHash == passwordHash(password)) {
                cout << "Login successful!\n";
                cout << "Welcome, " << username << "!\n";
            }
            else {
                cout << "Incorrect password.\n";
            }

            return;
        }
    }

    cout << "Username not found.\n";
}

int main() {
    int choice;

    do {
        cout << "\n===== LOGIN SYSTEM =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "0. Exit\n";

        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;

            case 2:
                loginUser();
                break;

            case 0:
                cout << "Program closed.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}