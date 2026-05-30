
 #include "Database.h"
#include <iostream>
#include <sstream>

using namespace std;

void Database::saveStudents(const vector<Student>& students) {
    ofstream file("students.txt");
    if (!file) {
        cerr << "⚠️ [System Error] Disk Write Failed: Cannot open students.txt\n";
        return;
    }
    
    for (const auto& s : students) {
        file << s.id << "|" << s.name << "|" << s.branch << "|" 
             << s.cgpa << "|" << s.backlogs << "|" << (s.isPlaced ? 1 : 0) << "|" 
             << s.placedCompany << "\n";
    }
    file.close();
}

void Database::saveCompanies(const vector<Company>& companies) {
    ofstream file("companies.txt");
    if (!file) {
        cerr << "⚠️ [System Error] Disk Write Failed: Cannot open companies.txt\n";
        return;
    }
    
    for (const auto& c : companies) {
        file << c.name << "|" << c.role << "|" << c.minCGPA << "|" 
             << c.maxBacklogsAllowed << "|" << c.ctc << "\n";
    }
    file.close();
}

vector<Student> Database::loadStudents() {
    vector<Student> list;
    ifstream file("students.txt");
    if (!file) return list; 
    
    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;

        stringstream ss(line);
        string id, name, branch, company, temp;
        double cgpa;
        int backlogs, isPlacedInt;

        try {
            if (!getline(ss, id, '|')) throw runtime_error("Missing ID");
            if (!getline(ss, name, '|')) throw runtime_error("Missing Name");
            if (!getline(ss, branch, '|')) throw runtime_error("Missing Branch");
            
            if (!getline(ss, temp, '|')) throw runtime_error("Missing CGPA");
            cgpa = stod(temp);
            
            if (!getline(ss, temp, '|')) throw runtime_error("Missing Backlogs");
            backlogs = stoi(temp); 
            
            if (!getline(ss, temp, '|')) throw runtime_error("Missing Placement Status");
            isPlacedInt = stoi(temp);
            
            if (!getline(ss, company)) company = "None";

            list.push_back({id, name, branch, cgpa, backlogs, (isPlacedInt == 1), company});
        } 
        catch (const exception& e) {
        
            
            cerr << "⚙️ [Data Corruption Corrupted] Skipped bad record at line " << lineNum << " - Reason: " << e.what() << "\n";
        }
    }
    file.close();
    return list;
}

vector<Company> Database::loadCompanies() {
    vector<Company> list;
    ifstream file("companies.txt");
    if (!file) return list;
    
    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;

        stringstream ss(line);
        string name, role, temp;
        double minCGPA, ctc;
        int maxBacklogs;

        try {
            if (!getline(ss, name, '|')) throw runtime_error("Missing Company Name");
            if (!getline(ss, role, '|')) throw runtime_error("Missing Role");
            
            if (!getline(ss, temp, '|')) throw runtime_error("Missing CGPA Cutoff");
            minCGPA = stod(temp);
            
            if (!getline(ss, temp, '|')) throw runtime_error("Missing Max Backlogs");
            maxBacklogs = stoi(temp);
            
            if (!getline(ss, temp)) throw runtime_error("Missing Package Info");
            ctc = stod(temp);

            list.push_back({name, role, minCGPA, maxBacklogs, ctc});
        } 
        catch (const exception& e) {
            cerr << "⚙️ [Data Corruption Warning] Skipped bad company drive entry at line " << lineNum << " - Reason: " << e.what() << "\n";
        }
    }
    file.close();
    return list;
}
