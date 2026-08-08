#include "PlacementSystem.hpp"
#include "FileManager.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>

// ============================================================
// Construction / persistence
// ============================================================

PlacementSystem::PlacementSystem(std::string dataDirectory) : dataDir(std::move(dataDirectory)) {
    loadAll();
    if (admins.empty()) {
        // First-run convenience: seed a default administrator account.
        admins.emplace_back("admin", "admin123", "Default Administrator");
        std::cout << "[Setup] No administrator account found - created default admin.\n";
        std::cout << "        Username: admin   Password: admin123\n";
        std::cout << "        (Please change this in a real deployment.)\n\n";
        saveAll();
    }
}

void PlacementSystem::loadAll() {
    FileManager::ensureDirectory(dataDir);
    students = FileManager::loadStudents(dataDir + "/students.txt");
    admins = FileManager::loadAdmins(dataDir + "/admins.txt");
    companies = FileManager::loadCompanies(dataDir + "/companies.txt");
    applications = FileManager::loadApplications(dataDir + "/applications.txt");
    FileManager::loadCounters(dataDir + "/counters.txt", nextCompanyId, nextAppId);
}

void PlacementSystem::saveAll() const {
    FileManager::saveStudents(dataDir + "/students.txt", students);
    FileManager::saveAdmins(dataDir + "/admins.txt", admins);
    FileManager::saveCompanies(dataDir + "/companies.txt", companies);
    FileManager::saveApplications(dataDir + "/applications.txt", applications);
    FileManager::saveCounters(dataDir + "/counters.txt", nextCompanyId, nextAppId);
}

// ============================================================
// Lookups
// ============================================================

Student* PlacementSystem::findStudentByRoll(int roll) {
    auto it = std::find_if(students.begin(), students.end(),
        [roll](const Student& s) { return s.getRollNo() == roll; });
    return it == students.end() ? nullptr : &(*it);
}

Student* PlacementSystem::findStudentByUsername(const std::string& uname) {
    auto it = std::find_if(students.begin(), students.end(),
        [&](const Student& s) { return s.getUsername() == uname; });
    return it == students.end() ? nullptr : &(*it);
}

Company* PlacementSystem::findCompanyById(int id) {
    auto it = std::find_if(companies.begin(), companies.end(),
        [id](const Company& c) { return c.getId() == id; });
    return it == companies.end() ? nullptr : &(*it);
}

// ============================================================
// Top level flow
// ============================================================

void PlacementSystem::run() {
    while (true) {
        welcomeScreen();
        int choice = Utils::readInt("Enter choice: ", 1, 3);
        try {
            if (choice == 1) adminLogin();
            else if (choice == 2) studentLogin();
            else {
                saveAll();
                std::cout << "\nData saved. Goodbye!\n";
                break;
            }
        } catch (const PlacementException& ex) {
            std::cout << "\n[Error] " << ex.what() << "\n";
            Utils::pause();
        }
    }
}

void PlacementSystem::welcomeScreen() {
    std::cout << "\n==============================================\n";
    std::cout << "     COLLEGE PLACEMENT TRACKER SYSTEM\n";
    std::cout << "==============================================\n";
    std::cout << "  1. Administrator Login\n";
    std::cout << "  2. Student Login\n";
    std::cout << "  3. Exit\n";
    std::cout << "----------------------------------------------\n";
}

void PlacementSystem::adminLogin() {
    std::cout << "\n-- Administrator Login --\n";
    std::string uname = Utils::readLine("Username: ");
    std::string pwd = Utils::readLine("Password: ");

    auto it = std::find_if(admins.begin(), admins.end(),
        [&](const Admin& a) { return a.getUsername() == uname; });
    if (it == admins.end() || !it->checkPassword(pwd))
        throw InvalidLoginException("Incorrect administrator username or password.");

    std::cout << "\nLogin successful. Welcome, " << it->getName() << "!\n";
    adminMenu(*it);
}

void PlacementSystem::studentLogin() {
    std::cout << "\n-- Student Login --\n";
    std::string uname = Utils::readLine("Username: ");
    std::string pwd = Utils::readLine("Password: ");

    Student* s = findStudentByUsername(uname);
    if (!s || !s->checkPassword(pwd))
        throw InvalidLoginException("Incorrect student username or password.");

    std::cout << "\nLogin successful. Welcome, " << s->getName() << "!\n";
    studentMenu(*s);
}

// ============================================================
// Admin menu
// ============================================================

void PlacementSystem::adminMenu(Admin& admin) {
    (void)admin;
    while (true) {
        std::cout << "\n---------------- ADMIN MENU ----------------\n";
        std::cout << "  1.  Add Student\n";
        std::cout << "  2.  Update Student\n";
        std::cout << "  3.  Delete Student\n";
        std::cout << "  4.  Search Student\n";
        std::cout << "  5.  List All Students\n";
        std::cout << "  6.  Add Company\n";
        std::cout << "  7.  Update Company\n";
        std::cout << "  8.  Delete Company\n";
        std::cout << "  9.  Search Company\n";
        std::cout << "  10. List All Companies\n";
        std::cout << "  11. View Eligible Students for a Company\n";
        std::cout << "  12. View Applications & Placement Statistics\n";
        std::cout << "  13. Update Application Status (shortlist/select/reject)\n";
        std::cout << "  14. Generate Summary Report\n";
        std::cout << "  15. Logout\n";
        std::cout << "---------------------------------------------\n";
        int choice = Utils::readInt("Enter choice: ", 1, 15);

        try {
            switch (choice) {
                case 1: addStudent(); break;
                case 2: updateStudent(); break;
                case 3: deleteStudent(); break;
                case 4: searchStudent(); break;
                case 5: listAllStudents(); break;
                case 6: addCompany(); break;
                case 7: updateCompany(); break;
                case 8: deleteCompany(); break;
                case 9: searchCompany(); break;
                case 10: listAllCompanies(); break;
                case 11: viewEligibleStudentsForCompany(); break;
                case 12: viewApplicationsAndStats(); break;
                case 13: manageApplicationStatus(); break;
                case 14: generateSummaryReport(); break;
                case 15:
                    saveAll();
                    std::cout << "Logged out.\n";
                    return;
            }
        } catch (const PlacementException& ex) {
            std::cout << "\n[Error] " << ex.what() << "\n";
        }
        Utils::pause();
    }
}

// ------------------------------------------------------------
// Student management (admin side)
// ------------------------------------------------------------

void PlacementSystem::addStudent() {
    std::cout << "\n-- Add Student --\n";
    std::string uname = Utils::readLine("Username: ");
    if (findStudentByUsername(uname))
        throw DuplicateRecordException("A student with that username already exists.");

    int roll = Utils::readInt("Roll No: ", 1, 999999999);
    if (findStudentByRoll(roll))
        throw DuplicateRecordException("A student with that roll number already exists.");

    std::string pwd = Utils::readLine("Password: ");
    std::string name = Utils::readLine("Full Name: ");
    std::string branch = Utils::readLine("Branch (e.g. CSE/IT/ECE): ");
    double cgpa = Utils::readDouble("CGPA (0-10): ", 0.0, 10.0);
    int backlogs = Utils::readInt("Number of active backlogs: ", 0, 100);

    students.emplace_back(uname, pwd, roll, name, branch, cgpa, backlogs);
    saveAll();
    std::cout << "Student added successfully.\n";
}

void PlacementSystem::updateStudent() {
    std::cout << "\n-- Update Student --\n";
    int roll = Utils::readInt("Enter Roll No to update: ", 1, 999999999);
    Student* s = findStudentByRoll(roll);
    if (!s) throw RecordNotFoundException("No student found with roll number " + std::to_string(roll));

    std::cout << "Current record:\n";
    s->displayInfo();
    std::cout << "\nLeave a field blank to keep its current value.\n";

    std::string name = Utils::readLine("New Name [" + s->getName() + "]: ", true);
    if (!name.empty()) s->setName(name);

    std::string branch = Utils::readLine("New Branch [" + s->getBranch() + "]: ", true);
    if (!branch.empty()) s->setBranch(branch);

    std::string cgpaStr = Utils::readLine("New CGPA [" + std::to_string(s->getCgpa()) + "]: ", true);
    if (!cgpaStr.empty()) s->setCgpa(std::stod(cgpaStr));

    std::string backStr = Utils::readLine("New Backlogs [" + std::to_string(s->getBacklogs()) + "]: ", true);
    if (!backStr.empty()) s->setBacklogs(std::stoi(backStr));

    saveAll();
    std::cout << "Student record updated.\n";
}

void PlacementSystem::deleteStudent() {
    std::cout << "\n-- Delete Student --\n";
    int roll = Utils::readInt("Enter Roll No to delete: ", 1, 999999999);
    Student* s = findStudentByRoll(roll);
    if (!s) throw RecordNotFoundException("No student found with roll number " + std::to_string(roll));

    s->displayInfo();
    if (!Utils::readYesNo("Are you sure you want to delete this student? (y/n): ")) {
        std::cout << "Cancelled.\n";
        return;
    }

    // Cascade delete: remove this student's applications too.
    applications.erase(std::remove_if(applications.begin(), applications.end(),
        [roll](const Application& a) { return a.getStudentRoll() == roll; }), applications.end());

    students.erase(std::remove_if(students.begin(), students.end(),
        [roll](const Student& st) { return st.getRollNo() == roll; }), students.end());

    saveAll();
    std::cout << "Student deleted.\n";
}

void PlacementSystem::searchStudent() {
    std::cout << "\n-- Search Student --\n";
    std::cout << "  1. By Roll No (exact)\n  2. By Name (contains)\n  3. By Branch\n";
    int mode = Utils::readInt("Choose search mode: ", 1, 3);

    std::vector<const Student*> matches;
    if (mode == 1) {
        int roll = Utils::readInt("Roll No: ", 1, 999999999);
        Student* s = findStudentByRoll(roll);
        if (s) matches.push_back(s);
    } else if (mode == 2) {
        std::string query = Utils::toLower(Utils::readLine("Name contains: "));
        for (const auto& s : students)
            if (Utils::toLower(s.getName()).find(query) != std::string::npos)
                matches.push_back(&s);
    } else {
        std::string branch = Utils::toLower(Utils::readLine("Branch: "));
        for (const auto& s : students)
            if (Utils::toLower(s.getBranch()) == branch)
                matches.push_back(&s);
    }

    if (matches.empty()) {
        std::cout << "No matching students found.\n";
        return;
    }
    std::cout << "\nFound " << matches.size() << " student(s):\n";
    for (const auto* s : matches) {
        std::cout << "----------------------------\n";
        s->displayInfo();
    }
}

void PlacementSystem::listAllStudents() const {
    std::cout << "\n-- All Students --\n";
    if (students.empty()) {
        std::cout << "No students registered yet.\n";
        return;
    }
    std::cout << "  1. Sort by Roll No\n  2. Sort by Name\n  3. Sort by CGPA (desc)\n";
    int mode = Utils::readInt("Sort order: ", 1, 3);

    std::vector<Student> sorted = students; // sort a copy - don't mutate stored order
    if (mode == 1) {
        std::sort(sorted.begin(), sorted.end(),
            [](const Student& a, const Student& b) { return a.getRollNo() < b.getRollNo(); });
    } else if (mode == 2) {
        std::sort(sorted.begin(), sorted.end(),
            [](const Student& a, const Student& b) { return a.getName() < b.getName(); });
    } else {
        std::sort(sorted.begin(), sorted.end(),
            [](const Student& a, const Student& b) { return a.getCgpa() > b.getCgpa(); });
    }

    std::cout << std::left << std::setw(8) << "Roll" << std::setw(20) << "Name"
              << std::setw(10) << "Branch" << std::setw(8) << "CGPA"
              << std::setw(10) << "Backlogs" << "Placed\n";
    std::cout << std::string(66, '-') << "\n";
    for (const auto& s : sorted) {
        std::cout << std::left << std::setw(8) << s.getRollNo() << std::setw(20) << s.getName()
                  << std::setw(10) << s.getBranch() << std::setw(8) << std::fixed << std::setprecision(2) << s.getCgpa()
                  << std::setw(10) << s.getBacklogs() << (s.isPlaced() ? "Yes" : "No") << "\n";
    }
}

// ------------------------------------------------------------
// Company management (admin side)
// ------------------------------------------------------------

void PlacementSystem::addCompany() {
    std::cout << "\n-- Add Company --\n";
    std::string name = Utils::readLine("Company Name: ");
    std::string role = Utils::readLine("Job Role: ");
    double package = Utils::readDouble("Package offered (LPA): ", 0.0, 1000.0);
    double minCgpa = Utils::readDouble("Minimum CGPA required: ", 0.0, 10.0);
    int maxBacklogs = Utils::readInt("Maximum backlogs allowed: ", 0, 100);
    std::string branchLine = Utils::readLine("Allowed branches (comma separated, blank = ALL): ", true);
    auto branches = EligibilityCriteria::branchesFromString(branchLine.empty() ? "ALL" : branchLine);

    EligibilityCriteria criteria(minCgpa, maxBacklogs, branches);
    companies.emplace_back(nextCompanyId++, name, role, package, criteria);
    saveAll();
    std::cout << "Company added with ID " << companies.back().getId() << ".\n";
}

void PlacementSystem::updateCompany() {
    std::cout << "\n-- Update Company --\n";
    int id = Utils::readInt("Enter Company ID to update: ", 1, 999999999);
    Company* c = findCompanyById(id);
    if (!c) throw RecordNotFoundException("No company found with ID " + std::to_string(id));

    std::cout << "Current record:\n";
    c->displayInfo();
    std::cout << "\nLeave a field blank to keep its current value.\n";

    std::string name = Utils::readLine("New Name [" + c->getName() + "]: ", true);
    if (!name.empty()) c->setName(name);

    std::string role = Utils::readLine("New Role [" + c->getRole() + "]: ", true);
    if (!role.empty()) c->setRole(role);

    std::string pkgStr = Utils::readLine("New Package [" + std::to_string(c->getPackage()) + "]: ", true);
    if (!pkgStr.empty()) c->setPackage(std::stod(pkgStr));

    std::string cgpaStr = Utils::readLine("New Min CGPA [" + std::to_string(c->getCriteria().getMinCgpa()) + "]: ", true);
    if (!cgpaStr.empty()) c->getCriteria().setMinCgpa(std::stod(cgpaStr));

    std::string backStr = Utils::readLine("New Max Backlogs [" + std::to_string(c->getCriteria().getMaxBacklogs()) + "]: ", true);
    if (!backStr.empty()) c->getCriteria().setMaxBacklogs(std::stoi(backStr));

    std::string branchLine = Utils::readLine("New Allowed Branches (comma separated, 'ALL' for all) [" + c->getCriteria().branchesToString() + "]: ", true);
    if (!branchLine.empty()) c->getCriteria().setAllowedBranches(EligibilityCriteria::branchesFromString(branchLine));

    saveAll();
    std::cout << "Company record updated.\n";
}

void PlacementSystem::deleteCompany() {
    std::cout << "\n-- Delete Company --\n";
    int id = Utils::readInt("Enter Company ID to delete: ", 1, 999999999);
    Company* c = findCompanyById(id);
    if (!c) throw RecordNotFoundException("No company found with ID " + std::to_string(id));

    c->displayInfo();
    if (!Utils::readYesNo("Are you sure you want to delete this company? (y/n): ")) {
        std::cout << "Cancelled.\n";
        return;
    }

    // Cascade: remove related applications and un-place any students placed here.
    applications.erase(std::remove_if(applications.begin(), applications.end(),
        [id](const Application& a) { return a.getCompanyId() == id; }), applications.end());
    for (auto& s : students)
        if (s.isPlaced() && s.getPlacedCompanyId() == id) s.setPlaced(false);

    companies.erase(std::remove_if(companies.begin(), companies.end(),
        [id](const Company& c2) { return c2.getId() == id; }), companies.end());

    saveAll();
    std::cout << "Company deleted.\n";
}

void PlacementSystem::searchCompany() {
    std::cout << "\n-- Search Company --\n";
    std::cout << "  1. By Company ID (exact)\n  2. By Name (contains)\n";
    int mode = Utils::readInt("Choose search mode: ", 1, 2);

    std::vector<const Company*> matches;
    if (mode == 1) {
        int id = Utils::readInt("Company ID: ", 1, 999999999);
        Company* c = findCompanyById(id);
        if (c) matches.push_back(c);
    } else {
        std::string query = Utils::toLower(Utils::readLine("Name contains: "));
        for (const auto& c : companies)
            if (Utils::toLower(c.getName()).find(query) != std::string::npos)
                matches.push_back(&c);
    }

    if (matches.empty()) {
        std::cout << "No matching companies found.\n";
        return;
    }
    std::cout << "\nFound " << matches.size() << " compan" << (matches.size() == 1 ? "y" : "ies") << ":\n";
    for (const auto* c : matches) {
        std::cout << "----------------------------\n";
        c->displayInfo();
    }
}

void PlacementSystem::listAllCompanies() const {
    std::cout << "\n-- All Companies --\n";
    if (companies.empty()) {
        std::cout << "No companies registered yet.\n";
        return;
    }
    std::vector<Company> sorted = companies;
    std::sort(sorted.begin(), sorted.end(),
        [](const Company& a, const Company& b) { return a.getPackage() > b.getPackage(); });

    std::cout << std::left << std::setw(6) << "ID" << std::setw(22) << "Name"
              << std::setw(16) << "Role" << std::setw(10) << "Package" << "Eligibility\n";
    std::cout << std::string(80, '-') << "\n";
    for (const auto& c : sorted) {
        std::cout << std::left << std::setw(6) << c.getId() << std::setw(22) << c.getName()
                   << std::setw(16) << c.getRole() << std::setw(10) << std::fixed << std::setprecision(2) << c.getPackage()
                   << c.getCriteria().toString() << "\n";
    }
}

// ------------------------------------------------------------
// Applications & reporting (admin side)
// ------------------------------------------------------------

void PlacementSystem::viewEligibleStudentsForCompany() {
    std::cout << "\n-- View Eligible Students for a Company --\n";
    int id = Utils::readInt("Company ID: ", 1, 999999999);
    Company* c = findCompanyById(id);
    if (!c) throw RecordNotFoundException("No company found with ID " + std::to_string(id));

    std::vector<const Student*> eligible;
    for (const auto& s : students)
        if (c->getCriteria().isEligible(s)) eligible.push_back(&s);

    std::sort(eligible.begin(), eligible.end(),
        [](const Student* a, const Student* b) { return a->getCgpa() > b->getCgpa(); });

    std::cout << "\nCompany: " << c->getName() << " | " << c->getCriteria().toString() << "\n";
    if (eligible.empty()) {
        std::cout << "No students currently meet this company's eligibility criteria.\n";
        return;
    }
    std::cout << eligible.size() << " eligible student(s):\n";
    std::cout << std::left << std::setw(8) << "Roll" << std::setw(20) << "Name"
              << std::setw(10) << "Branch" << "CGPA\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto* s : eligible) {
        std::cout << std::left << std::setw(8) << s->getRollNo() << std::setw(20) << s->getName()
                   << std::setw(10) << s->getBranch() << std::fixed << std::setprecision(2) << s->getCgpa() << "\n";
    }
}

void PlacementSystem::viewApplicationsAndStats() const {
    std::cout << "\n-- Applications & Placement Statistics --\n";
    if (applications.empty()) {
        std::cout << "No applications have been submitted yet.\n";
    } else {
        std::cout << std::left << std::setw(6) << "AppID" << std::setw(8) << "Roll"
                   << std::setw(12) << "CompanyID" << std::setw(14) << "Status" << "Date\n";
        std::cout << std::string(50, '-') << "\n";
        for (const auto& a : applications) {
            std::cout << std::left << std::setw(6) << a.getId() << std::setw(8) << a.getStudentRoll()
                       << std::setw(12) << a.getCompanyId() << std::setw(14) << statusToString(a.getStatus())
                       << a.getDate() << "\n";
        }
    }

    int totalApplied = (int)std::count_if(applications.begin(), applications.end(),
        [](const Application& a) { return a.getStatus() == Status::Applied; });
    int totalShortlisted = (int)std::count_if(applications.begin(), applications.end(),
        [](const Application& a) { return a.getStatus() == Status::Shortlisted; });
    int totalSelected = (int)std::count_if(applications.begin(), applications.end(),
        [](const Application& a) { return a.getStatus() == Status::Selected; });
    int totalRejected = (int)std::count_if(applications.begin(), applications.end(),
        [](const Application& a) { return a.getStatus() == Status::Rejected; });

    int placedCount = (int)std::count_if(students.begin(), students.end(),
        [](const Student& s) { return s.isPlaced(); });

    std::cout << "\nSummary:\n";
    std::cout << "  Total Applications : " << applications.size() << "\n";
    std::cout << "    Applied          : " << totalApplied << "\n";
    std::cout << "    Shortlisted      : " << totalShortlisted << "\n";
    std::cout << "    Selected         : " << totalSelected << "\n";
    std::cout << "    Rejected         : " << totalRejected << "\n";
    std::cout << "  Total Students     : " << students.size() << "\n";
    std::cout << "  Placed Students    : " << placedCount << "\n";
    if (!students.empty()) {
        double pct = 100.0 * placedCount / (double)students.size();
        std::cout << "  Placement %        : " << std::fixed << std::setprecision(2) << pct << "%\n";
    }
}

void PlacementSystem::manageApplicationStatus() {
    std::cout << "\n-- Update Application Status --\n";
    if (applications.empty()) {
        std::cout << "No applications to update.\n";
        return;
    }
    int appId = Utils::readInt("Enter Application ID: ", 1, 999999999);
    auto it = std::find_if(applications.begin(), applications.end(),
        [appId](const Application& a) { return a.getId() == appId; });
    if (it == applications.end())
        throw RecordNotFoundException("No application found with ID " + std::to_string(appId));

    std::cout << "Current status: " << statusToString(it->getStatus()) << "\n";
    std::cout << "  1. Applied\n  2. Shortlisted\n  3. Selected\n  4. Rejected\n";
    int choice = Utils::readInt("New status: ", 1, 4);
    Status newStatus = static_cast<Status>(choice - 1);
    it->setStatus(newStatus);

    // If a student is marked Selected, reflect that on their profile.
    if (newStatus == Status::Selected) {
        Student* s = findStudentByRoll(it->getStudentRoll());
        if (s) s->setPlaced(true, it->getCompanyId());
    }

    saveAll();
    std::cout << "Application status updated to " << statusToString(newStatus) << ".\n";
}

void PlacementSystem::generateSummaryReport() const {
    std::cout << "\n-- Generate Summary Report --\n";
    std::string path = dataDir + "/summary_report.txt";
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) throw FileIOException("Could not create report file.");

    int placedCount = (int)std::count_if(students.begin(), students.end(),
        [](const Student& s) { return s.isPlaced(); });

    out << "==================================================\n";
    out << " PLACEMENT CELL - SUMMARY REPORT\n";
    out << " Generated on: " << Utils::currentDate() << "\n";
    out << "==================================================\n\n";
    out << "Total Students Registered : " << students.size() << "\n";
    out << "Total Companies Registered: " << companies.size() << "\n";
    out << "Total Applications Filed  : " << applications.size() << "\n";
    out << "Total Students Placed     : " << placedCount << "\n";
    if (!students.empty()) {
        double pct = 100.0 * placedCount / (double)students.size();
        out << "Overall Placement %       : " << std::fixed << std::setprecision(2) << pct << "%\n";
    }

    out << "\n--- Branch-wise placement ---\n";
    std::vector<std::string> branches;
    for (const auto& s : students) {
        if (std::find(branches.begin(), branches.end(), s.getBranch()) == branches.end())
            branches.push_back(s.getBranch());
    }
    for (const auto& br : branches) {
        int total = (int)std::count_if(students.begin(), students.end(),
            [&](const Student& s) { return s.getBranch() == br; });
        int placed = (int)std::count_if(students.begin(), students.end(),
            [&](const Student& s) { return s.getBranch() == br && s.isPlaced(); });
        out << "  " << br << ": " << placed << " / " << total << " placed\n";
    }

    out << "\n--- Company-wise applications ---\n";
    for (const auto& c : companies) {
        int count = (int)std::count_if(applications.begin(), applications.end(),
            [&](const Application& a) { return a.getCompanyId() == c.getId(); });
        int selected = (int)std::count_if(applications.begin(), applications.end(),
            [&](const Application& a) { return a.getCompanyId() == c.getId() && a.getStatus() == Status::Selected; });
        out << "  " << c.getName() << " (ID " << c.getId() << "): "
            << count << " applications, " << selected << " selected\n";
    }

    out.close();
    std::cout << "Report generated: " << path << "\n";
}

// ============================================================
// Student menu
// ============================================================

void PlacementSystem::studentMenu(Student& s) {
    while (true) {
        std::cout << "\n--------------- STUDENT MENU ---------------\n";
        std::cout << "  1. View Profile\n";
        std::cout << "  2. Update Profile\n";
        std::cout << "  3. View Eligible Companies\n";
        std::cout << "  4. Apply to a Company\n";
        std::cout << "  5. Track My Application Status\n";
        std::cout << "  6. View Placement History\n";
        std::cout << "  7. Logout\n";
        std::cout << "---------------------------------------------\n";
        int choice = Utils::readInt("Enter choice: ", 1, 7);

        try {
            switch (choice) {
                case 1: viewProfile(s); break;
                case 2: updateProfile(s); break;
                case 3: viewEligibleCompanies(s); break;
                case 4: applyToCompany(s); break;
                case 5: trackApplications(s); break;
                case 6: viewPlacementHistory(s); break;
                case 7:
                    saveAll();
                    std::cout << "Logged out.\n";
                    return;
            }
        } catch (const PlacementException& ex) {
            std::cout << "\n[Error] " << ex.what() << "\n";
        }
        Utils::pause();
    }
}

void PlacementSystem::viewProfile(const Student& s) const {
    std::cout << "\n-- My Profile --\n";
    s.displayInfo();
}

void PlacementSystem::updateProfile(Student& s) {
    std::cout << "\n-- Update Profile --\n";
    std::cout << "Academic fields (CGPA/backlogs) update each semester; other details are yours to manage.\n";
    std::cout << "Leave a field blank to keep its current value.\n";

    std::string cgpaStr = Utils::readLine("New CGPA [" + std::to_string(s.getCgpa()) + "]: ", true);
    if (!cgpaStr.empty()) s.setCgpa(std::stod(cgpaStr));

    std::string backStr = Utils::readLine("New Backlog count [" + std::to_string(s.getBacklogs()) + "]: ", true);
    if (!backStr.empty()) s.setBacklogs(std::stoi(backStr));

    if (Utils::readYesNo("Change password? (y/n): ")) {
        std::string pwd = Utils::readLine("New password: ");
        s.setPassword(pwd);
    }

    saveAll();
    std::cout << "Profile updated.\n";
}

void PlacementSystem::viewEligibleCompanies(const Student& s) const {
    std::cout << "\n-- Eligible Companies --\n";
    std::vector<const Company*> eligible;
    for (const auto& c : companies)
        if (c.getCriteria().isEligible(s)) eligible.push_back(&c);

    if (eligible.empty()) {
        std::cout << "No companies currently match your profile.\n";
        return;
    }
    std::sort(eligible.begin(), eligible.end(),
        [](const Company* a, const Company* b) { return a->getPackage() > b->getPackage(); });

    std::cout << std::left << std::setw(6) << "ID" << std::setw(22) << "Name"
              << std::setw(16) << "Role" << "Package (LPA)\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto* c : eligible) {
        std::cout << std::left << std::setw(6) << c->getId() << std::setw(22) << c->getName()
                   << std::setw(16) << c->getRole() << std::fixed << std::setprecision(2) << c->getPackage() << "\n";
    }
}

void PlacementSystem::applyToCompany(Student& s) {
    std::cout << "\n-- Apply to a Company --\n";
    int id = Utils::readInt("Enter Company ID: ", 1, 999999999);
    Company* c = findCompanyById(id);
    if (!c) throw RecordNotFoundException("No company found with ID " + std::to_string(id));

    if (!c->getCriteria().isEligible(s))
        throw InvalidInputException("You do not meet the eligibility criteria for " + c->getName() + ".");

    bool alreadyApplied = std::any_of(applications.begin(), applications.end(),
        [&](const Application& a) { return a.getStudentRoll() == s.getRollNo() && a.getCompanyId() == id; });
    if (alreadyApplied)
        throw DuplicateRecordException("You have already applied to " + c->getName() + ".");

    if (s.isPlaced() && !Utils::readYesNo("You are already placed. Apply anyway? (y/n): ")) {
        std::cout << "Application cancelled.\n";
        return;
    }

    applications.emplace_back(nextAppId++, s.getRollNo(), id, Status::Applied, Utils::currentDate());
    saveAll();
    std::cout << "Application submitted to " << c->getName() << ".\n";
}

void PlacementSystem::trackApplications(const Student& s) const {
    std::cout << "\n-- My Applications --\n";
    std::vector<const Application*> mine;
    for (const auto& a : applications)
        if (a.getStudentRoll() == s.getRollNo()) mine.push_back(&a);

    if (mine.empty()) {
        std::cout << "You have not applied to any companies yet.\n";
        return;
    }
    std::cout << std::left << std::setw(6) << "AppID" << std::setw(22) << "Company"
              << std::setw(14) << "Status" << "Date\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto* a : mine) {
        std::string compName = "Unknown";
        for (const auto& c : companies)
            if (c.getId() == a->getCompanyId()) { compName = c.getName(); break; }
        std::cout << std::left << std::setw(6) << a->getId() << std::setw(22) << compName
                   << std::setw(14) << statusToString(a->getStatus()) << a->getDate() << "\n";
    }
}

void PlacementSystem::viewPlacementHistory(const Student& s) const {
    std::cout << "\n-- Placement History --\n";
    if (!s.isPlaced()) {
        std::cout << "You have not been placed yet. Keep applying!\n";
        return;
    }
    const Company* placedAt = nullptr;
    for (const auto& c : companies)
        if (c.getId() == s.getPlacedCompanyId()) { placedAt = &c; break; }

    std::cout << "Congratulations! You have been placed.\n";
    if (placedAt) {
        placedAt->displayInfo();
    } else {
        std::cout << "(Details of the placing company are no longer available.)\n";
    }
}
