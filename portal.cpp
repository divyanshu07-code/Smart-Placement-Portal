

#include "Portal.h"
#include "Database.h"   
#include <iostream>
#include <iomanip>      
#include <algorithm>    

using namespace std;

Portal::Portal() {
    Database db;
    students = db.loadStudents();   
    companies = db.loadCompanies();
}
void Portal::printLine(int width, char ch) const {
    cout << setfill(ch) << setw(width) << "" << setfill(' ') << "\n";
}

void Portal::addStudent() {
    Student s;
    cout << "\n--- Add New Student ---\n";
    cout << "Enter Student ID: ";
    cin >> s.id;
    cin.ignore();                     
    cout << "Enter Name: ";
    getline(cin, s.name);
    cout << "Enter Branch (e.g., ICT/ICT CS/EVD/CS AI/ECE AI/ MNC): ";
    getline(cin, s.branch);
    cout << "Enter CGPA: ";
    cin >> s.cgpa;
    cout << "Enter Active Backlogs Count: ";
    cin >> s.backlogs;
    s.isPlaced = false;
    s.placedCompany = "None";

    students.push_back(s);
    Database db;
    db.saveStudents(students); 
}

void Portal::displayStudents() const {
    cout << "\n" << setw(60) << left << "================ REGISTERED STUDENTS ================" << "\n";
    cout << setw(12) << left << "ID" 
         << setw(20) << left << "Name" 
         << setw(10) << left << "Branch" 
         << setw(8) << left << "CGPA" 
         << setw(10) << left << "Backlogs" 
         << setw(12) << left << "Status" << "\n";
    printLine(72, '-');

    for (const auto& s : students) {
        cout << setw(12) << left << s.id
             << setw(20) << left << s.name
             << setw(10) << left << s.branch
             << setw(8) << left << fixed << setprecision(2) << s.cgpa
             << setw(10) << left << s.backlogs
             << setw(12) << left << (s.isPlaced ? "Placed (" + s.placedCompany + ")" : "Unplaced") << "\n";
    }
    printLine(72, '=');
}

void Portal::addCompany() {
    Company c;
    cout << "\n--- Add New Recruiting Company ---\n";
    cin.ignore();
    cout << "Enter Company Name: ";
    getline(cin, c.name);
    cout << "Enter Job Profile/Role: ";
    getline(cin, c.role);
    cout << "Enter Minimum CGPA Cutoff: ";
    cin >> c.minCGPA;
    cout << "Enter Max Backlogs Allowed: ";
    cin >> c.maxBacklogsAllowed;
    cout << "Enter CTC Package Offered (LPA): ";
    cin >> c.ctc;

    companies.push_back(c);
    Database db;
    db.saveCompanies(companies); 
}

void Portal::displayCompanies() const {
    cout << "\n" << setw(65) << left << "================ RECRUITING COMPANIES ================" << "\n";
    cout << setw(18) << left << "Company Name" 
         << setw(22) << left << "Role/Profile" 
         << setw(12) << left << "Min CGPA" 
         << setw(10) << left << "Max Back" 
         << setw(10) << left << "CTC (LPA)" << "\n";
    printLine(72, '-');

    for (const auto& c : companies) {
        cout << setw(18) << left << c.name
             << setw(22) << left << c.role
             << setw(12) << left << fixed << setprecision(2) << c.minCGPA
             << setw(10) << left << c.maxBacklogsAllowed
             << setw(10) << left << c.ctc << "\n";
    }
    printLine(72, '=');
}

void Portal::leaderboard() const {
    vector<Student> sortedStudents = students;
    
    sort(sortedStudents.begin(), sortedStudents.end(), [](const Student& a, const Student& b) {
        if (a.cgpa != b.cgpa) return a.cgpa > b.cgpa;
        return a.backlogs < b.backlogs;
    });

    cout << "\n👑 🔥 ============ MERIT LEADERBOARD ============ 🔥 👑\n";
    cout << setw(6) << left << "Rank" 
         << setw(15) << left << "ID" 
         << setw(22) << left << "Student Name" 
         << setw(10) << left << "CGPA" 
         << setw(10) << left << "Branch" << "\n";
    printLine(63, '-');

    int rank = 1;
    for (const auto& s : sortedStudents) {
        cout << setw(6) << left << rank++
             << setw(15) << left << s.id
             << setw(22) << left << s.name
             << setw(10) << left << fixed << setprecision(2) << s.cgpa
             << setw(10) << left << s.branch << "\n";
    }
    printLine(63, '=');
}

void Portal::eligibilityCheck() const {
    cout << "\n--- Check Eligibility Status ---\n";
    string targetID;
    cout << "Enter Student ID to check: ";
    cin >> targetID;

    
    auto studentIt = find_if(students.begin(), students.end(), [&targetID](const Student& s) {
        return s.id == targetID;
    });

    if (studentIt == students.end()) {
        cout << "[Error] Student ID not found in the system.\n";
        return;
    }

    cout << "\nEligible Job Drives for " << studentIt->name << " (CGPA: " << studentIt->cgpa << "):\n";
    printLine(55, '-');
    
    int matchCount = 0;
    for (const auto& c : companies) {
        if (studentIt->cgpa >= c.minCGPA && studentIt->backlogs <= c.maxBacklogsAllowed) {
            cout << " ✅ " << setw(15) << left << c.name 
                 << " Role: " << setw(20) << left << c.role 
                 << " Package: " << c.ctc << " LPA\n";
            matchCount++;
        }
    }

    if (matchCount == 0) {
        cout << " ❌ No currently open job criteria matches this profile.\n";
    }
    printLine(55, '=');
}

void Portal::analytics() const {
    cout << "\n📊 ============ PORTAL PLACEMENT ANALYTICS ============\n";
    if (students.empty()) {
        cout << "Insufficient data to compute statistics.\n";
        return;
    }

    int totalStudents = students.size();
    int placedCount = 0;
    double maxCTC = 0.0;
    string dreamCompany = "None";

    for (const auto& s : students) {
        if (s.isPlaced) {
            placedCount++;
            for (const auto& c : companies) {
                if (c.name == s.placedCompany && c.ctc > maxCTC) {
                    maxCTC = c.ctc;
                    dreamCompany = c.name;
                }
            }
        }
    }

    double placementPercentage = ((double)placedCount / totalStudents) * 100.0;

    cout << " * Total Registered Batches  : " << totalStudents << " Students\n";
    cout << " * Total Placed Students     : " << placedCount << "\n";
    cout << " * Current Placement Ratio   : " << fixed << setprecision(1) << placementPercentage << "%\n";
    cout << " * Highest Package Secured   : " << fixed << setprecision(2) << maxCTC << " LPA (" << dreamCompany << ")\n";
    printLine(55, '=');
}