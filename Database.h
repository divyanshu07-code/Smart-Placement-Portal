#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <fstream>
#include "Portal.h"

class Database {
public:
    void saveStudents(const std::vector<Student>& students);
    void saveCompanies(const std::vector<Company>& companies);
    
    std::vector<Student> loadStudents();
    std::vector<Company> loadCompanies();
};

#endif