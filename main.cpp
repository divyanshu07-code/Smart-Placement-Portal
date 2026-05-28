
#include <iostream>
#include "Portal.h"

using namespace std;

void menu() {
    cout << "\n====== SMART PLACEMENT PORTAL ======\n";
    cout << "1. Add Student\n";
    cout << "2. Display Students\n";
    cout << "3. Add Company\n";
    cout << "4. Display Companies\n";
    cout << "5. Leaderboard\n";
    cout << "6. Eligibility Check\n";
    cout << "7. Analytics\n";
    cout << "8. Exit\n";
    cout << "====================================\n";
}

int main() {
    Portal portal;
    int choice;

    do {
        menu();
        cout << "Enter Choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid alphanumeric input string. Exiting program loop safely.\n";
            break;
        }

        switch(choice) {
            case 1:
                portal.addStudent();
                break;
            case 2:
                portal.displayStudents();
                break;
            case 3:
                portal.addCompany();
                break;
            case 4:
                portal.displayCompanies();
                break;
            case 5:
                portal.leaderboard();
                break;
            case 6:
                portal.eligibilityCheck();
                break;
            case 7:
                portal.analytics();
                break;
            case 8:
                cout << "Exiting Smart Portal system safely...\n";
                break;
            default:
                cout << "Invalid Choice! Please enter numbers 1 through 8.\n";
        }
    } while(choice != 8);

    return 0;
}