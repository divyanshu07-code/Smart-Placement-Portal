#include "Portal.h"
#include "Database.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

namespace {
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string toLowerStr(const string& str) {
        string lower = str;
        transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
            return tolower(c);
        });
        return lower;
    }
}

Portal::Portal() {
    Database db;
    students = db.loadStudents();
    companies = db.loadCompanies();
}

void Portal::printLine(int width, char ch) const {
    cout << setfill(ch) << setw(width) << "" << setfill(' ') << "\n";
}

int Portal::findStudentIndex(const string& id) const {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].id == id) return static_cast<int>(i);
    }
    return -1;
}

int Portal::findCompanyIndex(const string& name) const {
    for (size_t i = 0; i < companies.size(); ++i) {
        if (toLowerStr(companies[i].name) == toLowerStr(name)) return static_cast<int>(i);
    }
    return -1;
}

void Portal::addStudent() {
    Student s;
    cout << "\n--- Add New Student Record ---\n";
    cout << "Enter Student ID: ";
    cin >> s.id;

    if (findStudentIndex(s.id) != -1) {
        cout << "[!] Error: Student ID '" << s.id << "' already exists in the portal.\n";
        clearInput();
        return;
    }

    clearInput();
    cout << "Enter Full Name: ";
    getline(cin, s.name);

    cout << "Enter Branch (e.g. CSE, ICT, ECE, MNC, AI): ";
    getline(cin, s.branch);

    while (true) {
        cout << "Enter CGPA (0.00 - 10.00): ";
        if (cin >> s.cgpa && s.cgpa >= 0.0 && s.cgpa <= 10.0) {
            break;
        }
        cout << "[!] Invalid CGPA. Please enter a number between 0.0 and 10.0.\n";
        clearInput();
    }

    while (true) {
        cout << "Enter Active Backlogs Count: ";
        if (cin >> s.backlogs && s.backlogs >= 0) {
            break;
        }
        cout << "[!] Invalid backlogs count. Please enter 0 or a positive integer.\n";
        clearInput();
    }

    s.isPlaced = false;
    s.placedCompany = "None";

    students.push_back(s);
    Database db;
    db.saveStudents(students);
    cout << "[+] Student '" << s.name << "' successfully registered!\n";
}

void Portal::displayStudents() const {
    cout << "\n" << setw(80) << left << "========================= REGISTERED STUDENTS =========================" << "\n";
    if (students.empty()) {
        cout << "No student records found. Please add students first.\n";
        printLine(80, '=');
        return;
    }

    cout << setw(12) << left << "ID" 
         << setw(20) << left << "Name" 
         << setw(10) << left << "Branch" 
         << setw(8)  << left << "CGPA" 
         << setw(10) << left << "Backlogs" 
         << setw(20) << left << "Status" << "\n";
    printLine(80, '-');

    for (const auto& s : students) {
        string statusStr = s.isPlaced ? ("Placed (" + s.placedCompany + ")") : "Unplaced";
        cout << setw(12) << left << s.id
             << setw(20) << left << s.name
             << setw(10) << left << s.branch
             << setw(8)  << left << fixed << setprecision(2) << s.cgpa
             << setw(10) << left << s.backlogs
             << setw(20) << left << statusStr << "\n";
    }
    printLine(80, '=');
}

void Portal::searchStudent() const {
    cout << "\n--- Search Student ---\n";
    cout << "Enter Student ID or Name: ";
    string query;
    cin.ignore();
    getline(cin, query);

    if (query.empty()) return;

    string qLower = toLowerStr(query);
    bool found = false;

    cout << "\n" << setw(75) << left << "--------------------------- SEARCH RESULTS ---------------------------" << "\n";
    for (const auto& s : students) {
        if (s.id == query || toLowerStr(s.name).find(qLower) != string::npos) {
            if (!found) {
                cout << setw(12) << left << "ID" 
                     << setw(20) << left << "Name" 
                     << setw(10) << left << "Branch" 
                     << setw(8)  << left << "CGPA" 
                     << setw(10) << left << "Backlogs" 
                     << setw(15) << left << "Status" << "\n";
                printLine(75, '-');
                found = true;
            }
            string status = s.isPlaced ? ("Placed (" + s.placedCompany + ")") : "Unplaced";
            cout << setw(12) << left << s.id
                 << setw(20) << left << s.name
                 << setw(10) << left << s.branch
                 << setw(8)  << left << fixed << setprecision(2) << s.cgpa
                 << setw(10) << left << s.backlogs
                 << setw(15) << left << status << "\n";
        }
    }

    if (!found) {
        cout << "[!] No student found matching: " << query << "\n";
    }
    printLine(75, '=');
}

void Portal::deleteStudent() {
    cout << "\n--- Delete Student Record ---\n";
    cout << "Enter Student ID to delete: ";
    string id;
    cin >> id;

    int idx = findStudentIndex(id);
    if (idx == -1) {
        cout << "[!] Error: Student with ID '" << id << "' not found.\n";
        return;
    }

    char confirm;
    cout << "Are you sure you want to remove " << students[idx].name << " (" << id << ")? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        students.erase(students.begin() + idx);
        Database db;
        db.saveStudents(students);
        cout << "[+] Student record removed successfully.\n";
    } else {
        cout << "[-] Deletion cancelled.\n";
    }
}

void Portal::markStudentPlaced() {
    cout << "\n--- Mark Student as Placed ---\n";
    cout << "Enter Student ID: ";
    string id;
    cin >> id;

    int idx = findStudentIndex(id);
    if (idx == -1) {
        cout << "[!] Error: Student ID '" << id << "' not found.\n";
        return;
    }

    clearInput();
    cout << "Selected Student: " << students[idx].name << " (" << students[idx].branch << ")\n";
    cout << "Enter Recruiter / Company Name: ";
    string companyName;
    getline(cin, companyName);

    if (companyName.empty()) {
        cout << "[!] Company name cannot be blank.\n";
        return;
    }

    students[idx].isPlaced = true;
    students[idx].placedCompany = companyName;

    Database db;
    db.saveStudents(students);
    cout << "[+] Congratulations! " << students[idx].name << " marked as Placed at " << companyName << ".\n";
}

void Portal::addCompany() {
    Company c;
    cout << "\n--- Add New Recruiting Company ---\n";
    clearInput();

    cout << "Enter Company Name: ";
    getline(cin, c.name);

    if (findCompanyIndex(c.name) != -1) {
        cout << "[!] Error: Company '" << c.name << "' already exists.\n";
        return;
    }

    cout << "Enter Job Profile / Role: ";
    getline(cin, c.role);

    while (true) {
        cout << "Enter Minimum CGPA Cutoff (0.00 - 10.00): ";
        if (cin >> c.minCGPA && c.minCGPA >= 0.0 && c.minCGPA <= 10.0) break;
        cout << "[!] Invalid CGPA cutoff.\n";
        clearInput();
    }

    while (true) {
        cout << "Enter Max Backlogs Allowed: ";
        if (cin >> c.maxBacklogsAllowed && c.maxBacklogsAllowed >= 0) break;
        cout << "[!] Invalid backlogs limit.\n";
        clearInput();
    }

    while (true) {
        cout << "Enter CTC Package Offered (in LPA): ";
        if (cin >> c.ctc && c.ctc > 0.0) break;
        cout << "[!] Invalid CTC package.\n";
        clearInput();
    }

    companies.push_back(c);
    Database db;
    db.saveCompanies(companies);
    cout << "[+] Company '" << c.name << "' registered successfully!\n";
}

void Portal::displayCompanies() const {
    cout << "\n" << setw(75) << left << "======================== RECRUITING COMPANIES ========================" << "\n";
    if (companies.empty()) {
        cout << "No company drives registered yet.\n";
        printLine(75, '=');
        return;
    }

    cout << setw(20) << left << "Company Name" 
         << setw(22) << left << "Role" 
         << setw(12) << left << "Min CGPA" 
         << setw(10) << left << "Max Back" 
         << setw(10) << left << "CTC (LPA)" << "\n";
    printLine(75, '-');

    for (const auto& c : companies) {
        cout << setw(20) << left << c.name
             << setw(22) << left << c.role
             << setw(12) << left << fixed << setprecision(2) << c.minCGPA
             << setw(10) << left << c.maxBacklogsAllowed
             << setw(10) << left << c.ctc << "\n";
    }
    printLine(75, '=');
}

void Portal::deleteCompany() {
    cout << "\n--- Delete Recruiting Company ---\n";
    clearInput();
    cout << "Enter Company Name to remove: ";
    string name;
    getline(cin, name);

    int idx = findCompanyIndex(name);
    if (idx == -1) {
        cout << "[!] Company '" << name << "' not found.\n";
        return;
    }

    companies.erase(companies.begin() + idx);
    Database db;
    db.saveCompanies(companies);
    cout << "[+] Company '" << name << "' removed from recruitment drives.\n";
}

void Portal::leaderboard() const {
    if (students.empty()) {
        cout << "\n[!] No students available to generate leaderboard.\n";
        return;
    }

    vector<Student> sortedStudents = students;
    
    // Sort logic: Highest CGPA first -> Tie breaker: Lowest backlogs -> Tie breaker 2: Name alphabetical
    sort(sortedStudents.begin(), sortedStudents.end(), [](const Student& a, const Student& b) {
        if (a.cgpa != b.cgpa) return a.cgpa > b.cgpa;
        if (a.backlogs != b.backlogs) return a.backlogs < b.backlogs;
        return a.name < b.name;
    });

    cout << "\n" << setw(75) << left << "👑 ======================== MERIT LEADERBOARD ======================== 👑" << "\n";
    cout << setw(6)  << left << "Rank" 
         << setw(12) << left << "ID" 
         << setw(22) << left << "Student Name" 
         << setw(10) << left << "Branch" 
         << setw(8)  << left << "CGPA" 
         << setw(10) << left << "Backlogs" << "\n";
    printLine(75, '-');

    int rank = 1;
    for (const auto& s : sortedStudents) {
        cout << setw(6)  << left << rank++
             << setw(12) << left << s.id
             << setw(22) << left << s.name
             << setw(10) << left << s.branch
             << setw(8)  << left << fixed << setprecision(2) << s.cgpa
             << setw(10) << left << s.backlogs << "\n";
    }
    printLine(75, '=');
}

void Portal::eligibilityCheck() const {
    cout << "\n--- Check Student Eligibility ---\n";
    cout << "Enter Student ID: ";
    string targetID;
    cin >> targetID;

    int idx = findStudentIndex(targetID);
    if (idx == -1) {
        cout << "[!] Error: Student ID not found in the system.\n";
        return;
    }

    const auto& student = students[idx];
    cout << "\nEligibility Report for " << student.name << " (" << student.id << ")\n";
    cout << "Branch: " << student.branch << " | CGPA: " << fixed << setprecision(2) << student.cgpa 
         << " | Active Backlogs: " << student.backlogs << "\n";
    printLine(70, '-');
    
    int matchCount = 0;
    for (const auto& c : companies) {
        if (student.cgpa >= c.minCGPA && student.backlogs <= c.maxBacklogsAllowed) {
            cout << "  [ELIGIBLE] " << setw(18) << left << c.name 
                 << " Role: " << setw(20) << left << c.role 
                 << " CTC: " << fixed << setprecision(2) << c.ctc << " LPA\n";
            matchCount++;
        }
    }

    if (matchCount == 0) {
        cout << "  [x] No currently registered company drives match this profile.\n";
    } else {
        cout << "\nTotal Eligible Drives: " << matchCount << "\n";
    }
    printLine(70, '=');
}

void Portal::companyShortlist() const {
    cout << "\n--- Generate Company Drive Shortlist ---\n";
    clearInput();
    cout << "Enter Company Name: ";
    string compName;
    getline(cin, compName);

    int cIdx = findCompanyIndex(compName);
    if (cIdx == -1) {
        cout << "[!] Company '" << compName << "' not found.\n";
        return;
    }

    const auto& comp = companies[cIdx];
    cout << "\nShortlisted Candidates for " << comp.name << " (" << comp.role << " - " << comp.ctc << " LPA)\n";
    cout << "Criteria: Min CGPA: " << comp.minCGPA << " | Max Backlogs: " << comp.maxBacklogsAllowed << "\n";
    printLine(70, '-');

    int eligibleCount = 0;
    for (const auto& s : students) {
        if (s.cgpa >= comp.minCGPA && s.backlogs <= comp.maxBacklogsAllowed) {
            eligibleCount++;
            cout << setw(4) << left << eligibleCount
                 << setw(12) << left << s.id
                 << setw(22) << left << s.name
                 << setw(10) << left << s.branch
                 << "CGPA: " << fixed << setprecision(2) << s.cgpa << "\n";
        }
    }

    if (eligibleCount == 0) {
        cout << "No registered students currently meet this cutoff.\n";
    } else {
        cout << "\nTotal Qualified Students: " << eligibleCount << " / " << students.size() << "\n";
    }
    printLine(70, '=');
}

void Portal::analytics() const {
    cout << "\n" << setw(65) << left << "📊 ================== PLACEMENT ANALYTICS ================== 📊" << "\n";
    if (students.empty()) {
        cout << "Insufficient data: No student records registered.\n";
        printLine(65, '=');
        return;
    }

    int totalStudents = static_cast<int>(students.size());
    int placedCount = 0;
    double highestCTC = 0.0;
    string highestCTCCompany = "N/A";
    double totalPlacedCTC = 0.0;
    int ctcKnownCount = 0;

    for (const auto& s : students) {
        if (s.isPlaced) {
            placedCount++;
            // Check company CTC if placed in registered company
            for (const auto& c : companies) {
                if (toLowerStr(c.name) == toLowerStr(s.placedCompany)) {
                    totalPlacedCTC += c.ctc;
                    ctcKnownCount++;
                    if (c.ctc > highestCTC) {
                        highestCTC = c.ctc;
                        highestCTCCompany = c.name;
                    }
                    break;
                }
            }
        }
    }

    double placementPercentage = (static_cast<double>(placedCount) / totalStudents) * 100.0;
    double avgCTC = (ctcKnownCount > 0) ? (totalPlacedCTC / ctcKnownCount) : 0.0;

    cout << " • Total Registered Students : " << totalStudents << "\n";
    cout << " • Total Placed Students     : " << placedCount << "\n";
    cout << " • Unplaced Students         : " << (totalStudents - placedCount) << "\n";
    cout << " • Overall Placement Ratio   : " << fixed << setprecision(1) << placementPercentage << "%\n";
    cout << " • Total Recruiting Drives   : " << companies.size() << "\n";
    
    if (highestCTC > 0.0) {
        cout << " • Highest Package Secured   : " << fixed << setprecision(2) << highestCTC << " LPA (" << highestCTCCompany << ")\n";
    }
    if (avgCTC > 0.0) {
        cout << " • Average Package (Known)   : " << fixed << setprecision(2) << avgCTC << " LPA\n";
    }
    printLine(65, '=');
}
