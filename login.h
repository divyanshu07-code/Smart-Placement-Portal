#ifndef LOGIN_H
#define LOGIN_H

#include <string>

class Login {
private:
    std::string credFile;
    std::string maskPassword(const std::string& password) const;

public:
    Login(const std::string& filename = "credentials.txt");
    bool authenticate();
    bool changePassword();
};

#endif // LOGIN_H
