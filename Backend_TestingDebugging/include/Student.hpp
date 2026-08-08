#ifndef STUDENT_HPP
#define STUDENT_HPP

#include "User.hpp"
#include <string>

class Student : public User {
private:
    int rollNo = 0;
    std::string name;
    std::string branch;
    double cgpa = 0.0;
    int backlogs = 0;
    bool placed = false;
    int placedCompanyId = -1;

public:
    Student() = default;
    Student(std::string uname, std::string pwd, int roll, std::string nm,
            std::string br, double cg, int back);

    // Getters (encapsulation: fields are private, accessed via these)
    int getRollNo() const { return rollNo; }
    const std::string& getName() const { return name; }
    const std::string& getBranch() const { return branch; }
    double getCgpa() const { return cgpa; }
    int getBacklogs() const { return backlogs; }
    bool isPlaced() const { return placed; }
    int getPlacedCompanyId() const { return placedCompanyId; }

    // Setters with light validation
    void setName(const std::string& n);
    void setBranch(const std::string& b);
    void setCgpa(double c);
    void setBacklogs(int b);
    void setPlaced(bool p, int companyId = -1);

    // Overrides (polymorphism)
    std::string role() const override { return "Student"; }
    void displayInfo() const override;

    // Persistence: "username|password|roll|name|branch|cgpa|backlogs|placed|placedCompanyId"
    std::string serialize() const;
    static Student deserialize(const std::string& line);
};

#endif // STUDENT_HPP
