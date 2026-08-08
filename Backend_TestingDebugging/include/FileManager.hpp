#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <string>
#include <vector>
#include "Student.hpp"
#include "Admin.hpp"
#include "Company.hpp"
#include "Application.hpp"

// A pure static utility class responsible for all file handling.
// Keeping I/O in one place is what makes the rest of the program
// storage-agnostic (modular programming).
class FileManager {
public:
    static std::vector<Student> loadStudents(const std::string& path);
    static void saveStudents(const std::string& path, const std::vector<Student>& students);

    static std::vector<Admin> loadAdmins(const std::string& path);
    static void saveAdmins(const std::string& path, const std::vector<Admin>& admins);

    static std::vector<Company> loadCompanies(const std::string& path);
    static void saveCompanies(const std::string& path, const std::vector<Company>& companies);

    static std::vector<Application> loadApplications(const std::string& path);
    static void saveApplications(const std::string& path, const std::vector<Application>& apps);

    // Simple "key=value" counters file used to persist auto-increment ids.
    static void loadCounters(const std::string& path, int& nextCompanyId, int& nextAppId);
    static void saveCounters(const std::string& path, int nextCompanyId, int nextAppId);

    // Creates the directory if it does not already exist.
    static void ensureDirectory(const std::string& dir);
};

#endif // FILEMANAGER_HPP
