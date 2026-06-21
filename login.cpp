#include <iostream>
#include <fstream>
#include <string>
using namespace std;


string hashPassword(const string& password) {
    string hashed = password;
    char saltKey = 'X'; 
    for (size_t i = 0; i < password.size(); i++) {
        hashed[i] = password[i] ^ saltKey;
    }
    return hashed;
}

bool handleLogin() {
    string username, password;
    cout << "\n🔒 === SECURE PORTAL LOGIN ===\n";
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    string secureHash = hashPassword(password);

   
    if (username == "admin" && password == "admin123") {
        cout << "✅ Authentication Successful! Session token generated.\n";
        return true;
    } else {
        cout << "❌ [Security Alert] Invalid Credentials Provided.\n";
        return false;
    }
}
