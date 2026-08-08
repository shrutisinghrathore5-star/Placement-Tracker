#include "Company.hpp"
#include "Student.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

bool EligibilityCriteria::isEligible(const Student& s) const {
    if (s.getCgpa() < minCgpa) return false;
    if (s.getBacklogs() > maxBacklogs) return false;
    if (!allowedBranches.empty()) {
        bool found = std::any_of(allowedBranches.begin(), allowedBranches.end(),
            [&](const std::string& b) { return Utils::toUpper(b) == Utils::toUpper(s.getBranch()); });
        if (!found) return false;
    }
    return true;
}

std::string EligibilityCriteria::branchesToString() const {
    if (allowedBranches.empty()) return "ALL";
    std::ostringstream oss;
    for (size_t i = 0; i < allowedBranches.size(); ++i) {
        oss << allowedBranches[i];
        if (i + 1 < allowedBranches.size()) oss << ",";
    }
    return oss.str();
}

std::vector<std::string> EligibilityCriteria::branchesFromString(const std::string& s) {
    if (s == "ALL" || s.empty()) return {};
    auto parts = Utils::split(s, ',');
    for (auto& p : parts) p = Utils::trim(p);
    return parts;
}

std::string EligibilityCriteria::toString() const {
    std::ostringstream oss;
    oss << "Min CGPA: " << std::fixed << std::setprecision(2) << minCgpa
        << ", Max Backlogs: " << maxBacklogs
        << ", Branches: " << branchesToString();
    return oss.str();
}

void Company::displayInfo() const {
    std::cout << std::left
               << "Company ID : " << companyId << "\n"
               << "Name       : " << name << "\n"
               << "Role       : " << role << "\n"
               << "Package    : " << std::fixed << std::setprecision(2) << package << " LPA\n"
               << "Eligibility: " << criteria.toString() << "\n";
}

std::string Company::serialize() const {
    std::vector<std::string> fields = {
        std::to_string(companyId), name, role, std::to_string(package),
        std::to_string(criteria.getMinCgpa()), std::to_string(criteria.getMaxBacklogs()),
        criteria.branchesToString()
    };
    return Utils::join(fields, '|');
}

Company Company::deserialize(const std::string& line) {
    auto f = Utils::split(line, '|');
    if (f.size() < 7) throw FileIOException("Corrupt company record: " + line);
    Company c;
    c.companyId = std::stoi(f[0]);
    c.name = f[1];
    c.role = f[2];
    c.package = std::stod(f[3]);
    c.criteria.setMinCgpa(std::stod(f[4]));
    c.criteria.setMaxBacklogs(std::stoi(f[5]));
    c.criteria.setAllowedBranches(EligibilityCriteria::branchesFromString(f[6]));
    return c;
}
