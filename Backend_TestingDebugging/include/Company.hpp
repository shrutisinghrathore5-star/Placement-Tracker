#ifndef COMPANY_HPP
#define COMPANY_HPP

#include <string>
#include <vector>

class Student; // forward declaration to avoid a circular include

// Encapsulates the eligibility rules a company sets for applicants.
class EligibilityCriteria {
private:
    double minCgpa = 0.0;
    int maxBacklogs = 0;
    std::vector<std::string> allowedBranches; // empty => all branches allowed

public:
    EligibilityCriteria() = default;
    EligibilityCriteria(double cgpa, int backlogs, std::vector<std::string> branches)
        : minCgpa(cgpa), maxBacklogs(backlogs), allowedBranches(std::move(branches)) {}

    double getMinCgpa() const { return minCgpa; }
    int getMaxBacklogs() const { return maxBacklogs; }
    const std::vector<std::string>& getAllowedBranches() const { return allowedBranches; }

    void setMinCgpa(double c) { minCgpa = c; }
    void setMaxBacklogs(int b) { maxBacklogs = b; }
    void setAllowedBranches(const std::vector<std::string>& b) { allowedBranches = b; }

    bool isEligible(const Student& s) const;
    std::string branchesToString() const; // comma-joined, e.g. "CSE,IT,ECE" ("ALL" if empty)
    static std::vector<std::string> branchesFromString(const std::string& s);
    std::string toString() const; // human readable summary
};

class Company {
private:
    int companyId = 0;
    std::string name;
    std::string role;
    double package = 0.0; // in LPA
    EligibilityCriteria criteria;

public:
    Company() = default;
    Company(int id, std::string nm, std::string rl, double pkg, EligibilityCriteria crit)
        : companyId(id), name(std::move(nm)), role(std::move(rl)), package(pkg), criteria(std::move(crit)) {}

    int getId() const { return companyId; }
    const std::string& getName() const { return name; }
    const std::string& getRole() const { return role; }
    double getPackage() const { return package; }
    const EligibilityCriteria& getCriteria() const { return criteria; }
    EligibilityCriteria& getCriteria() { return criteria; }

    void setName(const std::string& n) { name = n; }
    void setRole(const std::string& r) { role = r; }
    void setPackage(double p) { package = p; }
    void setCriteria(const EligibilityCriteria& c) { criteria = c; }

    void displayInfo() const;

    // Persistence: "id|name|role|package|minCgpa|maxBacklogs|branch1,branch2,..."
    std::string serialize() const;
    static Company deserialize(const std::string& line);
};

#endif // COMPANY_HPP
