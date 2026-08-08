# Placement Tracker System

A console-based **Placement Tracker System** developed in **C++** using Object-Oriented Programming (OOP) concepts. The system is designed to help a college Training and Placement Cell manage student records, company information, eligibility criteria, job applications, and placement activities efficiently.

## 1. Project Overview

The Placement Tracker System provides separate interfaces for:

* **Administrator**
* **Student**

The Administrator can manage students and companies, define eligibility criteria, monitor applications, and generate placement statistics.

Students can maintain their profiles, check eligible companies, apply for opportunities, track application status, and view their placement history.

The application uses **file handling** for persistent storage, so data remains available even after the program is closed.

---

## 2. Key Features

### Administrator

* Secure administrator login
* Add student records
* Update student records
* Delete student records
* Search students
* Add company details
* Update company details
* Delete companies
* Search companies
* Define company eligibility criteria

  * Minimum CGPA
  * Eligible branches
  * Maximum allowed backlogs
  * Other criteria
* View students eligible for a company
* View applications
* Update application status
* View placement statistics
* Generate summary reports

### Student

* Secure student login
* View profile
* Update profile
* View eligible companies
* Apply for companies
* Track application status
* View application history
* View placement history

---

## 3. OOP Concepts Used

The project demonstrates major Object-Oriented Programming concepts in C++.

### Encapsulation

Data members of classes are kept private/protected and accessed through appropriate public member functions.

### Inheritance

Common functionality is shared through a base user class, with specialized classes for different types of users.

Example:

```text
             User
            /    \
     Administrator  Student
```

### Polymorphism

Common operations can be implemented using virtual functions and overridden by derived classes where required.

### Abstraction

Complex operations such as file management, authentication, eligibility checking, and application processing are separated into dedicated modules/classes.

### Classes and Objects

Major entities are represented using classes such as:

* `User`
* `Student`
* `Administrator`
* `Company`
* `Application`
* `FileManager`

---

## 4. File Handling

The system uses files to provide persistent storage.

Typical data files include:

```text
data/
├── students.txt
├── companies.txt
├── applications.txt
└── admin.txt
```

The files store information such as:

* Student profiles
* Company details
* Eligibility criteria
* Applications
* Application statuses
* Administrator credentials

When the application starts, stored data is loaded from files. Changes made during execution are written back to the files.

---

## 5. Project Structure

A modular structure is used to keep the project organized.

```text
Placement-Tracker/
│
├── main.cpp
│
├── include/
│   ├── User.hpp
│   ├── Student.hpp
│   ├── Administrator.hpp
│   ├── Company.hpp
│   ├── Application.hpp
│   ├── FileManager.hpp
│   └── Exceptions.hpp
│
├── src/
│   ├── User.cpp
│   ├── Student.cpp
│   ├── Administrator.cpp
│   ├── Company.cpp
│   ├── Application.cpp
│   ├── FileManager.cpp
│   └── Exceptions.cpp
│
├── data/
│   ├── students.txt
│   ├── companies.txt
│   ├── applications.txt
│   └── admin.txt
│
└── README.md
```

The exact file structure may vary depending on the final implementation.

---

## 6. System Workflow

```text
                 START
                   |
                   v
          +------------------+
          |   Main Menu      |
          +------------------+
             /            \
            /              \
           v                v
   Administrator Login   Student Login
           |                |
           v                v
   Administrator Menu    Student Menu
           |                |
     +-----+-----+     +----+-----+
     |     |     |     |    |     |
     v     v     v     v    v     v
 Students Companies Apps Profile Apply
     |       |       |     |    |
     +-------+-------+     +----+
             |
             v
       File Persistence
             |
             v
            EXIT
```

---

## 7. Eligibility Checking

Before a student applies to a company, the system checks whether the student satisfies the company's eligibility criteria.

Example criteria:

```text
Minimum CGPA       : 7.50
Maximum Backlogs   : 0
Eligible Branches  : CSE, AI&ML, ECE
```

A student is considered eligible only when the required conditions are satisfied.

This prevents students from applying to companies for which they do not meet the defined criteria.

---

## 8. Application Management

Each application connects a student with a company.

An application can contain:

```text
Application ID
Student ID
Company ID
Application Date
Application Status
```

Possible statuses include:

```text
Applied
Shortlisted
Rejected
Selected
```

The Administrator can monitor and update application statuses.

---

## 9. Searching and Sorting

The system supports efficient management of records through searching and sorting operations.

Examples:

* Search student by ID
* Search student by name
* Search company by ID
* Search company by name
* Sort students by CGPA
* Sort companies by name
* Filter companies according to eligibility
* Filter applications according to status

STL containers and algorithms are used wherever appropriate.

Examples include:

```cpp
vector
map
algorithm
sort()
find()
```

---

## 10. Exception Handling

The system uses C++ exception handling to manage invalid or unexpected situations.

Examples:

* Invalid input
* Invalid login credentials
* Duplicate student/company ID
* Student not found
* Company not found
* Invalid CGPA
* Invalid number of backlogs
* File opening failure
* Invalid application
* Attempt to apply multiple times

Example:

```cpp
try {
    // operation
}
catch (const exception& e) {
    cout << "Error: " << e.what();
}
```

---

## 11. Input Validation

The system validates user input wherever necessary.

Examples:

* CGPA must be within the valid range
* Backlogs cannot be negative
* IDs must be valid
* Required fields cannot be empty
* Duplicate IDs are prevented
* Invalid menu choices are handled
* Invalid login attempts are rejected

This helps prevent incorrect or inconsistent data from entering the system.

---

## 12. Placement Statistics

The Administrator can view useful placement statistics such as:

* Total number of students
* Total number of companies
* Total applications
* Number of selected students
* Number of rejected applications
* Number of students placed
* Company-wise applications
* Placement percentage

Example:

```text
========== PLACEMENT SUMMARY ==========

Total Students       : 120
Total Companies      : 18
Total Applications   : 245
Students Placed      : 72
Students Not Placed  : 48
Placement Percentage : 60%

========================================
```

---

## 13. Technologies Used

| Technology         | Purpose                   |
| ------------------ | ------------------------- |
| C++                | Core programming language |
| OOP                | System architecture       |
| STL                | Containers and algorithms |
| File Handling      | Persistent data storage   |
| Exception Handling | Error management          |
| Console UI         | User interaction          |

---

## 14. Requirements

### Software

* C++ compiler supporting C++17 or later
* GCC / MinGW / G++
* Windows, Linux, or macOS

### Recommended

```text
C++17
GCC / MinGW
Visual Studio Code
```

---

## 15. Compilation

If the project uses separate source and header files, compile all `.cpp` files together.

### Windows / MinGW

```bash
g++ -std=c++17 main.cpp src/*.cpp -Iinclude -o placement_tracker
```

Run:

```bash
./placement_tracker
```

On Windows PowerShell, you can also use:

```powershell
.\placement_tracker.exe
```

---

## 16. First Run

On the first execution, the system initializes the required data files and administrator account if they do not already exist.

Example:

```text
Starting Placement Tracker System...

[Setup] No administrator account found -
created default admin.

Username: admin
Password: admin123
```

The default credentials should be changed or configured appropriately for a real deployment.

---

## 17. Main Menu

The application provides a menu-driven interface similar to:

```text
========================================
       PLACEMENT TRACKER SYSTEM
========================================

1. Administrator Login
2. Student Login
3. Exit

Enter choice:
```

After successful authentication, the appropriate dashboard is displayed.

---

## 18. Administrator Dashboard

```text
========== ADMINISTRATOR MENU ==========

1. Manage Students
2. Manage Companies
3. View Eligible Students
4. View Applications
5. Update Application Status
6. Placement Statistics
7. Generate Reports
8. Logout

Enter choice:
```

---

## 19. Student Dashboard

```text
============= STUDENT MENU =============

1. View Profile
2. Update Profile
3. View Eligible Companies
4. Apply to Company
5. Track Applications
6. Placement History
7. Logout

Enter choice:
```

---

## 20. Design Goals

The project is designed with the following goals:

* Demonstrate practical use of OOP concepts
* Maintain modular and maintainable code
* Provide persistent data storage
* Reduce manual placement-cell work
* Automate eligibility checking
* Track applications systematically
* Provide useful placement statistics
* Handle invalid input safely
* Provide a simple and user-friendly console interface

---

## 21. Future Enhancements

The system can be extended with:

* Graphical User Interface
* Database integration using MySQL/SQLite
* Password hashing and stronger authentication
* Email notifications
* Resume upload and management
* Automated placement notifications
* Advanced analytics and visualization
* Role-based access control
* Export reports to CSV/PDF
* Web-based interface

---

## 22. Project Objective

The primary objective of the Placement Tracker System is to provide a structured and efficient platform for managing campus placement activities while demonstrating practical implementation of **C++ Object-Oriented Programming, STL, file handling, exception handling, modular programming, searching, sorting, and data management**.

---

## 23. Authors

**Placement Tracker System**

1. Shruti Singh : Backend – Student & Company Logic + Testing & Debugging
