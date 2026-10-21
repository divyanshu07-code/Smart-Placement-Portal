#include <iostream>
#include <limits>
#include "Portal.h"
#include "login.h"

using namespace std;

void showMenu() {
    cout << "\n=============================================\n";
    cout << "           SMART PLACEMENT PORTAL            \n";
    cout << "=============================================\n";
    cout << " [STUDENT MANAGEMENT]\n";
    cout << "   1. Add New Student Record\n";
    cout << "   2. Display All Students\n";
    cout << "   3. Search Student (by ID / Name)\n";
    cout << "   4. Mark Student as Placed\n";
    cout << "   5. Delete Student Record\n\n";
    cout << " [COMPANY MANAGEMENT]\n";
    cout << "   6. Add Recruiting Company Drive\n";
    cout << "   7. Display All Recruiting Companies\n";
    cout << "   8. Delete Company Drive\n\n";
    cout << " [DRIVE MATCHING & ANALYTICS]\n";
    cout << "   9. Merit Leaderboard (CGPA & Tie-Breaker)\n";
    cout << "  10. Check Student Eligibility (by ID)\n";
    cout << "  11. Generate Company Shortlist (by Company)\n";
    cout << "  12. Placement Statistics & Analytics\n\n";
    cout << " [SETTINGS & EXIT]\n";
    cout << "  13. Change Admin Password\n";
    cout << "  14. Exit Portal\n";
    cout << "=============================================\n";
}

int main() {
    cout << "\n*********************************************\n";
    cout << "*    CAMPUS PLACEMENT MANAGEMENT SYSTEM     *\n";
    cout << "*********************************************\n";

    Login auth;
    if (!auth.authenticate()) {
        cout << "\n[!] System Terminated due to failed authentication.\n";
        return 1;
    }

    Portal portal;
    int choice = 0;

    while (true) {
        showMenu();
        cout << "Enter your choice (1-14): ";

        if (!(cin >> choice)) {
            cout << "\n[!] Invalid input! Please enter a number between 1 and 14.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                portal.addStudent();
                break;
            case 2:
                portal.displayStudents();
                break;
            case 3:
                portal.searchStudent();
                break;
            case 4:
                portal.markStudentPlaced();
                break;
            case 5:
                portal.deleteStudent();
                break;
            case 6:
                portal.addCompany();
                break;
            case 7:
                portal.displayCompanies();
                break;
            case 8:
                portal.deleteCompany();
                break;
            case 9:
                portal.leaderboard();
                break;
            case 10:
                portal.eligibilityCheck();
                break;
            case 11:
                portal.companyShortlist();
                break;
            case 12:
                portal.analytics();
                break;
            case 13:
                auth.changePassword();
                break;
            case 14:
                cout << "\n[+] Thank you for using Smart Placement Portal. Exiting safely...\n";
                return 0;
            default:
                cout << "\n[!] Invalid selection! Please enter a valid option (1-14).\n";
                break;
        }
    }

    return 0;
}
