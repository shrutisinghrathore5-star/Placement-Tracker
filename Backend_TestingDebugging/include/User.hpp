#ifndef USER_HPP
#define USER_HPP

#include <string>

// Abstract base class for anyone who can log into the system.
// Student and Admin both derive from this - classic OOP inheritance,
// with displayRole()/displayMenuTitle() providing runtime polymorphism.
class User {
protected:
    std::string username;
    std::string password; // stored as-is for a college project; a real system would hash this

public:
    User() = default;
    User(std::string uname, std::string pwd)
        : username(std::move(uname)), password(std::move(pwd)) {}

    virtual ~User() = default;

    // Encapsulated accessors
    const std::string& getUsername() const { return username; }
    void setUsername(const std::string& u) { username = u; }

    void setPassword(const std::string& p) { password = p; }
    bool checkPassword(const std::string& attempt) const { return password == attempt; }

    // Pure virtual -> forces every concrete user type to identify itself.
    virtual std::string role() const = 0;

    // Pure virtual -> each user type prints its own profile summary.
    virtual void displayInfo() const = 0;
};

#endif // USER_HPP
