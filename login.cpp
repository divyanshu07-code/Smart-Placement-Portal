#include "login.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Login::Login(const string& filename) : credFile(filename) {
    // If credentials file does not exist, initialize with default admin:admin123
    ifstream checkFile(credFile);
    if (!checkFile.good()) {
        ofstream outFile(credFile);
        if (outFile) {
            outFile << "admin|" << maskPassword("admin123") << "\n";
            outFile.close();
        }
    }
}

string Login::maskPassword(const string& password) const {
    string masked = password;
    const char saltKey = 'X';
    for (size_t i = 0; i < password.size(); i++) {
        masked[i] = password[i] ^ saltKey;
    }
    return masked;
}

bool Login::authenticate() {
    ifstream file(credFile);
    string storedUser = "admin";
    string storedMaskedPass = maskPassword("admin123");

    if (file) {
        string line;
        if (getline(file, line)) {
            stringstream ss(line);
            getline(ss, storedUser, '|');
            getline(ss, storedMaskedPass);
        }
        file.close();
    }

    int attempts = 3;
    cout << "\n=============================================\n";
    cout << "          SECURE PORTAL LOGIN               \n";
    cout << "=============================================\n";

    while (attempts > 0) {
        string username, password;
        cout << "Enter Username : ";
        cin >> username;
        cout << "Enter Password : ";
        cin >> password;

        if (username == storedUser && maskPassword(password) == storedMaskedPass) {
            cout << "\n[+] Authentication Successful! Welcome, " << username << ".\n";
            return true;
        } else {
            attempts--;
            cout << "[-] Invalid credentials! Remaining attempts: " << attempts << "\n\n";
        }
    }

    cout << "[!] Access Denied. Maximum login attempts exceeded.\n";
    return false;
}

bool Login::changePassword() {
    ifstream file(credFile);
    string storedUser = "admin";
    string storedMaskedPass = maskPassword("admin123");

    if (file) {
        string line;
        if (getline(file, line)) {
            stringstream ss(line);
            getline(ss, storedUser, '|');
            getline(ss, storedMaskedPass);
        }
        file.close();
    }

    cout << "\n--- Change Admin Password ---\n";
    string currentPass, newPass, confirmPass;
    cout << "Enter Current Password : ";
    cin >> currentPass;

    if (maskPassword(currentPass) != storedMaskedPass) {
        cout << "[!] Current password verification failed!\n";
        return false;
    }

    cout << "Enter New Password     : ";
    cin >> newPass;
    cout << "Confirm New Password   : ";
    cin >> confirmPass;

    if (newPass.empty()) {
        cout << "[!] Password cannot be empty.\n";
        return false;
    }

    if (newPass != confirmPass) {
        cout << "[!] New passwords do not match. Operation cancelled.\n";
        return false;
    }

    ofstream outFile(credFile);
    if (!outFile) {
        cout << "[!] Error writing updated credentials to disk.\n";
        return false;
    }

    outFile << storedUser << "|" << maskPassword(newPass) << "\n";
    outFile.close();

    cout << "[+] Password updated successfully!\n";
    return true;
}
