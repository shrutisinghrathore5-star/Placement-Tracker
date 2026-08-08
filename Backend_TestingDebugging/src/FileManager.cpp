#include "FileManager.hpp"
#include "Exceptions.hpp"
#include <fstream>
#include <sys/stat.h>

void FileManager::ensureDirectory(const std::string& dir) {
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) {
        // Directory does not exist - create it (POSIX mkdir, 0755 perms)
        mkdir(dir.c_str(), 0755);
    }
}

// ---------- Generic helpers (templated locally via lambdas would need C++ tricks,
// so we keep simple repetition per type for clarity - this is a teaching project) ----------

std::vector<Student> FileManager::loadStudents(const std::string& path) {
    std::vector<Student> result;
    std::ifstream in(path);
    if (!in.is_open()) return result; // no file yet => empty list, not an error
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        result.push_back(Student::deserialize(line));
    }
    return result;
}

void FileManager::saveStudents(const std::string& path, const std::vector<Student>& students) {
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) throw FileIOException("Could not open " + path + " for writing.");
    for (const auto& s : students) out << s.serialize() << "\n";
}

std::vector<Admin> FileManager::loadAdmins(const std::string& path) {
    std::vector<Admin> result;
    std::ifstream in(path);
    if (!in.is_open()) return result;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        result.push_back(Admin::deserialize(line));
    }
    return result;
}

void FileManager::saveAdmins(const std::string& path, const std::vector<Admin>& admins) {
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) throw FileIOException("Could not open " + path + " for writing.");
    for (const auto& a : admins) out << a.serialize() << "\n";
}

std::vector<Company> FileManager::loadCompanies(const std::string& path) {
    std::vector<Company> result;
    std::ifstream in(path);
    if (!in.is_open()) return result;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        result.push_back(Company::deserialize(line));
    }
    return result;
}

void FileManager::saveCompanies(const std::string& path, const std::vector<Company>& companies) {
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) throw FileIOException("Could not open " + path + " for writing.");
    for (const auto& c : companies) out << c.serialize() << "\n";
}

std::vector<Application> FileManager::loadApplications(const std::string& path) {
    std::vector<Application> result;
    std::ifstream in(path);
    if (!in.is_open()) return result;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        result.push_back(Application::deserialize(line));
    }
    return result;
}

void FileManager::saveApplications(const std::string& path, const std::vector<Application>& apps) {
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) throw FileIOException("Could not open " + path + " for writing.");
    for (const auto& a : apps) out << a.serialize() << "\n";
}

void FileManager::loadCounters(const std::string& path, int& nextCompanyId, int& nextAppId) {
    nextCompanyId = 1;
    nextAppId = 1;
    std::ifstream in(path);
    if (!in.is_open()) return;
    std::string key;
    char eq;
    int val;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;
        std::string k = line.substr(0, eqPos);
        int v = std::stoi(line.substr(eqPos + 1));
        if (k == "nextCompanyId") nextCompanyId = v;
        else if (k == "nextAppId") nextAppId = v;
    }
    (void)key; (void)eq; (void)val;
}

void FileManager::saveCounters(const std::string& path, int nextCompanyId, int nextAppId) {
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) throw FileIOException("Could not open " + path + " for writing.");
    out << "nextCompanyId=" << nextCompanyId << "\n";
    out << "nextAppId=" << nextAppId << "\n";
}
