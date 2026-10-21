#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include "Portal.h"

class Database {
private:
    std::string studentsFile;
    std::string companiesFile;

public:
    Database(const std::string& sFile = "students.txt", const std::string& cFile = "companies.txt");

    void saveStudents(const std::vector<Student>& students);
    std::vector<Student> loadStudents();

    void saveCompanies(const std::vector<Company>& companies);
    std::vector<Company> loadCompanies();
};

#endif // DATABASE_H
