#include "Database.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Database::Database(const string& sFile, const string& cFile)
    : studentsFile(sFile), companiesFile(cFile) {}

void Database::saveStudents(const vector<Student>& students) {
    ofstream file(studentsFile);
    if (!file) {
        cerr << "[!] System Error: Cannot write to " << studentsFile << "\n";
        return;
    }
    
    for (const auto& s : students) {
        file << s.id << "|" << s.name << "|" << s.branch << "|" 
             << s.cgpa << "|" << s.backlogs << "|" << (s.isPlaced ? 1 : 0) << "|" 
             << s.placedCompany << "\n";
    }
    file.close();
}

vector<Student> Database::loadStudents() {
    vector<Student> list;
    ifstream file(studentsFile);
    if (!file) return list; 
    
    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;

        stringstream ss(line);
        string id, name, branch, company, temp;
        double cgpa = 0.0;
        int backlogs = 0, isPlacedInt = 0;

        try {
            if (!getline(ss, id, '|')) throw runtime_error("Missing Student ID");
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
            cerr << "[!] Warning: Skipped invalid student record at line " << lineNum << " (" << e.what() << ")\n";
        }
    }
    file.close();
    return list;
}

void Database::saveCompanies(const vector<Company>& companies) {
    ofstream file(companiesFile);
    if (!file) {
        cerr << "[!] System Error: Cannot write to " << companiesFile << "\n";
        return;
    }
    
    for (const auto& c : companies) {
        file << c.name << "|" << c.role << "|" << c.minCGPA << "|" 
             << c.maxBacklogsAllowed << "|" << c.ctc << "\n";
    }
    file.close();
}

vector<Company> Database::loadCompanies() {
    vector<Company> list;
    ifstream file(companiesFile);
    if (!file) return list;
    
    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;

        stringstream ss(line);
        string name, role, temp;
        double minCGPA = 0.0, ctc = 0.0;
        int maxBacklogs = 0;

        try {
            if (!getline(ss, name, '|')) throw runtime_error("Missing Company Name");
            if (!getline(ss, role, '|')) throw runtime_error("Missing Role");
            
            if (!getline(ss, temp, '|')) throw runtime_error("Missing CGPA Cutoff");
            minCGPA = stod(temp);
            
            if (!getline(ss, temp, '|')) throw runtime_error("Missing Max Backlogs");
            maxBacklogs = stoi(temp);
            
            if (!getline(ss, temp)) throw runtime_error("Missing CTC Package");
            ctc = stod(temp);

            list.push_back({name, role, minCGPA, maxBacklogs, ctc});
        } 
        catch (const exception& e) {
            cerr << "[!] Warning: Skipped invalid company record at line " << lineNum << " (" << e.what() << ")\n";
        }
    }
    file.close();
    return list;
}
