#include "Application.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"

std::string statusToString(Status s) {
    switch (s) {
        case Status::Applied:     return "Applied";
        case Status::Shortlisted: return "Shortlisted";
        case Status::Selected:    return "Selected";
        case Status::Rejected:    return "Rejected";
    }
    return "Unknown";
}

Status statusFromString(const std::string& s) {
    if (s == "Applied") return Status::Applied;
    if (s == "Shortlisted") return Status::Shortlisted;
    if (s == "Selected") return Status::Selected;
    if (s == "Rejected") return Status::Rejected;
    throw InvalidInputException("Unknown application status: " + s);
}

std::string Application::serialize() const {
    std::vector<std::string> fields = {
        std::to_string(appId), std::to_string(studentRoll), std::to_string(companyId),
        statusToString(status), dateApplied
    };
    return Utils::join(fields, '|');
}

Application Application::deserialize(const std::string& line) {
    auto f = Utils::split(line, '|');
    if (f.size() < 5) throw FileIOException("Corrupt application record: " + line);
    Application a;
    a.appId = std::stoi(f[0]);
    a.studentRoll = std::stoi(f[1]);
    a.companyId = std::stoi(f[2]);
    a.status = statusFromString(f[3]);
    a.dateApplied = f[4];
    return a;
}
