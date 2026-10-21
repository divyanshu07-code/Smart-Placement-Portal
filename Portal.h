#ifndef PORTAL_H
#define PORTAL_H

#include <string>
#include <vector>

struct Student {
    std::string id;
    std::string name;
    std::string branch;
    double cgpa;
    int backlogs;
    bool isPlaced;
    std::string placedCompany;
};

struct Company {
    std::string name;
    std::string role;
    double minCGPA;
    int maxBacklogsAllowed;
    double ctc; // in LPA
};

class Portal {
private:
    std::vector<Student> students;
    std::vector<Company> companies;

    void printLine(int width, char ch = '-') const;
    int findStudentIndex(const std::string& id) const;
    int findCompanyIndex(const std::string& name) const;

public:
    Portal();

    // Student Management
    void addStudent();
    void displayStudents() const;
    void searchStudent() const;
    void deleteStudent();
    void markStudentPlaced();

    // Company Management
    void addCompany();
    void displayCompanies() const;
    void deleteCompany();

    // Drives & Analytics
    void leaderboard() const;
    void eligibilityCheck() const;
    void companyShortlist() const;
    void analytics() const;
};

#endif // PORTAL_H
