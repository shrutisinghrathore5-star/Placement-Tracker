#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "User.hpp"
#include <string>

class Admin : public User {
private:
    std::string name;

public:
    Admin() = default;
    Admin(std::string uname, std::string pwd, std::string nm)
        : User(std::move(uname), std::move(pwd)), name(std::move(nm)) {}

    const std::string& getName() const { return name; }
    void setName(const std::string& n) { name = n; }

    std::string role() const override { return "Administrator"; }
    void displayInfo() const override;

    // Persistence: "username|password|name"
    std::string serialize() const;
    static Admin deserialize(const std::string& line);
};

#endif // ADMIN_HPP
