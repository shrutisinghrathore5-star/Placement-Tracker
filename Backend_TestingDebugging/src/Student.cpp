#include "Student.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>

Student::Student(std::string uname, std::string pwd, int roll, std::string nm,
                  std::string br, double cg, int back)
    : User(std::move(uname), std::move(pwd)), rollNo(roll), name(std::move(nm)),
      branch(std::move(br)), cgpa(cg), backlogs(back) {
    if (cgpa < 0.0 || cgpa > 10.0)
        throw InvalidInputException("CGPA must be between 0 and 10.");
    if (backlogs < 0)
        throw InvalidInputException("Backlogs cannot be negative.");
}

void Student::setName(const std::string& n) {
    if (n.empty()) throw InvalidInputException("Name cannot be empty.");
    name = n;
}

void Student::setBranch(const std::string& b) {
    if (b.empty()) throw InvalidInputException("Branch cannot be empty.");
    branch = b;
}

void Student::setCgpa(double c) {
    if (c < 0.0 || c > 10.0) throw InvalidInputException("CGPA must be between 0 and 10.");
    cgpa = c;
}

void Student::setBacklogs(int b) {
    if (b < 0) throw InvalidInputException("Backlogs cannot be negative.");
    backlogs = b;
}

void Student::setPlaced(bool p, int companyId) {
    placed = p;
    placedCompanyId = p ? companyId : -1;
}

void Student::displayInfo() const {
    std::cout << std::left
               << "Roll No   : " << rollNo << "\n"
               << "Name      : " << name << "\n"
               << "Branch    : " << branch << "\n"
               << "CGPA      : " << std::fixed << std::setprecision(2) << cgpa << "\n"
               << "Backlogs  : " << backlogs << "\n"
               << "Placed    : " << (placed ? "Yes" : "No");
    if (placed) std::cout << " (Company ID: " << placedCompanyId << ")";
    std::cout << "\n";
}

std::string Student::serialize() const {
    std::vector<std::string> fields = {
        username, password, std::to_string(rollNo), name, branch,
        std::to_string(cgpa), std::to_string(backlogs),
        placed ? "1" : "0", std::to_string(placedCompanyId)
    };
    return Utils::join(fields, '|');
}

Student Student::deserialize(const std::string& line) {
    auto f = Utils::split(line, '|');
    if (f.size() < 9) throw FileIOException("Corrupt student record: " + line);
    Student s;
    s.username = f[0];
    s.password = f[1];
    s.rollNo = std::stoi(f[2]);
    s.name = f[3];
    s.branch = f[4];
    s.cgpa = std::stod(f[5]);
    s.backlogs = std::stoi(f[6]);
    s.placed = (f[7] == "1");
    s.placedCompanyId = std::stoi(f[8]);
    return s;
}
