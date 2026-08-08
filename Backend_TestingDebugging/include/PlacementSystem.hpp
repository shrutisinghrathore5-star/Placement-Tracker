#ifndef PLACEMENTSYSTEM_HPP
#define PLACEMENTSYSTEM_HPP

#include <string>
#include <vector>
#include "Student.hpp"
#include "Admin.hpp"
#include "Company.hpp"
#include "Application.hpp"

class PlacementSystem {
private:
    std::string dataDir;
    std::vector<Student> students;
    std::vector<Admin> admins;
    std::vector<Company> companies;
    std::vector<Application> applications;

    int nextCompanyId = 1;
    int nextAppId = 1;

    // --- persistence ---
    void loadAll();
    void saveAll() const;

    // --- lookups (return pointers into the vectors so callers can mutate in place) ---
    Student* findStudentByRoll(int roll);
    Student* findStudentByUsername(const std::string& uname);
    Company* findCompanyById(int id);

    // --- top level flows ---
    void welcomeScreen();
    void adminLogin();
    void studentLogin();

    // --- admin features ---
    void adminMenu(Admin& admin);
    void addStudent();
    void updateStudent();
    void deleteStudent();
    void searchStudent();
    void listAllStudents() const;

    void addCompany();
    void updateCompany();
    void deleteCompany();
    void searchCompany();
    void listAllCompanies() const;

    void viewEligibleStudentsForCompany();
    void viewApplicationsAndStats() const;
    void manageApplicationStatus();
    void generateSummaryReport() const;

    // --- student features ---
    void studentMenu(Student& s);
    void viewProfile(const Student& s) const;
    void updateProfile(Student& s);
    void viewEligibleCompanies(const Student& s) const;
    void applyToCompany(Student& s);
    void trackApplications(const Student& s) const;
    void viewPlacementHistory(const Student& s) const;

public:
    explicit PlacementSystem(std::string dataDirectory);
    void run();
};

#endif // PLACEMENTSYSTEM_HPP
